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

#include "../includes/viewer/PhaseViewer.hpp"

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
	out << "  Rencontre " << encounterNum << " :  "
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
			out << "[ à jouer ]\n";
			continue;
		}

		out << std::setw(3) << m->getScoreA()
			<< " - "
			<< std::setw(3) << m->getScoreB();

		cpTeam winner = m->getWinner();

		if (winner)
		{
			if (!toFile)
				out << "\033[1;32m";

			out << "  ->  Vainqueur : " << winner->getName();

			if (!toFile)
				out << "\033[0m";

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
			out << "\033[1;33m";

		out << "Vainqueur de la rencontre : " << nameA;

		if (!toFile)
			out << "\033[0m";
	}
	else if (winsB > winsA)
	{
		if (!toFile)
			out << "\033[1;33m";

		out << "Vainqueur de la rencontre : " << nameB;

		if (!toFile)
			out << "\033[0m";
	}
	else
		out << "Rencontre non terminée";

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
	{
		std::cout << "  Aucun match enregistré dans cette phase.\n";
		return;
	}

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
	{
		std::cout << "\033[1;33m  [!] Phase non terminée — résultats indisponibles.\033[0m\n";
		return;
	}

	vpTeam winners	= phase.getWinners();
	vpTeam losers	= phase.getLosers();

	std::cout << "\n  ── Résultats de " << phase.getName() << " ──\n";
	std::cout << "  " << String(40, '-') << "\n";

	std::cout << "  Qualifiés :\n";

	for (size_t i = 0; i < winners.size(); ++i)
	{
		if (!winners[i])
			continue;

		std::cout << "\033[1;32m    " << (i + 1) << ". " << winners[i]->getName() << "\033[0m\n";
	}

	if (!losers.empty())
	{
		std::cout << "  Éliminés :\n";

		for (cpTeam t : losers)
		{
			if (!t)
				continue;

			std::cout << "\033[1;31m    - " << t->getName() << "\033[0m\n";
		}
	}

	std::cout << "  " << String(40, '-') << "\n";
}
