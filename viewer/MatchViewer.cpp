//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iomanip>
#include <iostream>

#include "../includes/viewer/MatchViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/TablePrinter.hpp"

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Tournament.hpp"

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

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				MatchViewer::showMatchTitle(cMatch match)
{
	std::cout << match.getTeamA()->getName() << " vs " << match.getTeamB()->getName();

	if (match.isFinished())
	{
		std::cout << " [ SCORE : " << match.getScoreA() << " - " << match.getScoreB() << " ]";

		if (match.getWinner())
			std::cout << " -> Winner : " << match.getWinner()->getName();
	}
	else
		std::cout << " [ MATCH TO PLAY ]";

	std::cout << std::endl;
}

/**
 * Affiche un tableau de tout les matches avec details
 * id | match | score team 1 | score team 2 | fini
 */
void				MatchViewer::showExtendedTableOfAllMatchesInPool(cvpMatch matches)
{
	TablePrinter table;

	table.setHeaders({"ID", "Rencontre", "ST 1", "ST 2", "Fini"});

	int i = 0;
	for (cpMatch m : matches)
	{
		vString rowData = {
			std::to_string(i++),
			std::format("{:<20.20} vs {:>20.20}", m->getTeamA()->getName(), m->getTeamB()->getName()),
			m->isFinished() ? std::to_string(m->getScoreA()) : "-",
			m->isFinished() ? std::to_string(m->getScoreB()) : "-",
			m->isFinished() ? "Oui" : "Non"
		};

		table.addRow(rowData);
	}

	table.printTable(std::cout);
}

/**
 * Affiche l'etat d'avancement des rencontres dans une poule
 */
void				MatchViewer::showDetailsTableOfAllMatchesInPool(cPool pool)
{
	TablePrinter table;

	PrintUtils::printTitle(std::format("MATCHES OF POOL: {}", pool.getName()));

	table.setHeaders({"Rencontre", "Resultat", "Termine"});

	for (const auto& match : pool.getMatches())
	{
		vString rowData = {
			std::format("{} vs {}", match->getTeamA()->getName(), match->getTeamB()->getName()),
			match->isFinished() ? std::format("{} - {}", match->getScoreA(), match->getScoreB()) : "- : -",
			match->isFinished() ? "Oui" : "Non"
		};
		
		table.addRow(rowData);
	}

	table.printTable(std::cout);
}

/**
 * Affiche liste des matchs avec le status
 */
void				MatchViewer::showAllMatchesWithStatusInPool(cPool pool)
{
	PrintUtils::printTitle(std::format("MATCHES OF {}", pool.getName()));
	PrintUtils::writeMatchesList(std::cout, pool.getMatches(), false);
	PrintUtils::printSeparator();
}

/**
 * TESTER FUNCTION - TO REMOVED or DELETED
 */
void				MatchViewer::printAll(Tournament& tournament)
{
	PrintUtils::printTitle("MatchViewer");

	auto pools = tournament.getPools();

	if (!pools.empty() && pools[0])
	{
		auto matches = pools[0]->getMatches();

		if (!matches.empty() && matches[0])
			showMatchTitle(*matches[0]);

		showExtendedTableOfAllMatchesInPool(matches);
		showDetailsTableOfAllMatchesInPool(*pools[0]);
		showAllMatchesWithStatusInPool(*pools[0]);
	}
}
