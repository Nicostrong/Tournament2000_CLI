//
// Created by Nicolas Fordoxcel on 18/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <iostream>

#include "../includes/cli/TournamentViewer.hpp"

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

	std::cout << "\n=================== ARBRE DE LA PHASE FINALE ===================\n\n";
	std::cout << "\tQUARTS\t\t\tDEMIS\t\t\t\t\t3e PLACE\tFINALE\t\t\tVAINQUEUR\n\n";

	const String q[4][2] =
	{
		{ getTeamNameOrPlaceholder(Q, 0, 1), getTeamNameOrPlaceholder(Q, 0, 2) },
		{ getTeamNameOrPlaceholder(Q, 1, 1), getTeamNameOrPlaceholder(Q, 1, 2) },
		{ getTeamNameOrPlaceholder(Q, 2, 1), getTeamNameOrPlaceholder(Q, 2, 2) },
		{ getTeamNameOrPlaceholder(Q, 3, 1), getTeamNameOrPlaceholder(Q, 3, 2) }
	};
	const String s[2][2] =
	{
		{ getTeamNameOrPlaceholder(S, 0, 1), getTeamNameOrPlaceholder(S, 0, 2) },
		{ getTeamNameOrPlaceholder(S, 1, 1), getTeamNameOrPlaceholder(S, 1, 2) }
	};
	const String f[2] =
	{
		getTeamNameOrPlaceholder(F, 0, 1),
		getTeamNameOrPlaceholder(F, 0, 2)
	};
	const String t3[2] =
	{
		getTeamNameOrPlaceholder(T, 0, 1),
		getTeamNameOrPlaceholder(T, 0, 2)
	};

	String winner = "A determiner";

	if (F && F->isFinished() && !F->getMatches().empty())
	{
		cpTeam	w = F->getMatches()[0]->getWinner();

		if (w)
			winner = w->getName();
	}

	cBool hasThird = tournament.getSettings().getIsThirdPlaceMatch();

	std::cout << "\t" << q[0][0] << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[0][0] << " ---\n";
	std::cout << "\t" << q[0][1] << " ---\n";
	std::cout << "\t\t\t\t\t\t\t\t\t\t|---> " << f[0] << " ---\n";
	std::cout << "\t" << q[1][0] << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[0][1] << " ---\n";
	std::cout << "\t" << q[1][1] << " ---\n";

	if (hasThird)
		std::cout << "\t\t\t\t\t\t\t\t|--> " << t3[0] << " ---\n";

	std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t|---> " << winner << "\n";

	if (hasThird)
		std::cout << "\t\t\t\t\t\t\t\t|--> " << t3[1] << " ---\n";

	std::cout << "\t" << q[2][0] << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[1][0] << " ---\n";
	std::cout << "\t" << q[2][1] << " ---\n";
	std::cout << "\t\t\t\t\t\t\t\t\t\t|---> " << f[1] << " ---\n";
	std::cout << "\t" << q[3][0] << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[1][1] << " ---\n";
	std::cout << "\t" << q[3][1] << " ---\n";
	std::cout << "==============================================================================\n";
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
		PrintUtils::addError("La finale n'est pas encore terminee.");
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
