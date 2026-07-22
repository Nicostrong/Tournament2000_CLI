//
// Created by Nicolas Fordoxcel on 22/07/2026.
//

//	STDLIB
#include <iostream>
#include <fstream>
#include <iomanip>

//	INCLUDES
#include "../includes/cli/PhaseCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VP_MATCH	=	std::vector<Match*>;
using				VP_TEAM		=	std::vector<Team*>;
using				CVP_MATCH	=	const std::vector<Match*>&;

/********************/
/*	PRIVATE METHOD	*/
/********************/

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
void				PhaseCLI::writeEncounter(std::ostream& out, CVP_MATCH matches,
												const size_t startIdx, const int nbSets,
												const int encounterNum, const bool toFile)
{
	if (startIdx >= matches.size())
		return ;

	const Match*	first = matches[startIdx];
	C_STRING		nameA = first->getTeamA() ? first->getTeamA()->getName() : "?";
	C_STRING		nameB = first->getTeamB() ? first->getTeamB()->getName() : "?";

	out << "\n";
	out << "  Rencontre " << encounterNum << " :  "
		<< nameA << "  vs  " << nameB << "\n";
	out << "  " << STRING(nameA.size() + nameB.size() + 14, '-') << "\n";

	int		winsA = 0;
	int		winsB = 0;

	for (int s = 0; s < nbSets; ++s)
	{
		const size_t	idx = startIdx + static_cast<size_t>(s);

		if (idx >= matches.size())
			break ;

		const Match*	m = matches[idx];

		out << "    Set " << (s + 1) << " : ";

		if (!m || !m->isFinished())
		{
			out << "[ à jouer ]\n";
			continue ;
		}

		out << std::setw(3) << m->getScoreA()
			<< " - "
			<< std::setw(3) << m->getScoreB();

		const Team*	winner = m->getWinner();

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

/********************/
/*	PUBLIC METHOD	*/
/********************/

/**
 * Affiche la phase complete dans le terminal.
 * Utilise les couleurs ANSI pour les scores et les vainqueurs.
 */
void				PhaseCLI::displayPhase(const Phase& phase)
{
	CVP_MATCH		matches = phase.getMatches();
	const int		nbSets = phase.getNbSetToPlay();

	std::cout << "\n";
	std::cout << "╔══════════════════════════════════════════════════╗\n";
	std::cout << "║  PHASE : " << std::left << std::setw(40) << phase.getName() << "║\n";
	std::cout << "║  Sets par rencontre : " << std::left << std::setw(28) << nbSets << "║\n";
	std::cout << "╚══════════════════════════════════════════════════╝\n";

	if (matches.empty())
	{
		std::cout << "  Aucun match enregistré dans cette phase.\n";
		return ;
	}

	int				encounterNum = 1;

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
void				PhaseCLI::displayResults(const Phase& phase)
{
	if (!phase.isFinished())
	{
		std::cout << "\033[1;33m  [!] Phase non terminée — résultats indisponibles.\033[0m\n";
		return ;
	}

	VP_TEAM			winners	= phase.getWinners();
	VP_TEAM			losers	= phase.getLosers();

	std::cout << "\n  ── Résultats de " << phase.getName() << " ──\n";
	std::cout << "  " << STRING(40, '-') << "\n";

	std::cout << "  Qualifiés :\n";

	for (size_t i = 0; i < winners.size(); ++i)
	{
		if (!winners[i])
			continue ;

		std::cout << "\033[1;32m    " << (i + 1) << ". " << winners[i]->getName() << "\033[0m\n";
	}

	if (!losers.empty())
	{
		std::cout << "  Éliminés :\n";

		for (const Team* t : losers)
		{
			if (!t)
				continue ;

			std::cout << "\033[1;31m    - " << t->getName() << "\033[0m\n";
		}
	}

	std::cout << "  " << STRING(40, '-') << "\n";
}

/**
 * Exporte l historique complet d une phase dans un fichier texte.
 * Pas de codes ANSI dans le fichier (toFile = true).
 */
bool				PhaseCLI::exportToTxt(const Phase& phase, C_STRING filename)
{
	std::ofstream	file(filename);

	if (!file.is_open())
	{
		std::cerr << "\033[1;31m[!] Impossible de créer le fichier : " << filename << "\033[0m\n";
		return (false);
	}

	CVP_MATCH		matches = phase.getMatches();
	const int		nbSets = phase.getNbSetToPlay();

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

	// ── Rencontres ─────────────────────────────────────────────────────────
	int				encounterNum = 1;

	for (size_t i = 0; i < matches.size(); i += static_cast<size_t>(nbSets))
	{
		writeEncounter(file, matches, i, nbSets, encounterNum, true);
		file << "  " << STRING(50, '-') << "\n";
		++encounterNum;
	}

	if (phase.isFinished())
	{
		VP_TEAM		winners = phase.getWinners();
		VP_TEAM		losers = phase.getLosers();

		file << "\n============================================================\n";
		file << "  RÉSULTATS\n";
		file << "============================================================\n";

		file << "  Qualifiés :\n";

		for (size_t i = 0; i < winners.size(); ++i)
		{
			if (!winners[i])
				continue ;

			file << "    " << (i + 1) << ". " << winners[i]->getName() << "\n";
		}

		if (!losers.empty())
		{
			file << "  Éliminés :\n";

			for (const Team* t : losers)
			{
				if (!t)
					continue ;

				file << "    - " << t->getName() << "\n";
			}
		}
	}

	file << "\n============================================================\n";
	file.close();

	return (true);
}
