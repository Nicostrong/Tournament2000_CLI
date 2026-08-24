//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <limits>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <exception>

#include "../includes/class/Tournament.hpp"

#include "../includes/cli/TeamCLI.hpp"

#include "../includes/viewer/TitleViewer.hpp"
#include "../includes/viewer/TeamViewer.hpp"

#include "../includes/utils/Exporter.hpp"
#include "../includes/utils/PrintUtils.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

namespace
{
	struct	UserInterruptedException : public std::exception {};

	void checkInterruption()
	{
		if (!g_running || std::cin.eof())
			throw UserInterruptedException();
	}
}

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/************************/
/*  GESTION DU MENU		*/
/************************/

void				TeamCLI::displayMenuUI(cTour tournament)
{
	handleTitle();
	PrintUtils::handleMessages();
	TeamViewer::showAllTeams(tournament.getTeams());
	std::cout << "Selectionnez une team en entrant son ID (tapez 'r' pour revenir au menu precedent): ";
	checkInterruption();
}

void				TeamCLI::handleTitle()
{
	PrintUtils::clear();
	TitleViewer::banner();
	TitleViewer::teams();
}

void				TeamCLI::menuTeam()
{
	std::cout << "\n=========================== TEAM ===========================" << std::endl;
	std::cout <<  Color::YELLOW << "\t1.\t" << Color::RESET << "Modifier le nom" << std::endl;
	std::cout <<  Color::YELLOW << "\t2.\t" << Color::RESET << "Modifier un membre" << std::endl;
	std::cout <<  Color::YELLOW << "\t3.\t" << Color::RESET << "Disqualifier l'equipe" << std::endl;
	std::cout <<  Color::YELLOW << "\tR.\t" << Color::RESET << "Retour au menu precedent" << std::endl;
	std::cout << "============================================================" << std::endl;
	std::cout << "Votre choix : ";
}

void				TeamCLI::submenuTeam(pTeam team)
{
	try
	{
		while (true)
		{
			menuTeam();

			String input = fetchInput();
			
			if (input.empty())
				continue;

			if (input == "r" || input == "R")
				return;

			int choice = parseChoice(input);
			
			if (choice != -1)
				executeChoice(choice, team);
		}
	}
	catch (const UserInterruptedException&)
	{
		return;
	}
}

/********************/
/*  HANDLER SAISIE	*/
/********************/

void				TeamCLI::clearInput()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

String				TeamCLI::fetchInput()
{
	String input;
	
	if (!(std::cin >> input))
	{
		checkInterruption();
		clearInput();
		PrintUtils::addError("Saisie invalide.");
		return ("");
	}
	
	return (input);
}

int					TeamCLI::parseChoice(cString input)
{
	try
	{
		return (std::stoi(input));
	}
	catch (...)
	{
		PrintUtils::addError("Saisie invalide.");
		return (-1);
	}
}

void				TeamCLI::executeChoice(cInt choice, pTeam team)
{
	switch (choice)
	{
		case 1:
			handleModifyTeamName(team);
			break;

		case 2:
			handleModifyTeamMember(team);
			break;

		case 3:
			handleDisqualifiedTeam(team);
			break;

		default:
			PrintUtils::addError("Choix non disponible.");
			break;
	}
}

void				TeamCLI::handleShowOneTeams(pTeam team)
{
	(void)team;
}

void				TeamCLI::handleModifyTeamName(pTeam team)
{
	(void)team;
}

void				TeamCLI::handleModifyTeamMember(pTeam team)
{
	(void)team;
}

void				TeamCLI::handleDisqualifiedTeam(pTeam team)
{
	(void)team;
}

/*
void				TeamCLI::handleTeamManagement(Tournament& tournament)
{
	while (true)
	{
		PrintUtils::clear();
		TitleViewer::banner();
		TitleViewer::teams();
		std::cout << "\n--- GESTION DES EQUIPES ---\n";

		std::vector<Team*> allTeams;

		for (cpPool pool : tournament.getPools())
		{
			if (pool)
			{
				for (Team* team : pool->getTeams())
				{
					if (team)
					{
						allTeams.push_back(team);
						std::cout << allTeams.size() << ". " << team->getName() << "\n";
					}
				}
			}
		}

		std::cout << (allTeams.size() + 1) << ". Retour\n";
		std::cout << "Choisissez une equipe a modifier : ";

		String input = fetchInput();
		int choice = parseChoice(input);

		if (choice < 1 || choice > static_cast<int>(allTeams.size() + 1))
		{
			PrintUtils::addError("Choix invalide.");
			continue;
		}

		if (choice == static_cast<int>(allTeams.size() + 1))
			break;

		Team* selectedTeam = allTeams[choice - 1];
		handleSingleTeamEdit(selectedTeam, tournament);
	}
}

void				TeamCLI::handleSingleTeamEdit(Team* team, Tournament& tournament)
{
	while (true)
	{
		std::cout << "\n--- EQUIPE : " << team->getName() << " ---\n";
		std::cout << "1. Modifier le nom de l'equipe\n";
		std::cout << "2. Remplacer un membre\n";
		std::cout << "3. Retour\n";
		std::cout << "Votre choix : ";

		int choice = parseChoice(fetchInput());

		if (choice == 1)
		{
			String newName;

			std::cout << "Nouveau nom : ";
			std::cin.ignore();
			std::getline(std::cin, newName);
			
			if (!newName.empty())
			{
				team->setName(newName);
				PrintUtils::addSuccess("Nom de la team modifie avec succes !");
			}
		}
		else if (choice == 2)
		{
			const auto& members = team->getMembers();

			std::cout << "\nMembres actuels :\n";

			for (size_t i = 0; i < members.size(); ++i)
				if (members[i])
					std::cout << (i + 1) << ". " << members[i]->getFullName() << "\n";
			
			std::cout << "Membre a remplacer (index) : ";

			int memberIdx = parseChoice(fetchInput());
			
			if (memberIdx >= 1 && memberIdx <= static_cast<int>(members.size()))
			{
				(void)tournament;
			}
			else
			{
				PrintUtils::addError("Index invalide.");
			}
		}
		else if (choice == 3)
		{
			break;
		}
		else
		{
			PrintUtils::addError("Choix non disponible.");
		}
	}
}
*/

bool				TeamCLI::checkTeamId(int id, Tournament& tournament)
{
	if ( id == 0)
		return (false);
	
	return (tournament.getTeams().size() < static_cast<size_t>(id) ? false : true);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 *	On affiche un tableau avec toutes les teams
 *	On demande a l utilisateur de choisir une team via son id
 *	Si l id est correcte, on affiche le sous menu de gestion de la team
 *	sinon on affiche une erreur
 */
void				TeamCLI::handleMenuTeam(Tournament& tournament)
{
	try
	{
		while (true)
		{
			displayMenuUI(tournament);

			String input = fetchInput();
			
			if (input.empty())
				continue;

			if (input == "r" || input == "R")
				return;

			int choice = parseChoice(input);

			if (!checkTeamId(choice, tournament))
			{
				PrintUtils::addError(std::format("l'id {} n'existe pas.", choice));
				continue;
			}

			pTeam team = tournament.getTeamById(choice);
			
			if (team)
				submenuTeam(team);
		}
	}
	catch (const UserInterruptedException&)
	{
		return;
	}
}
