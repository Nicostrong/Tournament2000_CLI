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
/*	STATIC VARIABLES																				*/
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

/**
 *	Gestion de l affichage des menu
 */
void				ShowCLI::displayMenuUI(cTour tournament)
{
	CLIUtils::handleTitle(TitleViewer::tournament);
	PrintUtils::handleMessages();
	menuShow(tournament);
	CLIUtils::checkInterruption();
}

/**
 *	Affiche les menus sous conditions du tournoi
 */
void				ShowCLI::menuShow(cTour tournament)
{
	std::vector<MenuItem> items =
	{
		{'P', "Print Player"},
		{'T', "Print Team"},
		{'O', "Print Pool"},
		{'M', "Print Match"}
	};

	if (tournament.getHasSixteenth() && tournament.isSixteenthUnlocked())
		items.push_back({'1', "Print 1/16"});

	if (tournament.getHasEighth() && tournament.isEighthUnlocked())
		items.push_back({'2', "Print 1/8"});

	if (tournament.isQuartersUnlocked())
		items.push_back({'3', "Print 1/4"});

	if (tournament.isSemisUnlocked())
		items.push_back({'4', "Print 1/2"});

	if (tournament.isThirdUnlocked())
	{
		items.push_back({'5', "Print Petite finale"});
		items.push_back({'6', "Print Finale"});
	}

	if (tournament.isFinalUnlocked() && !tournament.getHasThirdMatch())
		items.push_back({'5', "Print Finale"});

	items.push_back({'7', "Print Tournament"});
	items.push_back({'8', "Print Settings"});
	items.push_back({'Q', "Quit"});

	CLIUtils::displayMenu(std::format("TOURNOI : {}", tournament.getSettings().getName()), items);
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
			if (tournament.getHasSixteenth() && tournament.isSixteenthUnlocked())
				PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 2:
			if (tournament.getHasEighth() && tournament.isEighthUnlocked())
				PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 3:
			if (tournament.isQuartersUnlocked())
				PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 4:
			if (tournament.isSemisUnlocked())
				PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 5:
			if (tournament.isThirdUnlocked())
				PhaseViewer::printAll(tournament);
			else
				PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 6:
			if (tournament.isFinalUnlocked() && !tournament.getHasThirdMatch())
				PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 7:
			TournamentViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 8:
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
			displayMenuUI(tournament);

			String input = CLIUtils::input();

			if (input.empty())
				continue;

			if (input == "q" || input == "Q")
				return;

			if (input == "p" || input == "P")
			{
				PlayerViewer::printAll(tournament);
				CLIUtils::waitForEnter();
				continue;
			}

			if (input == "t" || input == "T")
			{
				TeamViewer::printAll(tournament);
				CLIUtils::waitForEnter();
				continue;
			}

			if (input == "o" || input == "O")
			{
				PoolViewer::printAll(tournament);
				CLIUtils::waitForEnter();
				continue;
			}

			if (input == "m" || input == "M")
			{
				MatchViewer::printAll(tournament);
				CLIUtils::waitForEnter();
				continue;
			}

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
