//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <vector>
#include <format>
#include <limits>
#include <iostream>
#include <exception>

#include "../includes/class/Team.hpp"
#include "../includes/class/Pool.hpp"
# include "../includes/class/Match.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/viewer/PoolViewer.hpp"
#include "../includes/viewer/MatchViewer.hpp"
#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/cli/CLIUtils.hpp"
#include "../includes/cli/MatchCLI.hpp"

#include "../includes/Constantes.hpp"

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

void				MatchCLI::displayMenuUI(cvpMatch matches)
{
	CLIUtils::handleTitle(TitleViewer::matches);
	PrintUtils::handleMessages();
	MatchViewer::displayAllMatches(matches);
	std::cout << "Selectionnez un matche en entrant son id (tapez 'r' pour revenir au menu precedent): ";
	CLIUtils::checkInterruption();
}

void				MatchCLI::menuMatch(pMatch match)
{
	std::vector<MenuItem> items;

	items.push_back({'1', "Enregistrer un score"});
	items.push_back({'2', "Modifier le score"});
	items.push_back({'R', "Retour au menu precedent"});
	CLIUtils::displayMenu(std::format("{}\tVs\t{}", match->getTeamA()->getName(), match->getTeamB()->getName()), items);
}

void				MatchCLI::submenuMatch(pMatch match)
{
	try
	{
		while (true)
		{
			menuMatch(match);

			String input = CLIUtils::input();
			
			if (input.empty())
				continue;

			if (input == "r" || input == "R")
				return;

			auto choice = CLIUtils::parseInt(input);
			
			if (choice.has_value())
			{
				executeChoice(choice.value(), match);
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

void				MatchCLI::executeChoice(cInt choice, pMatch match)
{
	switch (choice)
	{
		case 1:
			//	enregistrer un score
			handleSaveScore(match);
			break;

		case 2:
			//	modifier un score
			handleModifyScore(match);
			break;
		
		default:
			PrintUtils::addError("Choix non disponible.");
			break;
	}
}

/********************/
/*  HANDLER ACTION	*/
/********************/

void				MatchCLI::handleSaveScore(pMatch match)
{
	int sA = CLIUtils::askInt(std::format("Score de {}", match->getTeamA()->getName()), 0, SCOREMAXTOWIN, 0);
	int sB = CLIUtils::askInt(std::format("Score de {}", match->getTeamB()->getName()), 0, SCOREMAXTOWIN, 0);

	if (match->setScore(sA, sB))
		PrintUtils::addSuccess("Score enregistre !");
	else
		PrintUtils::addError(std::format("Les scores ne sont pas valide TeamA: {} - TeamB: {}.", sA, sB));
}

void				MatchCLI::handleModifyScore(pMatch match)
{
	int sA = CLIUtils::askInt(std::format("Score actuel de {} => {}", match->getTeamA()->getName(), match->getScoreA()), 0, SCOREMAXTOWIN, match->getScoreA());
	int sB = CLIUtils::askInt(std::format("Score actuel de {} => {}", match->getTeamB()->getName(), match->getScoreB()), 0, SCOREMAXTOWIN, match->getScoreB());

	match->setScore(sA, sB);
	PrintUtils::addSuccess("Score modifie !");
}

/********************/
/*  HELPER			*/
/********************/

bool				MatchCLI::checkMatchId(int id, size_t size)
{
	return (id < static_cast<int>(size));
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/



void				MatchCLI::handleMenuMatch(cvpMatch matches)
{
	try
	{
		while (true)
		{
			displayMenuUI(matches);

			String input = CLIUtils::input();
			
			if (input.empty())
				continue;

			if (input == "r" || input == "R")
				return;

			auto choice = CLIUtils::parseInt(input);

			if (choice.has_value() && !checkMatchId(choice.value(), matches.size()))
			{
				PrintUtils::addError(std::format("l'id {} n'existe pas.", choice.value()));
				continue;
			}

			if (!matches.empty() && choice.has_value())
				submenuMatch(matches[choice.value() - 1]);
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}