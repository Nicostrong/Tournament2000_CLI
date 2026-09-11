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

/**
 *	Gestion de l affichage du menu principale
 */
vMenuItem			ExportCLI::displayMenuUI(cTour tournament)
{
	vMenuItem menu = generateMenuExport(tournament);

	CLIUtils::handleTitle(TitleViewer::exportMenu);
	PrintUtils::handleMessages();
	CLIUtils::checkInterruption();
	return (menu);
}

/**
 *	Affiche les menus sous conditions du tournoi
*/
vMenuItem			ExportCLI::generateMenuExport(cTour tournament)
{
	std::vector<MenuItem> menuLst =
	{
		{"1", "Players (csv)"},
		{"2", "Players (txt)"},
		{"3", "Teams"},
		{"4", "Pool"}
	};

	if (tournament.getSixteenth())
		menuLst.push_back({"5", "1/16"});

	if (tournament.getEighth())
		menuLst.push_back({"6", "1/8"});

	if (tournament.getQuarters())
		menuLst.push_back({"7", "1/4"});

	if (tournament.getSemis())
		menuLst.push_back({"8", "1/2"});

	if (tournament.getThirdPlace())
		menuLst.push_back({"9", "Petite Final"});

	if (tournament.getFinal())
		menuLst.push_back({"10", "Final"});

	menuLst.push_back({"11", "Full tournament"});
	menuLst.push_back({"R", "Return"});
	
	return (menuLst);
}

/**
 *	Aiguillage du menu
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
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "Players.txt");

			if (Exporter::exportPlayersToTxt(tournament.getPlayers(), filename))
				PrintUtils::addSuccess("Export of players success.");
			else
				PrintUtils::addError("Export of players errors");
			break;

		case 3:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "Team.txt");

			if (Exporter::exportTeamsToTxt(tournament, filename))
				PrintUtils::addSuccess("Export of teams success.");
			else
				PrintUtils::addError("Export of teams errors");
			break;

		case 4:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "Pools.txt");

			if (Exporter::exportPoolsToTxt(tournament, filename))
				PrintUtils::addSuccess("Export of teams success.");
			else
				PrintUtils::addError("Export of teams errors");
			break;

		case 5:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "sixteenth.txt");

			if (Exporter::exportPhaseToTxt(tournament.getSixteenth(), filename))
				PrintUtils::addSuccess("Export of 1/16 success.");
			else
				PrintUtils::addError("Export of 1/16 errors.");
			break;

		case 6:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "eighth.txt");

			if (Exporter::exportPhaseToTxt(tournament.getEighth(), filename))
				PrintUtils::addSuccess("Export of 1/8 success.");
			else
				PrintUtils::addError("Export of 1/8 errors.");
			break;

		case 7:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "quarter.txt");

			if (Exporter::exportPhaseToTxt(tournament.getQuarters(), filename))
				PrintUtils::addSuccess("Export of 1/4 success.");
			else
				PrintUtils::addError("Export of 1/4 errors.");
			break;

		case 8:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "semis.txt");

		if (Exporter::exportPhaseToTxt(tournament.getSemis(), filename))
				PrintUtils::addSuccess("Export of 1/2 success.");
			else
				PrintUtils::addError("Export of 1/2 errors.");
			break;

		case 9:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "thirdPlace.txt");

			if (Exporter::exportPhaseToTxt(tournament.getThirdPlace(), filename))
				PrintUtils::addSuccess("Export of third place match success.");
			else
				PrintUtils::addError("Export of third place match errors.");
			break;

		case 10:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "final.txt");

			if (Exporter::exportPhaseToTxt(tournament.getFinal(), filename))
				PrintUtils::addSuccess("Export of final success.");
			else
				PrintUtils::addError("Export of final errors.");
			break;

		case 11:
			filename = CLIUtils::askString("Entrer le nom du fichier a creer: ", "tournament.txt");

			if (Exporter::exportTournamentToTxt(tournament, filename))
				PrintUtils::addSuccess("Export of full tournament success.");
			else
				PrintUtils::addError("Export of full tournament errors.");
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

			CLIUtils::displayMenu(menu);

			String input = CLIUtils::askMenuChoice(menu);
			
			if (input.empty())
				continue;

			if (input== "R")
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
