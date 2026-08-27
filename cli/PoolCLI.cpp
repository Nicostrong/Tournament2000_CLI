//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <vector>
#include <format>
#include <limits>
#include <iostream>
#include <exception>

#include "../includes/cli/PoolCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"
#include "../includes/cli/CLIUtils.hpp"

#include "../includes/class/Pool.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/viewer/PoolViewer.hpp"
#include "../includes/viewer/MatchViewer.hpp"
#include "../includes/viewer/TitleViewer.hpp"

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

/************************/
/*  GESTION DU MENU		*/
/************************/

void				PoolCLI::displayMenuUI(cTour tournament)
{
	CLIUtils::handleTitle(TitleViewer::pools);
	PrintUtils::handleMessages();
	PoolViewer::displayPoolsList(tournament);
	std::cout << "Selectionnez une pool en entrant son id (tapez 'r' pour revenir au menu precedent): ";
	CLIUtils::checkInterruption();
}

void				PoolCLI::menuPool(pPool pool)
{
	std::vector<MenuItem> items =
	{
		{'1', "Afficher les matchs de la pool"},
		{'2', "Afficher les matchs a venir"},
		{'3', "Afficher la synthese de la pool"},
		{'R', "Retour au menu precedent"}
	};

	CLIUtils::displayMenu(std::format("{}", pool->getName()), items);
}

void				PoolCLI::submenuPool(pPool pool, Tournament& tournament)
{
	try
	{
		while (true)
		{
			menuPool(pool);

			String input = CLIUtils::input();
			
			if (input.empty())
				continue;

			if (input == "r" || input == "R")
				return;

			auto choice = CLIUtils::parseInt(input);
			
			if (choice.has_value())
			{
				executeChoice(choice.value(), pool, tournament);
				return;
			}
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}

/****************/
/*  EXECUTION	*/
/****************/

void				PoolCLI::executeChoice(cInt choice, pPool pool, Tournament& tournament)
{
	(void)tournament;

	switch (choice)
	{
		case 1:
			MatchCLI::handleMenuMatch(pool->getMatches());
			break;

		case 2:
			//handleModifyTeamMember(team, tournament);
			break;

		case 3:
			//handleDisqualifiedTeam(team);
			break;

		default:
			PrintUtils::addError("Choix non disponible.");
			break;
	}
}

/********************/
/*  HANDLER ACTION	*/
/********************/

/********************/
/*  HELPER			*/
/********************/

bool				PoolCLI::checkPoolId(int id, Tournament& tournament)
{
	return (id >= 1 && id <= static_cast<int>(tournament.getPools().size()));
}

/*******************************************************************************/

void				PoolCLI::manageSinglePool(cPool pool)
{
	int choice = 0;

	while (choice != 4)
	{
		PrintUtils::clear();
		TitleViewer::pools();
		
		std::cout << "\n=== GESTION DE LA " << pool.getName() << " ===\n\n";
		std::cout << "  1. Afficher le classement détaillé\n";
		std::cout << "  2. Afficher les matchs\n";
		std::cout << "  3. Afficher la composition des équipes\n";
		std::cout << "  4. Retour\n\n";
		std::cout << "Votre choix : ";

		if (!(std::cin >> choice))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			choice = 0;
		}

		switch (choice)
		{
			case 1:
				PoolViewer::displayFullTable(pool);
				break;
			case 2:
				PoolViewer::displayMatches(pool);
				break;
			case 3:
				PoolViewer::displayPoolDetails(pool);
				break;
			case 4:
				break;
			default:
				PrintUtils::addError("Choix invalide.");
				break;
		}

		if (choice != 4)
		{
			std::cout << "\nAppuyez sur Entrée pour continuer...";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cin.get();
		}
	}
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				PoolCLI::handleMenuPool(Tournament& tournament)
{
	try
	{
		while (true)
		{
			displayMenuUI(tournament);

			String input = CLIUtils::input();
			
			if (input.empty())
				continue;

			if (input == "r" || input == "R")
				return;

			auto choice = CLIUtils::parseInt(input);

			if (choice.has_value() && !checkPoolId(choice.value(), tournament))
			{
				PrintUtils::addError(std::format("l'id {} n'existe pas.", choice.value()));
				continue;
			}

			cvpPool pool = tournament.getPools();
			
			if (!pool.empty() && choice.has_value())
				submenuPool(pool[choice.value() -1], tournament);
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}