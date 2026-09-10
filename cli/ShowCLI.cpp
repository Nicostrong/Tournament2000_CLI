//
// Created by Nicolas Fordoxcel on 04/09/2026.
//
/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <vector>
#include <format>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/cli/PoolCLI.hpp"
#include "../includes/cli/ShowCLI.hpp"
#include "../includes/cli/CLIUtils.hpp"

#include "../includes/viewer/TeamViewer.hpp"
#include "../includes/viewer/PhaseViewer.hpp"
#include "../includes/viewer/PoolViewer.hpp"
#include "../includes/viewer/TitleViewer.hpp"
#include "../includes/viewer/MatchViewer.hpp"
#include "../includes/viewer/PlayerViewer.hpp"
#include "../includes/viewer/SettingsViewer.hpp"
#include "../includes/viewer/TournamentViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				vMenuItem		=	std::vector<MenuItem>;

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

/**
 *	Gestion de l affichage des menu
 */
vMenuItem			ShowCLI::displayMenuUI(cTour tournament)
{
	CLIUtils::handleTitle(TitleViewer::showMenu);
	PrintUtils::handleMessages();
	vMenuItem menu = menuShow(tournament);
	CLIUtils::checkInterruption();
	return (menu);
}

/**
 *	Affiche les menus sous conditions du tournoi
 */
vMenuItem			ShowCLI::menuShow(cTour tournament)
{
	std::vector<MenuItem> items =
	{
		{"1", "Print Player list"},
		{"2", "Print Team"},
		{"3", "Print Pool"},
		{"4", "Print Match"}
	};

	if (tournament.getHasSixteenth() && tournament.isSixteenthUnlocked())
		items.push_back({"5", "Print 1/16"});

	if (tournament.getHasEighth() && tournament.isEighthUnlocked())
		items.push_back({"6", "Print 1/8"});

	if (tournament.isQuartersUnlocked())
		items.push_back({"7", "Print 1/4"});

	if (tournament.isSemisUnlocked())
		items.push_back({"8", "Print 1/2"});

	if (tournament.isThirdUnlocked())
		items.push_back({"9", "Print Petite finale"});

	if (tournament.isFinalUnlocked())
		items.push_back({"10", "Print Finale"});

	items.push_back({"11", "Print Tournament"});
	items.push_back({"12", "Print Settings"});
	items.push_back({"R", "Return"});

	CLIUtils::displayMenu(std::format("TOURNOI : {}", tournament.getSettings().getName()), items);

	return (items);
}


/********************/
/*  HANDLER SAISIE	*/
/********************/

/**
 *	Appel la bonne methode d apres le choix de l utilisateur
 */
void				ShowCLI::executeChoice(cInt choice, Tournament& tournament)
{
	switch (choice)
	{
		case 1:
			PlayerViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 2:
			TeamViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;
		case 3:
			PoolViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 4:
			MatchViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 5:
			PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 6:
			PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 7:
			PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 8:
			PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 9:
			PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 10:
			PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 11:
			TournamentViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 12:
			SettingsViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		default:
			PrintUtils::addError("Choix non disponible.");
			break;
	}
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 *	Gestion du menu Tournament du programme
 */
void				ShowCLI::handleMenuShow(Tournament& tournament)
{
	try
	{
		while (true)
		{
			vMenuItem menu = displayMenuUI(tournament);
			String input = CLIUtils::askMenuChoice(menu);

			if (input.empty())
				continue;

			if (input == "r" || input == "R")
				return;

			auto choice = CLIUtils::parseInt(input);

			if (choice.has_value())
				executeChoice(choice.value(), tournament);
			else
				PrintUtils::addError("Saisie invalide.");
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}
