//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iostream>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Player.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/viewer/TeamViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/TablePrinter.hpp"

#include "../includes/Color.hpp"

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
 * Affiche la carte de la team avec id, nom et membres
 */
void				TeamViewer::showTeamCard(cTeam team)
{
	PrintUtils::printTitle("TEAM Card");
	std::cout << "Team ID: " << team.getId() << std::endl;
	std::cout << "Nom: " << team.getName() << std::endl;

	if (team.getHasMultiTeamPlayer())
		std::cout << " [Contient un Multi-Joueur]" << std::endl;

	std::cout << "Membres :\n\t";

	for (size_t i = 0; i < team.getMembers().size(); ++i)
	{
		std::cout << team.getMembers()[i]->getPseudo();

		if (team.getMembers()[i]->getIsMultiTeamPlayer())
			std::cout << " (Multi)";

		if (i < team.getMembers().size() - 1)
			std::cout << "\t&\t";
	}
	
	std::cout << std::endl;
}

/**
 * Affiche un tableau detaille de toute les teams
 * id | teamName | pts | + | - | diff | mixte | eli. | disq. | multi
 */
void				TeamViewer::showTeamsTableDetails(vpTeam teams)
{
	if (teams.empty())
		return (PrintUtils::addError("No team."));

	TablePrinter table;

	table.setHeaders({"ID", "Equipe", "Pts", "Pts +", "Pts -", "Diff", "Mixte", "Eli.", "Dis.", "Multi"});
	int i = 0;

	for (const Team* team : teams)
	{
		if (!team)
			continue;

		vString rowData = {
			std::to_string(team->getId()),
			team->getName(),
			std::to_string(team->getPoint()),
			std::to_string(team->getScoreMarked()),
			std::to_string(team->getScoreAgainst()),
			std::to_string(team->getScoreDiff()),
			team->getIsMixed() ? "Oui" : "Non",
			team->getIsEliminated() ? "Oui" : "Non",
			team->getIsDisqualified() ? "Oui" : "Non",
			team->getHasMultiTeamPlayer() ? "Oui" : "Non"
		};
		cString color = (i++ < 2) ? Color::BGREEN : Color::RESET;

		table.addRow(rowData, color);
	}

	table.printTable(std::cout);
}

/**
 * Affiche la liste des nom des equipes dans une poule
 */
void				TeamViewer::showListOfTeamsInPool(cPool pool)
{
	PrintUtils::printTitle(std::format("TEAMS IN POOL: {}", pool.getName()));

	for (const auto& team : pool.getTeams())
		std::cout << std::format("- {}\n", team->getName());
}

/**
 * Affiche la composition detaillee des equipes
 */
void				TeamViewer::showAllTemasCardInPool(cPool pool)
{
	PrintUtils::printSeparator();
	PrintUtils::printTitle(std::format("COMPOSITION DES EQUIPES - {}", pool.getName()));
	PrintUtils::printSeparator();

	for (cpTeam t : pool.getTeams())
		TeamViewer::showTeamCard(*t);
}

/**
 * TESTER FUNCTION - TO REMOVED or DELETED
 */
void				TeamViewer::printAll(Tournament& tournament)
{
	PrintUtils::printTitle("TeamViewer");

	auto teams = tournament.getTeams();

	if (!teams.empty() && teams[0])
		showTeamCard(*teams[0]);

	showTeamsTableDetails(teams);

	auto pools = tournament.getPools();

	if (!pools.empty() && pools[0])
	{
		showListOfTeamsInPool(*pools[0]);
		showAllTemasCardInPool(*pools[0]);
	}
}