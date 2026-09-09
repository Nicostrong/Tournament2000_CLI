//
// Created by Nicolas Fordoxcel on 18/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <iostream>

#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/cli/CLIUtils.hpp"

#include "../includes/viewer/TournamentViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/**
* Retourne le nom de l equipe en position teamPos (1=A, 2=B) du match
* matchIdx dans phase, ou "A determiner" si indisponible.
*/
String				TournamentViewer::getTeamNameOrPlaceholder(cpPhase phase, const size_t matchIdx, cInt teamPos)
{
	if (!phase)
		return ("A determiner");

	cvpMatch matches = phase->getMatches();

	if (matchIdx >= matches.size() || !matches[matchIdx])
		return ("A determiner");

	Team* t = (teamPos == 1)	? matches[matchIdx]->getTeamA()
								: matches[matchIdx]->getTeamB();

	return (t ? t->getName() : "A determiner");
}

void				TournamentViewer::extractEncounterNames(cpPhase phase, size_t count, vpairString& outNames)
{
	outNames.clear();
	outNames.reserve(count);

	for (size_t i = 0; i < count; ++i)
		outNames.emplace_back(getTeamNameOrPlaceholder(phase, i, 1),getTeamNameOrPlaceholder(phase, i, 2));
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/*void				TournamentViewer::displayFullBracket(cTour tournament)
{
	cpPhase Q = tournament.getQuarters();
	cpPhase S = tournament.getSemis();
	cpPhase F = tournament.getFinal();
	cpPhase T = tournament.getThirdPlace();

	PrintUtils::clear();
	PrintUtils::printSeparator('=', 150);
	PrintUtils::printTitle("ARBRE DE LA PHASE FINALE", 150);
	PrintUtils::printSeparator('=', 150);
	std::cout << "\tQUARTS\t\t\tDEMIS\t\t\t\t\t3e PLACE\tFINALE\t\t\tVAINQUEUR\n\n";

	vpairString q;
	vpairString s;
	vpairString f;
	vpairString t3;

	extractEncounterNames(Q, 4, q);
	extractEncounterNames(S, 2, s);
	extractEncounterNames(F, 1, f);
	extractEncounterNames(T, 1, t3);

	String winner = "A determiner";

	if (F && F->isFinished() && !F->getMatches().empty())
	{
		cpTeam	w = F->getMatches()[0]->getWinner();

		if (w)
			winner = w->getName();
	}

	cBool hasThird = tournament.getSettings().getIsThirdPlaceMatch();

	std::cout << "\t" << q[0].first << "\t---|\n";
	std::cout << "\t\t\t|---> " << s[0].first << "\t---|\n";
	std::cout << "\t" << q[0].second << "\t---|\n";
	std::cout << "\t\t\t\t\t\t\t\t|---> " << f[0].first << "\t---|\n";
	std::cout << "\t" << q[1].first << "\t---|\n";
	std::cout << "\t\t\t|---> " << s[0].second << "\t---|\n";
	std::cout << "\t" << q[1].second << "\t---|\n";

	if (hasThird)
		std::cout << "\t\t\t\t\t|--> " << t3[0].first << "\n";

	std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t|---> " << winner << "\n";

	if (hasThird)
		std::cout << "\t\t\t\t\t|--> " << t3[0].second << "\n";

	std::cout << "\t" << q[2].first << "\t---|\n";
	std::cout << "\t\t\t|---> " << s[1].first << "\t---|\n";
	std::cout << "\t" << q[2].second << "\t---|\n";
	std::cout << "\t\t\t\t\t\t\t\t|---> " << f[0].second << "\t---|\n";
	std::cout << "\t" << q[3].first << "\t---|\n";
	std::cout << "\t\t\t|---> " << s[1].second << " ---\n";
	std::cout << "\t" << q[3].second << "\t---|\n";

	PrintUtils::printSeparator('=', 150);
}*/

void				TournamentViewer::displayFullBracket(cTour tournament)
{
    struct PhaseInfo {
        std::string name;
        cpPhase phase;
        int count;
        vpairString pairs;
    };

    // 1. Détection dynamique des phases présentes dans le tournoi
    std::vector<PhaseInfo> phases;

    if (cpPhase p = tournament.getSixteenth())
        phases.push_back({"1/16 FINALE", p, 16, {}});
    if (cpPhase p = tournament.getEighth())
        phases.push_back({"1/8 FINALE", p, 8, {}});
    if (cpPhase p = tournament.getQuarters())
        phases.push_back({"QUARTS", p, 4, {}});
    if (cpPhase p = tournament.getSemis())
        phases.push_back({"DEMIS", p, 2, {}});
    if (cpPhase p = tournament.getFinal())
        phases.push_back({"FINALE", p, 1, {}});

    if (phases.empty())
        return;

    // Extraction des paires pour chaque phase active
    for (auto& info : phases)
        extractEncounterNames(info.phase, info.count, info.pairs);

    int totalRounds = static_cast<int>(phases.size());
    int initialMatches = phases[0].count;

    // 2. Calcul des dimensions du Canvas 2D
    const int colWidth = 24;
    int canvasHeight = initialMatches * 4; // 4 lignes par match au 1er tour

    // On ajoute de l'espace supplémentaire en bas pour dessiner la petite finale
    bool hasThirdPlace = tournament.getSettings().getIsThirdPlaceMatch() && tournament.getThirdPlace();
    if (hasThirdPlace) {
        canvasHeight += 6;
    }

    int canvasWidth = (totalRounds + 1) * colWidth + 10;

    std::vector<std::string> canvas(canvasHeight, std::string(canvasWidth, ' '));

    auto drawText = [&](int x, int y, const std::string& str) {
        if (y < 0 || y >= canvasHeight)
            return;
        for (size_t i = 0; i < str.length() && (static_cast<size_t>(x) + i) < static_cast<size_t>(canvasWidth); ++i)
            canvas[y][x + i] = str[i];
    };

    // 3. Calcul dynamique des coordonnées Y
    std::vector<std::vector<int>> matchMidpoints(totalRounds);

    // Tour Initial (Round 0)
    matchMidpoints[0].resize(initialMatches);
    for (int i = 0; i < initialMatches; ++i)
    {
        int y1 = i * 4;
        int y2 = i * 4 + 2;
        int yMid = i * 4 + 1;
        matchMidpoints[0][i] = yMid;

        std::string team1 = (i < (int)phases[0].pairs.size()) ? phases[0].pairs[i].first : "A determiner";
        std::string team2 = (i < (int)phases[0].pairs.size()) ? phases[0].pairs[i].second : "A determiner";

        drawText(0, y1, team1);
        drawText(0, y2, team2);

        // Connecteurs
        canvas[y1][16] = '+';
        canvas[y2][16] = '+';
        canvas[yMid][16] = '|';
        canvas[yMid][17] = '-';
        canvas[yMid][18] = '-';
        canvas[yMid][19] = '>';
    }

    // Tours suivants (Round 1 à Finale)
    for (int r = 1; r < totalRounds; ++r)
    {
        int matchCount = phases[r].count;
        matchMidpoints[r].resize(matchCount);
        int x = r * colWidth;

        for (int i = 0; i < matchCount; ++i)
        {
            int prevY1 = matchMidpoints[r - 1][2 * i];
            int prevY2 = matchMidpoints[r - 1][2 * i + 1];
            int yMid = (prevY1 + prevY2) / 2;
            matchMidpoints[r][i] = yMid;

            std::string team1 = (i < (int)phases[r].pairs.size()) ? phases[r].pairs[i].first : "A determiner";
            std::string team2 = (i < (int)phases[r].pairs.size()) ? phases[r].pairs[i].second : "A determiner";

            drawText(x, prevY1, team1);
            drawText(x, prevY2, team2);

            // Ligne verticale reliant les deux rencontres précédentes
            for (int y = prevY1; y <= prevY2; ++y)
                canvas[y][x + 16] = '|';

            canvas[prevY1][x + 16] = '+';
            canvas[prevY2][x + 16] = '+';

            // Connecteur vers le tour suivant
            canvas[yMid][x + 17] = '-';
            canvas[yMid][x + 18] = '-';
            canvas[yMid][x + 19] = '>';
        }
    }

    // Récupération du Vainqueur Final
    std::string winner = "A determiner";
    cpPhase finalPhase = phases.back().phase;
    if (finalPhase && finalPhase->isFinished() && !finalPhase->getMatches().empty())
        if (cpTeam w = finalPhase->getMatches()[0]->getWinner())
            winner = w->getName();

    drawText(totalRounds * colWidth, matchMidpoints.back()[0], winner);

    // ==========================================
    // AJOUT: Intégration de la Petite Finale
    // ==========================================
    if (hasThirdPlace)
    {
        cpPhase T = tournament.getThirdPlace();
        vpairString t3;
        extractEncounterNames(T, 1, t3);

        // On l'aligne sur la même colonne que la Finale
        int x = (totalRounds - 1) * colWidth;

        // Coordonnées Y tout en bas du canvas
        int y1 = canvasHeight - 5;
        int y2 = canvasHeight - 3;
        int yMid = (y1 + y2) / 2;

        std::string team1 = (!t3.empty()) ? t3[0].first : "A determiner";
        std::string team2 = (!t3.empty()) ? t3[0].second : "A determiner";

        drawText(x, y1 - 1, "[PETITE FINALE]");
        drawText(x, y1, team1);
        drawText(x, y2, team2);

        // Connecteurs de la petite finale
        canvas[y1][x + 16] = '+';
        canvas[y2][x + 16] = '+';
        canvas[yMid][x + 16] = '|';
        canvas[yMid][x + 17] = '-';
        canvas[yMid][x + 18] = '-';
        canvas[yMid][x + 19] = '>';

        // Vainqueur de la 3e place
        std::string thirdWinner = "A determiner";
        if (T && T->isFinished() && !T->getMatches().empty())
            if (cpTeam w3 = T->getMatches()[0]->getWinner())
                thirdWinner = w3->getName() + " (3e)";

        drawText(totalRounds * colWidth, yMid, thirdWinner);
    }

    // 4. Rendu dans la console
    PrintUtils::clear();
    PrintUtils::printSeparator('=', 150);
    PrintUtils::printTitle("ARBRE DU TOURNOI", 150);
    PrintUtils::printSeparator('=', 150);

    // En-têtes de colonnes
    for (int r = 0; r < totalRounds; ++r)
    {
        std::cout << phases[r].name;
        int padding = colWidth - static_cast<int>(phases[r].name.length());
        std::cout << std::string(std::max(1, padding), ' ');
    }
    std::cout << "VAINQUEUR\n\n";

    // Affichage ligne par ligne du canvas
    for (const auto& line : canvas)
    {
        size_t end = line.find_last_not_of(' ');

        if (end != std::string::npos)
            std::cout << line.substr(0, end + 1) << "\n";
        else
            std::cout << "\n";
    }

    PrintUtils::printSeparator('=', 150);
}

void				TournamentViewer::displayPodium(cTour tournament)
{
	cpPhase final = tournament.getFinal();
	cpPhase thirdPlace = tournament.getThirdPlace();

	std::cout << "\n╔══════════════════════════════════════╗\n";
	std::cout << "║            PALMARES FINAL            ║\n";
	std::cout << "╚══════════════════════════════════════╝\n";

	if (!final || !final->isFinished())
	{
		PrintUtils::addError("Final no ended.");
		return;
	}

	cvpTeam winners = final->getWinners();
	cvpTeam losers = final->getLosers();

	if (!winners.empty() && winners[0])
		std::cout << "\t1er:\t" << winners[0]->getName() << std::endl;

	if (!losers.empty() && losers[0])
		std::cout << "\t2e:\t" << losers[0]->getName() << std::endl;

	if (thirdPlace && thirdPlace->isFinished())
	{
		cvpTeam third = thirdPlace->getWinners();
		cvpTeam fourth = thirdPlace->getLosers();

		if (!third.empty()  && third[0])
			std::cout << "\t3e:\t" << third[0]->getName()  << std::endl;

		if (!fourth.empty() && fourth[0])
			std::cout << "\t4e:\t" << fourth[0]->getName() << std::endl;
	}

	std::cout << "══════════════════════════════════════\n";
}

/**
* TESTER FUNCTION - TO REMOVED or DELETED
*/
void				TournamentViewer::printAll(Tournament& tournament)
{
	PrintUtils::printTitle("TournamentViewer");

	displayFullBracket(tournament);
	displayPodium(tournament);
}