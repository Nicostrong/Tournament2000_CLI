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
		{"1", "Player list"},
		{"2", "Team"},
		{"3", "Pool"}
	};

	if (tournament.getHasSixteenth() && tournament.isSixteenthUnlocked())
		menuLst.push_back({"4", "1/16"});

	if (tournament.getHasEighth() && tournament.isEighthUnlocked())
		menuLst.push_back({"5", "1/8"});

	if (tournament.isQuartersUnlocked())
		menuLst.push_back({"6", "1/4"});

	if (tournament.isSemisUnlocked())
		menuLst.push_back({"7", "1/2"});

	if (tournament.isThirdUnlocked())
		menuLst.push_back({"8", "Petite finale"});

	if (tournament.isFinalUnlocked())
		menuLst.push_back({"9", "Finale"});

	menuLst.push_back({"10", "Tournament"});
	menuLst.push_back({"11", "Podium"});
	menuLst.push_back({"12", "Settings"});
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
			PlayerViewer::showFullTableOfPlayers(tournament.getPlayers());
			CLIUtils::waitForEnter();
			break;

		case 2:
			TeamViewer::showTeamsTableDetails(tournament.getTeams());
			CLIUtils::waitForEnter();
			break;

		case 3:
			PoolViewer::showFullSummaryPoolStanding(tournament);
			CLIUtils::waitForEnter();
			break;

	case 4:
			PhaseViewer::displayPhase(*(tournament.getSixteenth()));
			CLIUtils::waitForEnter();
			break;

		case 5:
			PhaseViewer::displayPhase(*(tournament.getEighth()));
			CLIUtils::waitForEnter();
			break;

		case 6:
			PhaseViewer::displayPhase(*(tournament.getQuarters()));
			CLIUtils::waitForEnter();
			break;

		case 7:
			PhaseViewer::displayPhase(*(tournament.getSemis()));
			CLIUtils::waitForEnter();
			break;

		case 8:
			PhaseViewer::displayPhase(*(tournament.getThirdPlace()));
			CLIUtils::waitForEnter();
			break;

		case 9:
			PhaseViewer::displayPhase(*(tournament.getFinal()));
			CLIUtils::waitForEnter();
			break;

		case 10:
			TournamentViewer::displayFullBracket(tournament);
			CLIUtils::waitForEnter();
			break;

		case 11:
			TournamentViewer::displayPodium(tournament);
			CLIUtils::waitForEnter();
			break;

		case 12:
			SettingsViewer::showSettings(tournament.getSettings());
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
