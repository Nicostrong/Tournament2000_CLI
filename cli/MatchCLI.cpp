//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <vector>
#include <format>
#include <iostream>

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

using				cInt			=	const int;

using				pMatch			=	Match*;
using				cvpMatch		=	const std::vector<Match*>&;

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

void				MatchCLI::displayMenuUI(vpMatch matches, pPool pool)
{
	CLIUtils::handleTitle(TitleViewer::matches);
	PrintUtils::handleMessages();

	pool->sortTeams();

	PoolViewer::displayFullTable(*pool);
	MatchViewer::displayAllMatches(matches);
	std::cout << "Selectionnez un matche en entrant son id (tapez 'r' pour revenir au menu precedent): ";
	CLIUtils::checkInterruption();
}

void				MatchCLI::menuMatch(pMatch match)
{
	std::vector<MenuItem> items;

	if (match->isFinished())
		items.push_back({'1', "Modifier le score"});
	else
		items.push_back({'1', "Enregistrer un score"});

	items.push_back({'2', "Afficher la rencontre"});
	items.push_back({'R', "Retour au menu precedent"});
	CLIUtils::displayMenu(std::format("{}\tVs\t{}", match->getTeamA()->getName(), match->getTeamB()->getName()), items);
}

void				MatchCLI::submenuMatch(pMatch match, pPool pool)
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
				executeChoice(choice.value(), match, pool);
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

void				MatchCLI::executeChoice(cInt choice, pMatch match, pPool pool)
{
	switch (choice)
	{
		case 1:
			if (match->isFinished())
				handleModifyScore(match);
			else
				handleSaveScore(match);
			break;
		
		case 2:
			PoolViewer::displayMatches(*pool);
			CLIUtils::waitForEnter();
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
	cInt sA = CLIUtils::askInt(std::format("Score de {}", match->getTeamA()->getName()), 0, SCOREMAXTOWIN, 0);
	cInt sB = CLIUtils::askInt(std::format("Score de {}", match->getTeamB()->getName()), 0, SCOREMAXTOWIN, 0);

	if (match->setScore(sA, sB))
		PrintUtils::addSuccess("Score enregistre !");
	else
		PrintUtils::addError(std::format("Les scores ne sont pas valide TeamA: {} - TeamB: {}.", sA, sB));
}

void				MatchCLI::handleModifyScore(pMatch match)
{
	cInt sA = CLIUtils::askInt(std::format("Score actuel de {} => {}", match->getTeamA()->getName(), match->getScoreA()), 0, SCOREMAXTOWIN, match->getScoreA());
	cInt sB = CLIUtils::askInt(std::format("Score actuel de {} => {}", match->getTeamB()->getName(), match->getScoreB()), 0, SCOREMAXTOWIN, match->getScoreB());

	if(match->setScore(sA, sB))
		PrintUtils::addSuccess("Score modifie !");
	else
		PrintUtils::addError(std::format("Les scores ne sont pas valide TeamA: {} - TeamB: {}.", sA, sB));
}

/********************/
/*  HELPER			*/
/********************/

bool				MatchCLI::checkMatchId(int id, size_t size)
{
	return (id < static_cast<int>(size) && id >= 0);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/



void				MatchCLI::handleMenuMatch(vpMatch matches, pPool pool)
{
	try
	{
		while (true)
		{
			displayMenuUI(matches, pool);

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
				submenuMatch(matches[choice.value()], pool);
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}