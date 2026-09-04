//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/viewer/PhaseViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cInt			=	const int;

using				cBool			=	const bool;

using				cpTeam			=	const Team*;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 * Ecrit une rencontre (N sets consecutifs) dans le flux out.
 *
 * Rencontre 1 :  Team A  vs  Team B
 * ─────────────────────────────────
 *   Set 1 : 21 - 15  ->  Vainqueur : Team A
 *   Set 2 : 14 - 21  ->  Vainqueur : Team B
 *   Set 3 : 21 - 18  ->  Vainqueur : Team A
 * Vainqueur de la rencontre : Team A
 */
void				PhaseViewer::writeEncounter(std::ostream& out, cvpMatch matches, const size_t startIdx,
						cInt nbSets, cInt encounterNum, cBool toFile)
{
	if (startIdx >= matches.size())
		return;

	cpMatch first = matches[startIdx];
	cString	nameA = first->getTeamA() ? first->getTeamA()->getName() : "?";
	cString	nameB = first->getTeamB() ? first->getTeamB()->getName() : "?";

	out << "\n";
	out << "  Match " << encounterNum << " :  "
		<< nameA << "  vs  " << nameB << "\n";
	out << "  " << String(nameA.size() + nameB.size() + 14, '-') << "\n";

	int winsA = 0;
	int winsB = 0;

	for (int s = 0; s < nbSets; ++s)
	{
		const size_t idx = startIdx + static_cast<size_t>(s);

		if (idx >= matches.size())
			break;

		cpMatch m = matches[idx];

		out << "    Set " << (s + 1) << " : ";

		if (!m || !m->isFinished())
		{
			out << "[ to play ]\n";
			continue;
		}

		out << std::setw(3) << m->getScoreA()
			<< " - "
			<< std::setw(3) << m->getScoreB();

		cpTeam winner = m->getWinner();

		if (winner)
		{
			if (!toFile)
				out << Color::BGREEN;

			out << "  ->  Winner : " << winner->getName();

			if (!toFile)
				out << Color::RESET;

			if (winner == first->getTeamA())
				winsA++;
			else
				winsB++;
		}
		else
			out << "  ->  Egalité";

		out << "\n";
	}

	out << "  ";

	if (winsA > winsB)
	{
		if (!toFile)
			out << Color::BYELLOW;

		out << "Winner of the match: " << nameA;

		if (!toFile)
			out << Color::RESET;
	}
	else if (winsB > winsA)
	{
		if (!toFile)
			out << Color::BYELLOW;

		out << "Winner of the match: " << nameB;

		if (!toFile)
			out << Color::RESET;
	}
	else
		out << "Match no ended.";

	out << "\n";
}

void				PhaseViewer::displayPhase(cPhase phase)
{
	cvpMatch matches = phase.getMatches();
	cInt nbSets = phase.getNbSetToPlay();

	std::cout << "\n";
	std::cout << "╔══════════════════════════════════════════════════╗\n";
	std::cout << "║  PHASE : " << std::left << std::setw(40) << phase.getName() << "║\n";
	std::cout << "║  Sets par rencontre : " << std::left << std::setw(28) << nbSets << "║\n";
	std::cout << "╚══════════════════════════════════════════════════╝\n";

	if (matches.empty())
		return (PrintUtils::addError("No matches for this stage."));

	int encounterNum = 1;

	for (size_t i = 0; i < matches.size(); i += static_cast<size_t>(nbSets))
	{
		writeEncounter(std::cout, matches, i, nbSets, encounterNum, false);
		++encounterNum;
	}

	if (phase.isFinished())
		displayResults(phase);
}

void				PhaseViewer::displayResults(cPhase phase)
{
	if (!phase.isFinished())
		return (PrintUtils::addError("Stage no ended — No results available."));

	vpTeam winners	= phase.getWinners();
	vpTeam losers	= phase.getLosers();

	std::cout << "\n  ── Results of " << phase.getName() << " ──\n";
	std::cout << "  " << String(40, '-') << "\n";

	std::cout << "  Qualified:\n";

	for (size_t i = 0; i < winners.size(); ++i)
	{
		if (!winners[i])
			continue;

		std::cout << Color::BGREEN << "    " << (i + 1) << ". " << winners[i]->getName() << Color::RESET << "\n";
	}

	if (!losers.empty())
	{
		std::cout << "  Eliminated:\n";

		for (cpTeam t : losers)
		{
			if (!t)
				continue;

			std::cout << Color::BRED << "    - " << t->getName() << Color::RESET << "\n";
		}
	}

	std::cout << "  " << String(40, '-') << "\n";
}

/**
 * TESTER FUNCTION - TO REMOVED or DELETED
 */
void				PhaseViewer::printAll(Tournament& tournament)
{
	PrintUtils::printTitle("PhaseViewer");

	Phase* finalPhase = tournament.getFinal();

	if (finalPhase)
	{
		displayPhase(*finalPhase);

		if (finalPhase->isFinished())
			displayResults(*finalPhase);
	}
}