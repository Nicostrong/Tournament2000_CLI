//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <vector>
#include <format>
#include <string>
#include <iostream>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Player.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/cli/TeamCLI.hpp"
#include "../includes/cli/CLIUtils.hpp"

#include "../includes/viewer/TitleViewer.hpp"
#include "../includes/viewer/TeamViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cString			=	const std::string&;

using				cInt			=	const int;

using				pTeam			=	Team*;
using				cpTeam			=	const Team*;

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
 *	Gestion de l affichage du menu principale
 */
void				TeamCLI::displayMenuUI(cTour tournament)
{
	CLIUtils::handleTitle(TitleViewer::teams);
	PrintUtils::handleMessages();
	TeamViewer::showTeamsTableDetails(tournament.getTeams());
	CLIUtils::checkInterruption();
}

/**
 *	Gestion de l affichage du menu secondaire
 */
void				TeamCLI::submenuTeam(pTeam team, Tournament& tournament)
{
	try
	{
		while (true)
		{
			vMenuItem menu = generateSubmenuTeam(team);

			CLIUtils::displayMenu(menu, std::format("TEAM\t{}", team->getName()));
			cString input = CLIUtils::askMenuChoice(menu);
			
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

/**
 *	Creation dynamique du menu
 */
vMenuItem			TeamCLI::generateSubmenuTeam(pTeam team)
{
	std::vector<MenuItem> menuLst =
	{
		{"1", "Modifier le nom"},
		{"2", "Modifier un membre"}
	};

	if (team->getIsDisqualified())
		menuLst.push_back({"3", "Retirer la disqualification"});
	else
		menuLst.push_back({"3", "Disqualifier l'equipe"});

	menuLst.push_back({"R", "Retour au menu precedent"});

	return (menuLst);
}

/**
 *	Aiguillage du menu
 */
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

/**
 *	Modifier le nom d une team
 */
void				TeamCLI::handleModifyTeamName(pTeam team)
{
	cString newName = CLIUtils::askString("Entrez le nouveau nom de la team", team->getName());

	if (!newName.empty())
	{
		team->setName(newName);
		PrintUtils::addSuccess(std::format("Le nouveau nom de la team {} a ete modifie avec succes.", team->getName()));
	}
}

/**
 *	Modifier un membre d une team
 */
void				TeamCLI::handleModifyTeamMember(pTeam team, Tournament& tournament)
{
	cInt memberIdx = selectMemberIndex(team);

	if (memberIdx == -1)
		return;

	vpPlayer candidates = getEligibleSubstitutes(team, tournament);
	pPlayer newMember = selectSubstitutePlayer(candidates);

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

/**
 *	Disqualifier une team
 */
void				TeamCLI::handleDisqualifiedTeam(pTeam team, Tournament& tournament)
{
	if (team->getIsDisqualified())
	{
		cString result = CLIUtils::askBool(std::format("Voulez-vous retirer la disqualification de l'equipe {} ?", team->getName()), false) ? "O" : "N";
		
		if (result[0] == 'O')
		{
			team->disqualifyTeam(false);
			PrintUtils::addSuccess(std::format("La disqualification de la team {} a ete retiree.", team->getName()));
		}
	}
	else
	{
		cString result = CLIUtils::askBool(std::format("Voulez-vous vraiment disqualifier l'equipe {}", team->getName()), false) ? "O" : "N";
		
		if (result[0] == 'O')
		{
			tournament.disqualifyTeam(team);
			PrintUtils::addSuccess(std::format("La team {} a ete disqualifiee.", team->getName()));
		}
	}
}

/********************/
/*  HELPER			*/
/********************/

/**
 *	Verification d un id
 */
bool				TeamCLI::checkTeamId(int id, Tournament& tournament)
{
	return (id >= 0 && id < static_cast<int>(tournament.getTeams().size()));
}

/**
 *	Recherche la pool dans laquelle une team se trouve
 */
pPool				TeamCLI::findTeamPool(cpTeam team, Tournament& tournament)
{
	for (pPool pool : tournament.getPools())
	{
		if (!pool)
			continue;

		for (cpTeam t : pool->getTeams())
			if (t == team)
				return (pool);
	}

	return (nullptr);
}

/**
 *	Verifie si un player est dans la meme pool
 */
bool				TeamCLI::isPlayerInPool(pPlayer player, pPool pool)
{
	if (!pool || !player)
		return (false);

	for (cpTeam team : pool->getTeams())
		if (team && team->hasMember(player))
			return (true);
	
	return (false);
}

/**
 *	Recherche les players suceptible de remplacer un memebre d une team
 */
vpPlayer				TeamCLI::getEligibleSubstitutes(pTeam team, Tournament& tournament)
{
	vpPlayer eligible;
	pPool targetPool = findTeamPool(team, tournament);

	for (auto* p : tournament.getPlayers())
	{
		if (!p || !p->getIsEliminated() || team->hasMember(p))
			continue;

		if (!isPlayerInPool(p, targetPool))
			eligible.push_back(p);
	}

	return (eligible);
}

/**
 *	Selection du membre a remplacer
 */
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
		return (-1);
	}

	return (memberIdx.value() - 1);
}

/**
 *	Selection du player remplacant
 */
pPlayer				TeamCLI::selectSubstitutePlayer(vpPlayer candidates)
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

/**
 *	Menu principale du menu Team
 */
void				TeamCLI::handleMenuTeam(Tournament& tournament)
{
	try
	{
		while (true)
		{
			displayMenuUI(tournament);

			cString input = CLIUtils::askString("Selectionnez une team en entrant son ID (tapez 'r' pour revenir au menu precedent):", "r");
			
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
			else
				PrintUtils::addError("Saisie invalide.");
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}
