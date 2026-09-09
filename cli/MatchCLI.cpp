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
#include "../includes/class/Match.hpp"
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

void				MatchCLI::displayMenuUI(vpMatch matches, cString title)
{
	CLIUtils::handleTitle(TitleViewer::matches);
	PrintUtils::handleMessages();

	if (!title.empty())
		PrintUtils::printTitle(std::format("MATCHES — {}", title));
	else
		PrintUtils::printTitle("MATCHES LISTE");

	MatchViewer::showExtendedTableOfAllMatches(matches);

	std::cout << "Select the ID of match you want to interract with ('r' to return): ";

	CLIUtils::checkInterruption();
}

void				MatchCLI::menuMatch(pMatch match)
{
	std::vector<MenuItem> items;

	if (match->isFinished())
		items.push_back({'1', "Modify score"});
	else
		items.push_back({'1', "Save score"});

	items.push_back({'2', "Show the match"});
	items.push_back({'R', "Return"});

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
			if (match->isFinished())
				handleModifyScore(match);
			else
				handleSaveScore(match);
			break;
		
		case 2:
			MatchViewer::showMatchTitle(*match);
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
	return (id >= 0 && id < static_cast<int>(size));
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				MatchCLI::handleMenuMatch(vpMatch matches, cString title)
{
	if (matches.empty())
		return (PrintUtils::addError("No matches found !"));

	try
	{
		while (true)
		{
			displayMenuUI(matches, title);

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

			if (choice.has_value() && matches[choice.value()])
				submenuMatch(matches[choice.value()]);
			else
				PrintUtils::addError("Saisie invallide.");
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}