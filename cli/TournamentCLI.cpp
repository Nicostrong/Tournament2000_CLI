//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <vector>
#include <format>
#include <limits>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/cli/CLIUtils.hpp"
#include "../includes/cli/PoolCLI.hpp"
#include "../includes/cli/TeamCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"
#include "../includes/cli/TournamentCLI.hpp"

#include "../includes/viewer/TeamViewer.hpp"
#include "../includes/viewer/PhaseViewer.hpp"
#include "../includes/viewer/PoolViewer.hpp"
#include "../includes/viewer/TitleViewer.hpp"
#include "../includes/viewer/MatchViewer.hpp"
#include "../includes/viewer/PlayerViewer.hpp"
#include "../includes/viewer/SettingsViewer.hpp"
#include "../includes/viewer/TournamentViewer.hpp"

#include "../includes/utils/Exporter.hpp"
#include "../includes/utils/PrintUtils.hpp"

#include "../includes/Color.hpp"

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
void				TournamentCLI::displayMenuUI(cTour tournament)
{
	CLIUtils::handleTitle(TitleViewer::tournament);
	PrintUtils::handleMessages();
	menuTournament(tournament);
	CLIUtils::checkInterruption();
}

/**
 *	Affiche les menus sous conditions du tournoi
 */
void				TournamentCLI::menuTournament(cTour tournament)
{
	std::vector<MenuItem> items =
	{
		{'1', "Teams"},
		{'2', "Pool"},
		{'3', "Print Pool"},
		{'4', "Print Team"},
		{'5', "Print Match"},
		{'6', "Print Phase"},
		{'7', "Print Player"},
		{'8', "Print Settings"},
		{'9', "Print Tournament"}
	};


	if (tournament.getHasSixteenth() && tournament.isSixteenthUnlocked())
		items.push_back({'3', "1/16"});

	if (tournament.getHasEighth() && tournament.isEighthUnlocked())
		items.push_back({'4', "1/8"});

	if (tournament.isQuartersUnlocked())
		items.push_back({'5', "1/4"});

	if (tournament.isSemisUnlocked())
		items.push_back({'6', "1/2"});

	if (tournament.isThirdUnlocked())
	{
		items.push_back({'7', "Petite finale"});
		items.push_back({'8', "Finale"});
	}

	if (tournament.isFinalUnlocked() && !tournament.getHasThirdMatch())
		items.push_back({'7', "Finale"});

	items.push_back({'E', "Export"});
	items.push_back({'S', "Show"});
	items.push_back({'Q', "Quit"});
	
	CLIUtils::displayMenu(std::format("TOURNOI : {}", tournament.getSettings().getName()), items);
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

		case 3:		//	Print Pool
			PoolViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 4:		//	Print Team
			TeamViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 5:		//	Print Match
			MatchViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 6:		//	Print Phase
			PhaseViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 7:		//	Print Player
			PlayerViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 8:		//	Print Settings
			SettingsViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		case 9:		//	Print Tournament
			TournamentViewer::printAll(tournament);
			CLIUtils::waitForEnter();
			break;

		/* case 3:		//	1/16
			if (tournament.getHasSixteenth() && tournament.isSixteenthUnlocked())
				handleEliminationPhase(tournament.getSixteenth(), [&]() { tournament.generateSixteenths(); },
					"1/16 DE FINALE", "1/16 generes avec succes !", "Impossible : verifiez que les poules soient terminees.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;

		case 4:		//	1/8
			if (tournament.getHasEighth() && tournament.isEighthUnlocked())
				handleEliminationPhase(tournament.getEighth(), [&]() { tournament.generateEighths(); },
					"1/8 DE FINALE", "1/8 generes avec succes !", "Impossible : verifiez que la phase precedente soit terminee.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;

		case 5:		//	1/4
			if (tournament.isQuartersUnlocked())
				handleEliminationPhase(tournament.getQuarters(), [&]() { tournament.generateQuarters(); },
					"QUARTS DE FINALE", "Quarts de finale generes avec succes !", "Impossible : verifiez que les poules/phases precedentes soient terminees.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;

		case 6:		//	1/2
			if (tournament.isSemisUnlocked())
				handleEliminationPhase(tournament.getSemis(), [&]() { tournament.generateSemis(); },
					"DEMI-FINALE", "Demi-finale generees avec succes !", "Impossible : verifiez que les quarts soient termines.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;

		case 7:		//	petite finale ou finale
			if (tournament.isThirdUnlocked())
				handleEliminationPhase(tournament.getThirdPlace(), [&]() { tournament.generateThirdPlace(); },
					"PETITE FINALE", "Matche de la petite finale generee avec succes !", "Impossible : verifiez que les demi-finales soient terminees.");
			else if (tournament.isFinalUnlocked())
				handleEliminationPhase(tournament.getFinal(), [&]() { tournament.generateFinal(); },
					"FINALE", "Finale generee avec succes !", "Impossible : verifiez que les demi-finales soient terminees.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;
		
		case 8:		//	finale
			if (tournament.isFinalUnlocked())
				handleEliminationPhase(tournament.getFinal(), [&]() { tournament.generateFinal(); },
					"FINALE", "Finale generee avec succes !", "Impossible : verifiez que les demi-finales soient terminees.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;

		*//*
		case 8:
			if (tournament.getQuarters() != nullptr)
				TournamentViewer::displayFullBracket(tournament);
			else
				PrintUtils::addError("L'arbre n'est pas encore disponible.");
			break;

		case 9:
			if (tournament.getFinal() && tournament.getFinal()->isFinished())
				TournamentViewer::displayPodium(tournament);
			else
				PrintUtils::addError("Le podium n'est pas encore disponible.");
			break;
		*/

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
void				TournamentCLI::handleEliminationPhase(pPhase phase,	const std::function<void()>& generateFn,
						cString phaseName, cString successMsg, cString errorMsg)
{
	if (!phase)
	{
		generateFn();

		if (phase)
			PrintUtils::addSuccess(successMsg);
		else
			PrintUtils::addError(errorMsg);
	}
	else
		handlePhase(phase, phaseName);
}

/**
 * Gere le flux complet de l export d une phase
 * Demande quelle phase, quel fichier, puis exporte.
 */
void				TournamentCLI::handleExport(Tournament& tournament)
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
			PrintUtils::addSuccess(std::format("Export réussi dans {}.", filename));
			break;
		}
		else
			PrintUtils::addError("Echec de l'export.");
	}
}

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

/************************/
/*  HANDLERS AFFICHAGE	*/
/************************/

/**
 * Affiche une phase si elle existe, sinon informe l utilisateur.
 */
void				TournamentCLI::handlePhase(Phase* phase, cString phaseName)
{
	if (!phase)
	{
		PrintUtils::addError(std::format("La phase '{}' n'existe pas ou n'est pas encore generee.", phaseName));
		return;
	}

	while (true)
	{
		CLIUtils::handleTitle(TitleViewer::tournament);
		PrintUtils::handleMessages();
		std::cout << "\n========== " << phaseName << " ==========\n\n";

		auto matches = phase->getMatches();

		if (matches.empty())
		{
			std::cout << "Aucun match pour cette phase.\n";
			CLIUtils::waitForEnter();
			return;
		}

		int count = 1;
		for (auto* m : matches)
		{
			if (m)
			{
				std::cout << "  " << std::setw(2) << count << ". ";
				MatchViewer::showMatchTitle(*m);
				std::cout << std::endl;
			}
			count++;
		}
		
		std::cout << "\n────────────────────────────────────────────────────────────\n";
		std::cout <<  Color::YELLOW << "\tR.\t" << Color::RESET << "Retour" << std::endl;
		std::cout << "============================================================\n";
		std::cout << "Votre choix : ";

		String input = CLIUtils::input();

		if (input.empty())
			continue;

		if (input == "r" || input == "R")
			return;

		/*auto choice = CLIUtils::parseInt(input);

		if (choice.has_value())
		{
			int idx = choice.value();
			if (idx >= 1 && idx <= static_cast<int>(matches.size()))
			{
				auto* m = matches[idx - 1];

				if (m)
					MatchCLI::submenuMatch(m);
			}
			else
				PrintUtils::addError("Numero de match invalide.");
		}
		else*/
		PrintUtils::addError("Saisie invalide. Entrez un numero ou R.");
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
			displayMenuUI(tournament);

			String input = CLIUtils::input();
			
			if (input.empty())
				continue;

			if (input == "q" || input == "Q")
				return;

			if (input == "e" || input == "E")
			{
				handleExport(tournament);
				continue;
			}

			if (input == "s" || input == "S")
			{
				//handleShow(tournament);
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
