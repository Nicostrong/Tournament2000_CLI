//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <string>
#include <vector>
#include <format>
#include <functional>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/cli/PoolCLI.hpp"
#include "../includes/cli/TeamCLI.hpp"
#include "../includes/cli/ShowCLI.hpp"
#include "../includes/cli/CLIUtils.hpp"
#include "../includes/cli/PhaseCLI.hpp"
#include "../includes/cli/ExportCLI.hpp"
#include "../includes/cli/TournamentCLI.hpp"

#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				pPhase			=	Phase*;

using				cTour			=	const Tournament&;

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
vMenuItem			TournamentCLI::displayMenuUI(cTour tournament)
{
	vMenuItem menu = generateMenuTournament(tournament);

	CLIUtils::handleTitle(TitleViewer::tournament);
	PrintUtils::handleMessages();
	CLIUtils::displayMenu(menu);
	CLIUtils::checkInterruption();

	return (menu);
}

/**
 *	Creation dynamique du menu
 */
vMenuItem			TournamentCLI::generateMenuTournament(cTour tournament)
{
	std::vector<MenuItem> menuLst =
	{
		{"1", "Teams"},
		{"2", "Pool"},
	};

	if (tournament.getHasSixteenth() && tournament.isSixteenthUnlocked() && tournament.getSixteenth())
		menuLst.push_back({"3", "Manage 1/16"});
	else if (tournament.getHasSixteenth() && tournament.isSixteenthUnlocked())
		menuLst.push_back({"3", "Generate 1/16"});

	if (tournament.getHasEighth() && tournament.isEighthUnlocked() && tournament.getEighth())
		menuLst.push_back({"4", "Manage 1/8"});
	else if (tournament.getHasEighth() && tournament.isEighthUnlocked())
			menuLst.push_back({"4", "Generate 1/8"});

	if (tournament.isQuartersUnlocked() && tournament.getQuarters())
		menuLst.push_back({"5", "Manage 1/4"});
	else if (tournament.isQuartersUnlocked())
		menuLst.push_back({"5", "Generate 1/4"});


	if (tournament.isSemisUnlocked() && tournament.getSemis())
		menuLst.push_back({"6", "Manage 1/2"});
	else if (tournament.isSemisUnlocked())
		menuLst.push_back({"6", "Generate 1/2"});

	if (tournament.isThirdUnlocked() && tournament.getThirdPlace())
		menuLst.push_back({"7", "Manage Petite finale"});
	else if (tournament.isThirdUnlocked() && !tournament.getThirdPlace())
		menuLst.push_back({"7", "Generate Petite finale"});

	if (tournament.isFinalUnlocked() && tournament.getFinal())
		menuLst.push_back({"8", "Manage Finale"});
	else if (tournament.isFinalUnlocked())
		menuLst.push_back({"8", "Generate Finale"});

	menuLst.push_back({"9", "Export"});
	menuLst.push_back({"10", "Show"});
	menuLst.push_back({"Q", "Quit"});

	return (menuLst);
}

/**
 *	Aiguillage du menu
 */
void				TournamentCLI::executeChoice(cInt choice, Tournament& tournament)
{
	switch (choice)
	{
		case 1:		//	TEAM
			TeamCLI::handleMenuTeam(tournament);
			break;

		case 2:		//	POOL
			PoolCLI::handleMenuPool(tournament);
			break;

		case 3:		//	1/16
			handleEliminationPhase(tournament.getSixteenth(),
			[&tournament]() { return (tournament.generateSixteenths()); },
			"1/16 DE FINALE",
			"1/16 generes avec succes !",
			"Impossible : verifiez que les poules soient terminees.");
			break;

		case 4:		//	1/8
			handleEliminationPhase(tournament.getEighth(),
			[&tournament]() { return (tournament.generateEighths()); },
			"1/8 DE FINALE",
			"1/8 generes avec succes !",
			"Impossible : verifiez que la phase precedente soit terminee.");
			break;

		case 5:		//	1/4
			handleEliminationPhase(tournament.getQuarters(),
			[&tournament]() { return (tournament.generateQuarters()); },
			"QUARTS DE FINALE",
			"Quarts de finale generes avec succes !",
			"Impossible : verifiez que les poules/phases precedentes soient terminees.");
			break;

		case 6:		//	1/2
			handleEliminationPhase(tournament.getSemis(),
			[&tournament]() { return (tournament.generateSemis()); },
			"DEMI-FINALE",
			"Demi-finale generees avec succes !",
			"Impossible : verifiez que les quarts soient termines.");
			break;

		case 7:		//	petite finale
			handleEliminationPhase(tournament.getThirdPlace(),
			[&tournament]() { return (tournament.generateThirdPlace()); },
			"PETITE FINALE",
			"Matche de la petite finale generee avec succes !",
			"Impossible : verifiez que les demi-finales soient terminees.");
			break;
		
		case 8:		//	finale
			handleEliminationPhase(tournament.getFinal(),
			[&tournament]() { return (tournament.generateFinal()); },
			"FINALE",
			"Finale generee avec succes !",
			"Impossible : verifiez que les demi-finales soient terminees.");
			break;

		case 9:
			ExportCLI::handleMenuExport(tournament);
			break;

		case 10:
			ShowCLI::handleMenuShow(tournament);
			break;

		default:
			PrintUtils::addError("Choix non disponible.");
			break;
	}
}

/**
 *	Si une phase n est pas generee, on appel la methode passe en parametre pour la generer
 */
void				TournamentCLI::handleEliminationPhase(pPhase phase,	std::function<bool()> generateFn,
						cString phaseName, cString successMsg, cString errorMsg)
{
	if (!phase)
	{
		if (generateFn())
			PrintUtils::addSuccess(successMsg);
		else
			PrintUtils::addError(errorMsg);
	}
	else
		PhaseCLI::handleMenuPhase(phase, phaseName);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 *	Menu principale du programme
 */
void				TournamentCLI::handleMenuTournament(Tournament& tournament)
{
	try
	{
		while (true)
		{
			vMenuItem menu =  displayMenuUI(tournament);
			String input = CLIUtils::askMenuChoice(menu);
			
			if (input.empty())
				continue;

			if (input == "q" || input == "Q")
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
