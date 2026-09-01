//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iostream>

#include "../includes/class/Team.hpp"
#include "../includes/class/Player.hpp"

#include "../includes/cli/CLIUtils.hpp"

#include "../includes/viewer/TeamViewer.hpp"
#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"

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

void				TeamViewer::showTeamDescription(cTeam team)
{
	CLIUtils::displayTitle("TEAM DESCRIPTION");
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

void				TeamViewer::showAllTeams(vpTeam teams)
{
	if (teams.empty())
		return (PrintUtils::addError("Aucune equipe."));

	int wName = 4;

	for (const Team* team : teams)
		if (team)
			wName = std::max(wName,	static_cast<int>(team->getName().size()));

	constexpr int W_ID      = 2;
	constexpr int W_PTS     = 3;
	constexpr int W_MARKED  = 5;
	constexpr int W_AGAINST = 5;
	constexpr int W_DIFF    = 4;
	constexpr int W_MIXED   = 5;
	constexpr int W_ELIM    = 4;
	constexpr int W_DISQ    = 4;
	constexpr int W_MULTI   = 5;

	const int separatorSize =
		W_ID + 3 +
		wName + 3 +
		W_PTS + 3 +
		W_MARKED + 3 +
		W_AGAINST + 3 +
		W_DIFF + 3 +
		W_MIXED + 3 +
		W_ELIM + 3 +
		W_DISQ + 3 +
		W_MULTI + 2;

	CLIUtils::displayTitle("Tableau des teams", separatorSize + 2);
	std::cout << "+" << std::string(separatorSize, '-') << "+\n";
	std::cout << std::format(
		"| {:>{}} | {:<{}} | {:>{}} | {:>{}} | {:>{}} | {:>{}} | {:<{}} | {:<{}} | {:<{}} | {:<{}} |\n",
		"ID", W_ID,
		"Equipe", wName,
		"Pts", W_PTS,
		"Pts +", W_MARKED,
		"Pts -", W_AGAINST,
		"Diff", W_DIFF,
		"Mixte", W_MIXED,
		"Eli.", W_ELIM,
		"Dis.", W_DISQ,
		"Multi", W_MULTI
	);
	std::cout << "+" << std::string(separatorSize, '-') << "+\n";

	for (const Team* team : teams)
	{
		if (!team)
			continue;

		std::cout << std::format(
			"| {:>{}} | {:<{}} | {:>{}} | {:>{}} | {:>{}} | {:>{}} | {:<{}} | {:<{}} | {:<{}} | {:<{}} |\n",
			team->getId(), W_ID,
			team->getName(), wName,
			team->getPoint(), W_PTS,
			team->getScoreMarked(), W_MARKED,
			team->getScoreAgainst(), W_AGAINST,
			team->getScoreDiff(), W_DIFF,
			team->getIsMixed() ? "Oui" : "Non", W_MIXED,
			team->getIsEliminated() ? "Oui" : "Non", W_ELIM,
			team->getIsDisqualified() ? "Oui" : "Non", W_DISQ,
			team->getHasMultiTeamPlayer() ? "Oui" : "Non", W_MULTI
		);
	}

	std::cout << "+" << std::string(separatorSize, '-') << "+\n";
}