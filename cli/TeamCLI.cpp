//
// Created by Nicolas Fordoxcel on 14/06/2026.
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
#include <exception>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Participant.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/cli/TeamCLI.hpp"
#include "../includes/cli/CLIUtils.hpp"

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

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/************************/
/*  GESTION DU MENU		*/
/************************/

void				TeamCLI::displayMenuUI(cTour tournament)
{
	CLIUtils::handleTitle(TitleViewer::teams);
	PrintUtils::handleMessages();
	TeamViewer::showAllTeams(tournament.getTeams());
	std::cout << "Selectionnez une team en entrant son ID (tapez 'r' pour revenir au menu precedent): ";
	CLIUtils::checkInterruption();
}

void				TeamCLI::menuTeam(pTeam team)
{
	std::vector<MenuItem> items =
	{
		{'1', "Modifier le nom"},
		{'2', "Modifier un membre"}
	};

	if (team->getIsDisqualified())
		items.push_back({'3', "Retirer la disqualification"});
	else
		items.push_back({'3', "Disqualifier l'equipe"});

	items.push_back({'R', "Retour au menu precedent"});

	CLIUtils::displayMenu(std::format("TEAM\t{}", team->getName()), items);
}

void				TeamCLI::submenuTeam(pTeam team, Tournament& tournament)
{
	try
	{
		while (true)
		{
			menuTeam(team);

			String input = CLIUtils::input();
			
			if (input.empty())
				continue;

			if (input == "r" || input == "R")
				return;

			auto choice = CLIUtils::parseInt(input);
			
			if (choice.has_value())
			{
				executeChoice(choice.value(), team, tournament);
				return;
			}
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}

void				TeamCLI::executeChoice(cInt choice, pTeam team, Tournament& tournament)
{
	switch (choice)
	{
		case 1:
			handleModifyTeamName(team);
			break;

		case 2:
			handleModifyTeamMember(team, tournament);
			break;

		case 3:
			handleDisqualifiedTeam(team, tournament);
			break;

		default:
			PrintUtils::addError("Choix non disponible.");
			break;
	}
}

/********************/
/*  HANDLER ACTION	*/
/********************/

void				TeamCLI::handleModifyTeamName(pTeam team)
{
	String actualName = team->getName();

	std::cout << "Nom actuel de la team: " << actualName << std::endl;
	std::cout << "Entrez le nouveau nom de la team: ";
	
	String newName;

	std::cin >> std::ws;
	std::getline(std::cin, newName);

	if (!newName.empty())
	{
		team->setName(newName);
		PrintUtils::addSuccess(std::format("Le nom de la team {} a ete modifie avec succes.", actualName));
	}
}

void				TeamCLI::handleModifyTeamMember(pTeam team, Tournament& tournament)
{
	int memberIdx = selectMemberIndex(team);

	if (memberIdx == -1)
		return;

	vpPart candidates = getEligibleSubstitutes(team, tournament);
	pPart newMember = selectSubstitutePlayer(candidates);

	if (!newMember)
		return;

	if (team->replaceMember(memberIdx, newMember))
	{
		team->renameTeam();
		PrintUtils::addSuccess("Membre remplace avec succes !");
	}
	else
		PrintUtils::addError("Erreur lors du remplacement du membre.");
}

void				TeamCLI::handleDisqualifiedTeam(pTeam team, Tournament& tournament)
{
	if (team->getIsDisqualified())
	{
		std::cout << "Voulez-vous retirer la disqualification de l'equipe " << team->getName() << "? (o/n)\n";
		
		String result = CLIUtils::input();
		
		if (result[0] == 'o' || result[0] == 'O')
		{
			team->disqualifyTeam(false);
			PrintUtils::addSuccess(std::format("La disqualification de la team {} a ete retiree.", team->getName()));
		}
	}
	else
	{
		std::cout << "Voulez-vous vraiment disqualifier l'equipe " << team->getName() << "? (o/n)\n";
		
		String result = CLIUtils::input();
		
		if (result[0] == 'o' || result[0] == 'O')
		{
			tournament.disqualifyTeam(team);
			PrintUtils::addSuccess(std::format("La team {} a ete disqualifiee.", team->getName()));
		}
	}
}

/********************/
/*  HELPER			*/
/********************/

bool				TeamCLI::checkTeamId(int id, Tournament& tournament)
{
	return (id >= 1 && id <= static_cast<int>(tournament.getTeams().size()));
}

cpPool				TeamCLI::findTeamPool(cpTeam team, Tournament& tournament)
{
	for (cpPool pool : tournament.getPools())
	{
		if (!pool)
			continue;

		for (cpTeam t : pool->getTeams())
			if (t == team)
				return (pool);
	}

	return (nullptr);
}

bool				TeamCLI::isPlayerInPool(cpPart player, cpPool pool)
{
	if (!pool || !player)
		return (false);

	for (cpTeam t : pool->getTeams())
		if (t && t->hasMember(player))
			return (true);
	
	return (false);
}

vpPart				TeamCLI::getEligibleSubstitutes(pTeam team, Tournament& tournament)
{
	vpPart eligible;
	cpPool targetPool = findTeamPool(team, tournament);

	for (auto* p : tournament.getParticipants())
	{
		if (!p || !p->getIsEliminated() || team->hasMember(p))
			continue;

		if (!isPlayerInPool(p, targetPool))
			eligible.push_back(p);
	}

	return (eligible);
}

int					TeamCLI::selectMemberIndex(pTeam team)
{
	const auto& members = team->getMembers();

	std::cout << "\nMembres actuels :\n";

	for (size_t i = 0; i < members.size(); ++i)
		if (members[i])
			std::cout << (i + 1) << ". " << members[i]->getPseudo() << "\n";

	std::cout << "Index du membre a remplacer : ";
	auto memberIdx = CLIUtils::parseInt(CLIUtils::input());

	if (memberIdx.has_value() && (memberIdx.value() < 1 || memberIdx.value() > static_cast<int>(members.size())))
	{
		PrintUtils::addError("Index invalide.");
		return -1;
	}

	return (memberIdx.value() - 1);
}

pPart				TeamCLI::selectSubstitutePlayer(vpPart candidates)
{
	if (candidates.empty())
	{
		PrintUtils::addError("Aucun joueur elimine disponible hors de cette poule.");
		return (nullptr);
	}

	std::cout << "\nJoueurs elimines disponibles :\n";

	for (size_t i = 0; i < candidates.size(); ++i)
		if (candidates[i])
			std::cout << (i + 1) << ". " << candidates[i]->getPseudo() << "\n";

	std::cout << "Index du nouveau joueur : ";
	auto choice = CLIUtils::parseInt(CLIUtils::input());

	if (choice.has_value() && (choice.value() < 1 || choice.value() > static_cast<int>(candidates.size())))
	{
		PrintUtils::addError("Index invalide.");
		return (nullptr);
	}

	return (candidates[choice.value() - 1]);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				TeamCLI::handleMenuTeam(Tournament& tournament)
{
	try
	{
		while (true)
		{
			displayMenuUI(tournament);

			String input = CLIUtils::input();
			
			if (input.empty())
				continue;

			if (input == "r" || input == "R")
				return;

			auto choice = CLIUtils::parseInt(input);

			if (!choice.has_value())
			{
				PrintUtils::addError("Saisie invalide");
				continue;
			}

			if (!checkTeamId(choice.value(), tournament))
			{
				PrintUtils::addError(std::format("l'id {} n'existe pas.", choice.value()));
				continue;
			}

			pTeam team = tournament.getTeamById(choice.value());

			if (team)
				submenuTeam(team, tournament);
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}
