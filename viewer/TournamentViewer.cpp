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

void				TournamentViewer::displayFullBracket(cTour tournament)
{
	struct PhaseInfo {
		std::string name;
		cpPhase phase;
		int count;
		vpairString pairs;
	};

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

	for (auto& info : phases)
		extractEncounterNames(info.phase, info.count, info.pairs);

	int totalRounds = static_cast<int>(phases.size());
	int initialMatches = phases[0].count;
	const int colWidth = 24;
	int canvasHeight = initialMatches * 4;

	bool hasThirdPlace = tournament.getSettings().getIsThirdPlaceMatch() && tournament.getThirdPlace();
	
	if (hasThirdPlace)
		canvasHeight += 6;

	int canvasWidth = (totalRounds + 1) * colWidth + 10;

	std::vector<std::string> canvas(canvasHeight, std::string(canvasWidth, ' '));

	auto drawText = [&](int x, int y, const std::string& str) {
		if (y < 0 || y >= canvasHeight)
			return;
		for (size_t i = 0; i < str.length() && (static_cast<size_t>(x) + i) < static_cast<size_t>(canvasWidth); ++i)
			canvas[y][x + i] = str[i];
	};

	std::vector<std::vector<int>> matchMidpoints(totalRounds);

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

		canvas[y1][16] = '+';
		canvas[y2][16] = '+';
		canvas[yMid][16] = '|';
		canvas[yMid][17] = '-';
		canvas[yMid][18] = '-';
		canvas[yMid][19] = '>';
	}

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

			for (int y = prevY1; y <= prevY2; ++y)
				canvas[y][x + 16] = '|';

			canvas[prevY1][x + 16] = '+';
			canvas[prevY2][x + 16] = '+';
			canvas[yMid][x + 17] = '-';
			canvas[yMid][x + 18] = '-';
			canvas[yMid][x + 19] = '>';
		}
	}

	std::string winner = "A determiner";
	cpPhase finalPhase = phases.back().phase;

	if (finalPhase && finalPhase->isFinished() && !finalPhase->getMatches().empty())
		if (cpTeam w = finalPhase->getMatches()[0]->getWinner())
			winner = w->getName();

	drawText(totalRounds * colWidth, matchMidpoints.back()[0], winner);

	if (hasThirdPlace)
	{
		cpPhase T = tournament.getThirdPlace();
		vpairString t3;

		extractEncounterNames(T, 1, t3);

		int x = (totalRounds - 1) * colWidth;
		int y1 = canvasHeight - 5;
		int y2 = canvasHeight - 3;
		int yMid = (y1 + y2) / 2;

		std::string team1 = (!t3.empty()) ? t3[0].first : "A determiner";
		std::string team2 = (!t3.empty()) ? t3[0].second : "A determiner";

		drawText(x, y1 - 1, "[PETITE FINALE]");
		drawText(x, y1, team1);
		drawText(x, y2, team2);

		canvas[y1][x + 16] = '+';
		canvas[y2][x + 16] = '+';
		canvas[yMid][x + 16] = '|';
		canvas[yMid][x + 17] = '-';
		canvas[yMid][x + 18] = '-';
		canvas[yMid][x + 19] = '>';

		std::string thirdWinner = "A determiner";

		if (T && T->isFinished() && !T->getMatches().empty())
			if (cpTeam w3 = T->getMatches()[0]->getWinner())
				thirdWinner = w3->getName() + " (3e)";

		drawText(totalRounds * colWidth, yMid, thirdWinner);
	}

	PrintUtils::clear();
	PrintUtils::printSeparator(std::cout, '=', 150);
	PrintUtils::printTitle(std::cout, "ARBRE DU TOURNOI", 150);
	PrintUtils::printSeparator(std::cout, '=', 150);

	for (int r = 0; r < totalRounds; ++r)
	{
		std::cout << phases[r].name;
		int padding = colWidth - static_cast<int>(phases[r].name.length());
		std::cout << std::string(std::max(1, padding), ' ');
	}
	std::cout << "VAINQUEUR\n\n";

	for (const auto& line : canvas)
	{
		size_t end = line.find_last_not_of(' ');

		if (end != std::string::npos)
			std::cout << line.substr(0, end + 1) << "\n";
		else
			std::cout << "\n";
	}

	PrintUtils::printSeparator(std::cout, '=', 150);
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
