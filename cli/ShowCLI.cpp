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

/**
 *	Gestion de l affichage du menu
 */
vMenuItem			ShowCLI::displayMenuUI(cTour tournament)
{
	vMenuItem menu = generateMenuShow(tournament);

	CLIUtils::handleTitle(TitleViewer::showMenu);
	PrintUtils::handleMessages();
	CLIUtils::checkInterruption();

	return (menu);
}

/**
 *	Creation dynamique du menu
 */
vMenuItem			ShowCLI::generateMenuShow(cTour tournament)
{
	std::vector<MenuItem> menuLst =
	{
		{"1", "Print Player list"},
		{"2", "Print Team"},
		{"3", "Print Pool"},
		{"4", "Print Match"}
	};

	if (tournament.getHasSixteenth() && tournament.isSixteenthUnlocked())
		menuLst.push_back({"5", "Print 1/16"});

	if (tournament.getHasEighth() && tournament.isEighthUnlocked())
		menuLst.push_back({"6", "Print 1/8"});

	if (tournament.isQuartersUnlocked())
		menuLst.push_back({"7", "Print 1/4"});

	if (tournament.isSemisUnlocked())
		menuLst.push_back({"8", "Print 1/2"});

	if (tournament.isThirdUnlocked())
		menuLst.push_back({"9", "Print Petite finale"});

	if (tournament.isFinalUnlocked())
		menuLst.push_back({"10", "Print Finale"});

	menuLst.push_back({"11", "Print Tournament"});
	menuLst.push_back({"12", "Print Settings"});
	menuLst.push_back({"R", "Return"});

	return (menuLst);
}

/**
 *	Aiguillage du menu
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
 *	Menu principale du menu Show
 */
void				ShowCLI::handleMenuShow(Tournament& tournament)
{
	try
	{
		while (true)
		{
			vMenuItem menu = displayMenuUI(tournament);

			CLIUtils::displayMenu(menu);

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
