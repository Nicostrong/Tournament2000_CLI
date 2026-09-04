//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iostream>
#include <algorithm>

#include "../includes/class/Player.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/TablePrinter.hpp"

#include "../includes/viewer/PlayerViewer.hpp"

#include "../includes/Constantes.hpp"
#include "../includes/cli/CLIUtils.hpp"

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

void				PlayerViewer::showPlayerCard(cPlayer p)
{
	const int len = std::max({MAXLENPSEUDO, MAXLENFIRSTNAME, MAXLENLASTNAME});
	cString title = std::format("Player id\t{}", p.getId());

	std::cout << "+--------------------------------------+\n";
	std::cout << "|" << std::format("{:^38}", title) << "|\n";
	std::cout << "+--------------------------------------+\n";
	std::cout << "|  Pseudo       : " << std::format("{:>{}}",p.getPseudo(), len) << " |\n";
	std::cout << "|  Nom          : " << std::format("{:>{}}",p.getLastName(), len) << " |\n";
	std::cout << "|  Prenom       : " << std::format("{:>{}}",p.getFirstName(), len) << " |\n";
	std::cout << "|  Genre        : " << std::format("{:>{}}",p.getGenderStr(), len) << " |\n";
	std::cout << "|  Elimine      : " << std::format("{:^{}}", (p.getIsEliminated() ? "Oui" : "Non"), len) << " |\n";
	std::cout << "|  Multi-equipe : " << std::format("{:^{}}", (p.getIsMultiTeamPlayer() ? "Oui" : "Non"), len) << " |\n";
	std::cout << "+--------------------------------------+\n";
}

void				PlayerViewer::showFullTableOfPlayers(cvpPlayer players)
{
	if (players.empty())
		return (PrintUtils::addError("No player to show."));

	PrintUtils::printTitle(std::format("LIST OF PLAYERS ({})", players.size()));

	TablePrinter table;

	table.setHeaders({
		" ID",
		" Pseudo",
		" Nom",
		" Prenom",
		" Genre",
		" Elimine",
		"Multi"});

	for (cpPlayer p : players)
	{
		if (!p)
			continue;

		vString rowData = {
			std::to_string(p->getId()),
			p->getPseudo(),
			p->getLastName(),
			p->getFirstName(),
			p->getGenderStr(),
			p->getIsEliminated() ? "oui" : "non",
			p->getIsMultiTeamPlayer() ? "Oui" : "Non"};

		table.addRow(rowData);
	}

	table.printTable(std::cout);
}

/**
 * TESTER FUNCTION - TO REMOVED or DELETED
 */
void				PlayerViewer::printAll(Tournament& tournament)
{
	PrintUtils::printTitle("PlayerViewer");

	auto players = tournament.getPlayers();

	if (!players.empty() && players[0])
		showPlayerCard(*players[0]);

	showFullTableOfPlayers(players);
}