//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <vector>
#include <format>

#include "../includes/cli/PoolCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"
#include "../includes/cli/CLIUtils.hpp"

#include "../includes/class/Pool.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/utils/PrintUtils.hpp"

#include "../includes/viewer/PoolViewer.hpp"
#include "../includes/viewer/TitleViewer.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cString			=	const std::string&;

using				cInt			=	const int;

using				pPool			=	Pool*;
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

/**
 *	Gestion de l affichage du menu principale
 */
void				PoolCLI::displayMenuUI(cTour tournament)
{
	CLIUtils::handleTitle(TitleViewer::pools);
	PrintUtils::handleMessages();
	PoolViewer::showPoolsListWithStatus(tournament);
	CLIUtils::checkInterruption();
}

/**
 *	Verification d un id
 */
bool				PoolCLI::checkPoolId(cInt id, Tournament& tournament)
{
	return (id >= 1 && id <= static_cast<int>(tournament.getPools().size()));
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 *	Menu principale du menu Pool
 */
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

			cString input = CLIUtils::askString("Selectionnez une pool en entrant son id (tapez 'r' pour revenir au menu precedent):", "R");
			
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
					MatchCLI::handleMenuMatch(selectedPool->getMatches(), std::format("POOL {}", selectedPool->getName()));
				}
			}
			else
				PrintUtils::addError("Saisie invalide.");
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}