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

#include "../includes/cli/PoolCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"
#include "../includes/cli/CLIUtils.hpp"

#include "../includes/class/Pool.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/utils/PrintUtils.hpp"

#include "../includes/viewer/PoolViewer.hpp"
#include "../includes/viewer/TeamViewer.hpp"
#include "../includes/viewer/MatchViewer.hpp"
#include "../includes/viewer/TitleViewer.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cString			=	const std::string&;

using				cInt			=	const int;

using				pPool			=	Pool*;
using				cPool			=	const Pool&;
using				cvpPool			=	const std::vector<Pool*>&;

using				cTour			=	const Tournament&;

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
	PoolViewer::showPoolsListWithStatus(tournament);
	std::cout << "Selectionnez une pool en entrant son id (tapez 'r' pour revenir au menu precedent): ";
	CLIUtils::checkInterruption();
}

/****************/
/*  EXECUTION	*/
/****************/

/********************/
/*  HANDLER ACTION	*/
/********************/

/********************/
/*  HELPER			*/
/********************/

bool				PoolCLI::checkPoolId(cInt id, Tournament& tournament)
{
	return (id >= 1 && id <= static_cast<int>(tournament.getPools().size()));
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
			cvpPool pools = tournament.getPools();

			if (pools.empty())
				return (PrintUtils::addError("Probleme de generation de pool."));

			displayMenuUI(tournament);

			for (auto pool: pools)
				if (pool->allMatchesFinished())
					PoolViewer::showDetailsPoolStanding(*pool);

			cString input = CLIUtils::input();
			
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
			
			if (!pools.empty() && choice.has_value())
			{
				pPool selectedPool = pools[choice.value() - 1];

				if (selectedPool)
				{
					selectedPool->sortTeams();
					MatchCLI::handleMenuMatch(selectedPool->getMatches(), std::format("POULE {}", selectedPool->getName()));
				}
			}
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}