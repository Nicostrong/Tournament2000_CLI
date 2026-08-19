//
// Created by Nicolas Fordoxcel on 22/07/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>

#include "../includes/cli/PhaseCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"

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
 * Ecrit une rencontre (N sets consecutifs) dans le flux out.
 *
 * Rencontre 1 :  Team A  vs  Team B
 * ─────────────────────────────────
 *   Set 1 : 21 - 15  ->  Vainqueur : Team A
 *   Set 2 : 14 - 21  ->  Vainqueur : Team B
 *   Set 3 : 21 - 18  ->  Vainqueur : Team A
 * Vainqueur de la rencontre : Team A
 */
void				PhaseCLI::writeEncounter(std::ostream& out, cvpMatch matches, const size_t startIdx,
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
	out << "  " << STRING(nameA.size() + nameB.size() + 14, '-') << "\n";

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

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 * Affiche la phase complete dans le terminal.
 * Utilise les couleurs ANSI pour les scores et les vainqueurs.
 */
void				PhaseCLI::displayPhase(cPhase phase)
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

/**
 * Affiche uniquement le tableau vainqueurs / perdants d une phase terminee.
 */
void				PhaseCLI::displayResults(cPhase phase)
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

/**
 * Exporte l historique complet d une phase dans un fichier texte.
 * Pas de codes ANSI dans le fichier (toFile = true).
 */
bool				PhaseCLI::exportToTxt(cPhase phase, cString filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
	{
		PrintUtils::addError(std::format("Impossible de creer le fichier : {}", filename));
		return (false);
	}

	cvpMatch matches = phase.getMatches();
	cInt nbSets = phase.getNbSetToPlay();

	file << "============================================================\n";
	file << "  PHASE : " << phase.getName() << "\n";
	file << "  Sets par rencontre : " << nbSets << "\n";
	file << "  Statut : " << (phase.isFinished() ? "Terminée" : "En cours") << "\n";
	file << "============================================================\n";

	if (matches.empty())
	{
		file << "\n  Aucun match enregistré.\n";
		file.close();
		return (true);
	}

	int encounterNum = 1;

	for (size_t i = 0; i < matches.size(); i += static_cast<size_t>(nbSets))
	{
		writeEncounter(file, matches, i, nbSets, encounterNum, true);
		file << "  " << STRING(50, '-') << "\n";
		++encounterNum;
	}

	if (phase.isFinished())
	{
		vpTeam winners = phase.getWinners();
		vpTeam losers = phase.getLosers();

		file << "\n============================================================\n";
		file << "  RÉSULTATS\n";
		file << "============================================================\n";

		file << "  Qualifiés :\n";

		for (size_t i = 0; i < winners.size(); ++i)
		{
			if (!winners[i])
				continue;

			file << "    " << (i + 1) << ". " << winners[i]->getName() << "\n";
		}

		if (!losers.empty())
		{
			file << "  Éliminés :\n";

			for (cpTeam t : losers)
			{
				if (!t)
					continue;

				file << "    - " << t->getName() << "\n";
			}
		}
	}

	file << "\n============================================================\n";
	file.close();

	return (true);
}
