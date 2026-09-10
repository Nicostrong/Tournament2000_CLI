//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <string>
#include <vector>
#include <format>
# include <functional>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
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

#include "../includes/utils/Exporter.hpp"
#include "../includes/utils/PrintUtils.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;

using				cInt			=	const int;
using				vInt			=	std::vector<int>;
using				cvInt			=	const std::vector<int>&;

using				cBool			=	const bool;

using				pMatch			=	Match*;
using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

using				pPhase			=	Phase*;
using				cPhase			=	const Phase&;
using				cpPhase			=	const Phase*;
using				vpPhase			=	std::vector<Phase*>;
using				cvpPhase		=	const std::vector<Phase*>&;

using				pTour			=	Tournament*;
using				cTour			=	const Tournament&;
using				cpTour			=	const Tournament*;

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
vMenuItem			TournamentCLI::displayMenuUI(cTour tournament)
{
	CLIUtils::handleTitle(TitleViewer::tournament);
	PrintUtils::handleMessages();
	vMenuItem menu = menuTournament(tournament);
	CLIUtils::checkInterruption();
	return (menu);
}

/**
 *	Affiche les menus sous conditions du tournoi
 */
vMenuItem			TournamentCLI::menuTournament(cTour tournament)
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
	
	CLIUtils::displayMenu(std::format("TOURNOI : {}", tournament.getSettings().getName()), menuLst);
	return (menuLst);
}


/********************/
/*  HANDLER SAISIE	*/
/********************/

/**
 *	Appel la bonne methode d apres le choix de l utilisateur
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
 * Factorise le pattern commun à toutes les phases eliminatoires :
 *   - si phase == nullptr → appelle generateFn(), affiche succes ou erreur
 *   - sinon → affiche les matchs via handlePhase()
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

/**
 * Gere le flux complet de l export d une phase
 * Demande quelle phase, quel fichier, puis exporte.
 */
/*void				TournamentCLI::handleExport(Tournament& tournament)
{
	while (true)
	{
		CLIUtils::handleTitle(TitleViewer::exportMenu);
		std::cout << Color::YELLOW << "\t1.\t" << Color::RESET << "Players\n";
		std::cout << Color::YELLOW << "\t2.\t" << Color::RESET << "Teams\n";
		std::cout << Color::YELLOW << "\t3.\t" << Color::RESET << "Pools\n";

		int menuIdx = 4;
		int sixteenthIdx = -1;
		int heighthIdx = -1;
		int quarterIdx = -1;
		int semiIdx = -1;
		int finalIdx = -1;
		int thirdIdx = -1;

		if (tournament.getSixteenth() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "1/16" << std::endl;
			sixteenthIdx = menuIdx++;
		}

		if (tournament.getEighth() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "1/8" << std::endl;
			heighthIdx = menuIdx++;
		}

		if (tournament.getQuarters() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "1/4" << std::endl;
			quarterIdx = menuIdx++;
		}

		if (tournament.getSemis() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "1/2" << std::endl;
			semiIdx = menuIdx++;
		}

		if (tournament.getFinal() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "Final" << std::endl;
			finalIdx = menuIdx++;
		}

		if (tournament.getThirdPlace() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "3 place" << std::endl;
			thirdIdx = menuIdx++;
		}

		int fullIdx = menuIdx++;

		std::cout << "────────────────────────────────────────────────────────────\n";
		std::cout << std::format("  {}. Tournoi Complet\n", fullIdx);
		std::cout << Color::YELLOW << "\t0.\t" << Color::RESET << "Quitter" <<std::endl;
		std::cout << "=============================================================\n";
		std::cout << "\tChoix :\t";

		String rawInput = CLIUtils::input();

		if (rawInput.empty())
			continue;
		
		auto parsed = CLIUtils::parseInt(rawInput);
		int choice = parsed.value();

		if (choice == 0)
			return;

		CLIUtils::clearInput();

		const String filename = CLIUtils::askString("Nom du fichier de sortie : ", "");

		if (filename.empty())
			continue;

		bool ok = false;

		if (choice == 1)
		{
			std::ofstream out(filename);

			if (out.is_open())
			{
				out << "========== LISTE DES PARTICIPANTS ==========\n";
				for (const Pool* p : tournament.getPools())
				{
					if (!p)
						continue;

					for (const Team* t : p->getTeams())
					{
						if (!t)
							continue;

						out << "Equipe : " << t->getName() << "\n";
					}
				}
				out.close();
				ok = true;
			}
		} 
		else if (choice == 2)
		{
			std::ofstream out(filename);

			if (out.is_open())
			{
				out << "========== LISTE DES EQUIPES ==========\n";

				for (const Pool* p : tournament.getPools())
				{
					if (!p)
						continue;

					out << "\n[" << p->getName() << "]\n";

					for (const Team* t : p->getTeams())
						if (t)
							out << "  - " << t->getName() << "\n";
				}
				out.close();
				ok = true;
			}
		} 
		else if (choice == 3)
			ok = Exporter::exportPoolsToTxt(tournament, filename);
		else if (choice == sixteenthIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getSixteenth(), filename);
		else if (choice == heighthIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getEighth(), filename);
		else if (choice == quarterIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getQuarters(), filename);
		else if (choice == semiIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getSemis(), filename);
		else if (choice == finalIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getFinal(), filename);
		else if (choice == thirdIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getThirdPlace(), filename);
		else if (choice == fullIdx)
			ok = Exporter::exportTournamentToTxt(tournament, filename);
		else
		{
			PrintUtils::addError("Choix non valide.");
			continue;
		}

		if (ok)
		{
			PrintUtils::addSuccess(std::format("Export reussi dans {}.", filename));
			break;
		}
		
		PrintUtils::addError("Echec de l'export.");
	}
}*/

/**
 * Mappe un entier vers la Phase* correspondante.
 * Retourne nullptr si le choix est hors-plage ou la phase non generee.
 *
 * 2→1/16  3→1/8  4→Quarts  5→Demis  6→Finale  7→3e Place
 */
cpPhase				TournamentCLI::getPhaseByMenuChoice(cTour tournament, cInt choice)
{
	switch (choice)
	{
		case 2:
			return (tournament.getSixteenth());

		case 3:
			return (tournament.getEighth());

		case 4:
			return (tournament.getQuarters());

		case 5:
			return (tournament.getSemis());

		case 6:
			return (tournament.getFinal());

		case 7:
			return (tournament.getThirdPlace());

		default:
			return (nullptr);
	}
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 *	Gestion du menu Tournament du programme
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
