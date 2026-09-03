//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iomanip>
#include <iostream>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/cli/CLIUtils.hpp"

#include "../includes/viewer/PoolViewer.hpp"
#include "../includes/viewer/TeamViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

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
 * Ecrit le tableau de classement SIMPLIFIÉ de la poule dans le flux out.
 * Colonnes : Rang | Equipe | Pts
 * Les 2 premières équipes sont mises en évidence.
 */
void				PoolViewer::writeSimpleTable(std::ostream& out, cPool pool, cBool toFile)
{
	cvpTeam teams = pool.getTeams();

	if (teams.empty())
		return (PrintUtils::addError("Aucune equipe dans cette poule."));

	constexpr int TABLE_WIDTH = 45;
	constexpr int W_RANK  = 3;
	constexpr int W_PTS   = 4;
	
	constexpr int FIXED_CHARS = 1 + W_RANK + W_PTS + (2 * 3) + 1;
	constexpr int W_TEAM = TABLE_WIDTH - FIXED_CHARS;

	std::string separator = "+" + std::string(TABLE_WIDTH, '-') + "+";

	out << separator << "\n";
	out << std::format("| {:^{}} | {:<{}} | {:^{}} |\n",
		"#", W_RANK,
		"Equipe", W_TEAM,
		"Pts", W_PTS);
	out << separator << "\n";

	for (size_t i = 0; i < teams.size(); ++i)
	{
		cpTeam t = teams[i];

		if (!t)
			continue;

		cBool isTop2 = (i < 2);
		std::string teamName = t->getName();

		if (teamName.length() > static_cast<size_t>(W_TEAM))
			teamName = teamName.substr(0, W_TEAM - 3) + "...";

		if (!toFile && isTop2)
			out << "\033[1;32m";

		out << std::format("| {:^{}} | {:<{}} | {:^{}} |\n",
			i + 1, W_RANK,
			teamName, W_TEAM,
			t->getPoint(), W_PTS);

		if (!toFile && isTop2)
			out << "\033[0m";
	}

	out << separator << "\n";
}

/**
 * Ecrit le tableau de classement COMPLET de la poule dans le flux out.
 * Colonnes : Rang | Equipe | Pts | P+ | P- | Diff
 * Les 2 premieres equipes sont mises en evidence.
 */
void				PoolViewer::writeCompleteTable(std::ostream& out, cPool pool, cBool toFile)
{
	cvpTeam teams = pool.getTeams();

	if (teams.empty())
		return (PrintUtils::addError("Aucune equipe dans cette poule."));

	constexpr int TABLE_WIDTH = 80;
	constexpr int W_RANK  = 3;
	constexpr int W_PTS   = 4;
	constexpr int W_PLUS  = 5;
	constexpr int W_MINUS = 5;
	constexpr int W_DIFF  = 6;

	constexpr int FIXED_CHARS = 1 + W_RANK + W_PTS + W_PLUS + W_MINUS + W_DIFF + (5 * 3) + 1;
	constexpr int W_TEAM = TABLE_WIDTH - FIXED_CHARS;

	std::string separator = "+" + std::string(TABLE_WIDTH, '-') + "+";

	out << separator << "\n";
	out << std::format("| {:^{}} | {:<{}} | {:^{}} | {:^{}} | {:^{}} | {:^{}} |\n",
		"#", W_RANK,
		"Equipe", W_TEAM,
		"Pts", W_PTS,
		"P+", W_PLUS,
		"P-", W_MINUS,
		"Diff", W_DIFF);
	out << separator << "\n";

	for (size_t i = 0; i < teams.size(); ++i)
	{
		cpTeam t = teams[i];

		if (!t)
			continue;

		cInt diff = t->getScoreDiff();
		cBool isTop2 = (i < 2);
		std::string teamName = t->getName();

		if (teamName.length() > static_cast<size_t>(W_TEAM))
			teamName = teamName.substr(0, W_TEAM - 3) + "...";

		std::string diffStr = std::format("{}{}", (diff > 0 ? "+" : ""), diff);

		if (!toFile && isTop2)
			out << "\033[1;32m";

		out << std::format("| {:^{}} | {:<{}} | {:^{}} | {:^{}} | {:^{}} | {:^{}} |\n",
			i + 1, W_RANK,
			teamName, W_TEAM,
			t->getPoint(), W_PTS,
			t->getScoreMarked(), W_PLUS,
			t->getScoreAgainst(), W_MINUS,
			diffStr, W_DIFF);

		if (!toFile && isTop2)
			out << "\033[0m";
	}

	out << separator << "\n";
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 * Affiche le classement SIMPLIFIE dans le terminal
 */
void				PoolViewer::displayTable(cPool pool)
{
	CLIUtils::displayTitle(std::format("CLASSEMENT {}", pool.getName()));
	writeSimpleTable(std::cout, pool, false);
}

/**
 * Affiche le classement COMPLET enrichi avec points et difference de score dans le terminal.
 */
void				PoolViewer::displayFullTable(cPool pool)
{
	CLIUtils::displayTitle(std::format("CLASSEMENT DETAILLE {}", pool.getName()));
	writeCompleteTable(std::cout, pool, false);
}

/**
 * Affiche la liste des poules du tournoi
 */
void				PoolViewer::displayPoolsList(cTour tournament)
{
	std::cout << std::string(44, '-') << '\n';
	std::cout << std::format("| {:<4} | {:<20} | {:<10} |\n", "ID", "Nom", "Terminee");
	std::cout << std::string(44, '-') << '\n';

	int i = 1;

	for (cpPool pool : tournament.getPools())
		std::cout << std::format("| {:<4} | {:<20} | {:<10} |\n",
			i++,
			pool->getName(),
			pool->getIsFinished() ? "Oui" : "Non");
	
	std::cout << std::string(44, '-') << "\n\n";
}

/**
 *	TO DO
 *	
 *	liste des methodes:
 *	
 *	displayTable			=>	affiche tableau simple (POOL)
 *	displayFullTable		=>	affiche tableau complet (POOL)
 *	displayMatches			=>	affiche synthese des matches [a jouer || resultat] (MATCH)
 *	displayPoolsList		=>	affiche la liste des pool + etat (POOL)
 *
 *	faire le dispatching dans les differentes classes
 */