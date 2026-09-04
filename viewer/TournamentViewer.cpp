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

	std::cout << "\t" << q[0].first << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[0].first << " ---\n";
	std::cout << "\t" << q[0].second << " ---\n";
	std::cout << "\t\t\t\t\t\t\t\t\t\t|---> " << f[0].first << " ---\n";
	std::cout << "\t" << q[1].first << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[0].second << " ---\n";
	std::cout << "\t" << q[1].second << " ---\n";

	if (hasThird)
		std::cout << "\t\t\t\t\t\t\t\t|--> " << t3[0].first << " ---\n";

	std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t|---> " << winner << "\n";

	if (hasThird)
		std::cout << "\t\t\t\t\t\t\t\t|--> " << t3[0].second << " ---\n";

	std::cout << "\t" << q[2].first << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[1].first << " ---\n";
	std::cout << "\t" << q[2].second << " ---\n";
	std::cout << "\t\t\t\t\t\t\t\t\t\t|---> " << f[0].second << " ---\n";
	std::cout << "\t" << q[3].first << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[1].second << " ---\n";
	std::cout << "\t" << q[3].second << " ---\n";

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
		std::cout << "\t\t\t\t1er:\t" << winners[0]->getName() << std::endl;

	if (!losers.empty() && losers[0])
		std::cout << "\t\t\t2e:\t\t" << losers[0]->getName() << std::endl;

	if (thirdPlace && thirdPlace->isFinished())
	{
		cvpTeam third = thirdPlace->getWinners();
		cvpTeam fourth = thirdPlace->getLosers();

		if (!third.empty()  && third[0])
			std::cout << "\t\t3e:\t\t\t" << third[0]->getName()  << std::endl;

		if (!fourth.empty() && fourth[0])
			std::cout << "\t4e:\t\t\t\t" << fourth[0]->getName() << std::endl;
	}

	std::cout << "══════════════════════════════════════\n";
}

/**
 * TESTER FUNCTION - TO REMOVED or DELETED
 */
void             TournamentViewer::printAll(Tournament& tournament)
{
	PrintUtils::printTitle("TournamentViewer");

	displayFullBracket(tournament);
	displayPodium(tournament);
}