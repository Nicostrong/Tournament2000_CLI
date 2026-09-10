//
// Created by Nicolas Fordoxcel on 08/09/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <vector>
#include <format>
#include <iostream>

//#include "../includes/class/Pool.hpp"
//#include "../includes/class/Team.hpp"
//#include "../includes/class/Phase.hpp"
//#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/cli/ExportCLI.hpp"
//#include "../includes/cli/TeamCLI.hpp"
//#include "../includes/cli/ShowCLI.hpp"
#include "../includes/cli/CLIUtils.hpp"
//#include "../includes/cli/PhaseCLI.hpp"
//#include "../includes/cli/TournamentCLI.hpp"

#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/utils/Exporter.hpp"
#include "../includes/utils/PrintUtils.hpp"

#include "../includes/Color.hpp"

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
vMenuItem			ExportCLI::displayMenuUI(cTour tournament)
{
	CLIUtils::handleTitle(TitleViewer::exportMenu);
	PrintUtils::handleMessages();
	vMenuItem menu = menuExport(tournament);
	CLIUtils::checkInterruption();
	return (menu);
}

/**
 *	Affiche les menus sous conditions du tournoi
*/
vMenuItem			ExportCLI::menuExport(cTour tournament)
{
	std::vector<MenuItem> menuLst =
	{
		{"1", "Players"},
		{"2", "Teams"},
		{"3", "Pool"}
	};

	if (tournament.getSixteenth())
		menuLst.push_back({"4", "1/16"});

	if (tournament.getEighth())
		menuLst.push_back({"5", "1/8"});

	if (tournament.getQuarters())
		menuLst.push_back({"6", "1/4"});

	if (tournament.getSemis())
		menuLst.push_back({"7", "1/2"});

	if (tournament.getThirdPlace())
		menuLst.push_back({"8", "Petite Final"});

	if (tournament.getFinal())
		menuLst.push_back({"9", "Final"});

	menuLst.push_back({"10", "Full tournament"});
	menuLst.push_back({"R", "Return"});
	
	CLIUtils::displayMenu(std::format("TOURNOI : {}", tournament.getSettings().getName()), menuLst);
	return (menuLst);
}


/********************/
/*  HANDLER SAISIE	*/
/********************/

/**
 *	Appel la bonne methode d apres le choix de l utilisateur
*/
void				ExportCLI::executeChoice(cInt choice, Tournament& tournament)
{
	String filename;

	switch (choice)
	{
		case 1:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "Players.csv");

			if (Exporter::exportPlayersToCSV(tournament.getPlayers(), filename))
				PrintUtils::addSuccess("Export of players success.");
			else
				PrintUtils::addError("Export of players errors");
			break;

		case 2:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "Team.txt");

			if (Exporter::exportPoolsToTxt(tournament, filename))
				PrintUtils::addSuccess("Export of teams success.");
			else
				PrintUtils::addError("Export of teams errors");
			break;

		case 3:
			Exporter::exportPoolsToTxt(tournament, "team.txt");
			break;

		case 4:
			if (tournament.getSixteenth())
			{
				filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "sixteenth.txt");

				if (Exporter::exportPoolsToTxt(tournament, filename))
					PrintUtils::addSuccess("Export of 1/16 success.");
				else
					PrintUtils::addError("Export of 1/16 errors.");
				break;
			}

			PrintUtils::addError("Export of 1/16 no available.");
			break;

		case 5:
			if (tournament.getEighth())
			{
				filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "eighth.txt");

				if (Exporter::exportPoolsToTxt(tournament, filename))
					PrintUtils::addSuccess("Export of 1/8 success.");
				else
					PrintUtils::addError("Export of 1/8 errors.");
				break;
			}

			PrintUtils::addError("Export of 1/8 no available.");
			break;

		case 6:
			if (tournament.getQuarters())
			{
				filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "quarter.txt");

				if (Exporter::exportPoolsToTxt(tournament, filename))
					PrintUtils::addSuccess("Export of 1/4 success.");
				else
					PrintUtils::addError("Export of 1/4 errors.");
				break;
			}

			PrintUtils::addError("Export of 1/4 no available.");
			break;

		case 7:
			if (tournament.getSemis())
			{
				filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "semis.txt");

				if (Exporter::exportPoolsToTxt(tournament, filename))
					PrintUtils::addSuccess("Export of 1/2 success.");
				else
					PrintUtils::addError("Export of 1/2 errors.");
				break;
			}

			PrintUtils::addError("Export of 1/2 no available.");
			break;

		case 8:
			if (tournament.getThirdPlace())
			{
				filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "thirdPlace.txt");

				if (Exporter::exportPoolsToTxt(tournament, filename))
					PrintUtils::addSuccess("Export of third place match success.");
				else
					PrintUtils::addError("Export of third place match errors.");
				break;
			}

			PrintUtils::addError("Export of third place match no available.");
			break;

		case 9:
			if (tournament.getFinal())
			{
				filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "final.txt");

				if (Exporter::exportPoolsToTxt(tournament, filename))
					PrintUtils::addSuccess("Export of final success.");
				else
					PrintUtils::addError("Export of final errors.");
				break;
			}

			PrintUtils::addError("Export of final no available.");
			break;

		case 10:
			if (tournament.getFinal())
			{
				filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "tournament.txt");

				if (Exporter::exportTournamentToTxt(tournament, filename))
					PrintUtils::addSuccess("Export of full tournament success.");
				else
					PrintUtils::addError("Export of full tournament errors.");
				break;
			}

			PrintUtils::addError("Export of full tournament no available.");
			break;

		default:
			PrintUtils::addError("Choix non disponible.");
			break;
	}
}

/**
 * Gere le flux complet de l export d une phase
 * Demande quelle phase, quel fichier, puis exporte.
 */
/*void				ExportCLI::handleExport(Tournament& tournament)
{
	while (true)
	{
		
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

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 *	Gestion du menu Tournament du programme
*/
void				ExportCLI::handleMenuExport(Tournament& tournament)
{
	try
	{
		while (true)
		{
			vMenuItem menu = displayMenuUI(tournament);
			String input = CLIUtils::askMenuChoice(menu);
			
			if (input.empty())
				continue;

			if (input == "r" || input== "R")
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
