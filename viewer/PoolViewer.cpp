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
#include "../includes/class/Tournament.hpp"

#include "../includes/viewer/PoolViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/TablePrinter.hpp"

#include "../includes/Color.hpp"

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
 * Ecrit le tableau de classement SIMPLIFIE de la poule dans le flux out.
 * Rang | Equipe | Pts
 * Les 2 premieres equipes sont mises en evidence.
 */
void				PoolViewer::writeSimpleTable(std::ostream& out, cPool pool, cBool toFile)
{
	cvpTeam teams = pool.getTeams();

	if (teams.empty())
		return (PrintUtils::addError("No teams on this pool."));

	TablePrinter table;

	table.setHeaders({"#", "Equipe", "Pts"});

	for (size_t i = 0; i < teams.size(); ++i)
	{
		cpTeam t = teams[i];

		if (!t)
			continue;

		vString rowData = {
			std::to_string(i + 1),
			t->getName(),
			std::to_string(t->getPoint())
		};
		String color = (i < 2) ? Color::BGREEN : "";

		table.addRow(rowData, color);
	}

	table.printTable(out, toFile);
}

/**
 * Ecrit le tableau de classement COMPLET de la poule dans le flux out.
 * Rang | Equipe | Pts | P+ | P- | Diff
 * Les 2 premieres equipes sont mises en evidence.
 */
void				PoolViewer::writeCompleteTable(std::ostream& out, cPool pool, cBool toFile)
{
	cvpTeam teams = pool.getTeams();

	if (teams.empty())
		return (PrintUtils::addError("No Teams in this pool."));

	TablePrinter table;

	table.setHeaders({"#", "Equipe", "Pts", "P+", "P-", "Diff"});

	for (size_t i = 0; i < teams.size(); ++i)
	{
		cpTeam t = teams[i];

		if (!t)
			continue;

		vString rowData = {
			std::to_string(i + 1),
			t->getName(),
			std::to_string(t->getPoint()),
			std::to_string(t->getScoreMarked()),
			std::to_string(t->getScoreAgainst()),
			std::to_string(t->getScoreDiff())
		};
		cString color = (i < 2) ? Color::BGREEN : "";

		table.addRow(rowData, color);
	}

	table.printTable(out, toFile);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 * Affiche le classement de la pool
 * id | team | points
 */
void				PoolViewer::showPoolStanding(cPool pool)
{
	PrintUtils::printTitle(std::format("CLASSEMENT {}", pool.getName()));
	writeSimpleTable(std::cout, pool, false);
}

/**
 * Affiche le classement detaille de la pool
 * id | team | pts | + | - | diff
 */
void				PoolViewer::showDetailsPoolStanding(cPool pool)
{
	PrintUtils::printTitle(std::format("CLASSEMENT DETAILLE {}", pool.getName()));
	writeCompleteTable(std::cout, pool, false);
}

/**
 * Affiche la liste des poules avec leur status
 * id | poolName | status
 */
void				PoolViewer::showPoolsListWithStatus(cTour tournament)
{
	TablePrinter table;

	table.setHeaders({"ID", "Nom", "Terminee"});

	int i = 1;

	for (cpPool pool : tournament.getPools())
	{
		vString rowData = {
			std::to_string(i++),
			pool->getName(),
			pool->getIsFinished() ? "oui" : "non"
		};

		table.addRow(rowData);
	}

	table.printTable(std::cout);
}

/**
 * TESTER FUNCTION - TO REMOVED or DELETED
 */
void				PoolViewer::printAll(Tournament& tournament)
{
	PrintUtils::printTitle("PoolViewer");

	auto pools = tournament.getPools();

	if (!pools.empty() && pools[0])
	{
		showPoolStanding(*pools[0]);
		showDetailsPoolStanding(*pools[0]);
	}

	showPoolsListWithStatus(tournament);
}