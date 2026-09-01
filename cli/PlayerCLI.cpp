//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <string>
#include <vector>
#include <iostream>

#include "../includes/class/Settings.hpp"

#include "../includes/manager/PlayerManager.hpp"

#include "../includes/viewer/TitleViewer.hpp"
#include "../includes/viewer/PlayerViewer.hpp"

#include "../includes/cli/CLIUtils.hpp"
#include "../includes/cli/PlayerCLI.hpp"

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/Importer.hpp"

#include "../includes/Color.hpp"
#include "../includes/Constantes.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

using				cSet			=	const Settings&;

using				cGender			=	const Gender&;

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

/************************/
/*  MENU				*/
/************************/

void				PlayerCLI::menuPlayer(const PlayerManager& manager, cSet settings)
{
	std::vector<MenuItem> items;
	cInt actualPlayers = static_cast<int>(manager.getSize());
	cInt maxPlayers = settings.getNbPlayers();
	bool showMenu7 = (settings.getAllowMultiTeamPlayers() 
		&& (maxPlayers - actualPlayers <= NBPLAYERINMULTITEAMMAX))
		|| (maxPlayers == actualPlayers);
	
	if (actualPlayers < maxPlayers)
		items.push_back({'1', "Ajouter un nouveau participant"});

	if (!manager.isEmpty())
	{
		items.push_back({'2', "Modifier un participant"});
		items.push_back({'3', "Supprimer un participant"});
	}

	if (actualPlayers < maxPlayers)
		items.push_back({'4', "Importer des participants (CSV)"});

	if (!manager.isEmpty())
	{
		items.push_back({'5', "Exporter des participants (CSV)"});
		items.push_back({'6', "Afficher un/des participant(s)"});
	}

	if (showMenu7)
		items.push_back({'7', "Lancer le tournoi"});

	CLIUtils::displayMenu("MENU PLAYERS", items);
}

/****************************************************************************************************/
/*	EXECUTION																						*/
/****************************************************************************************************/

bool				PlayerCLI::executeChoice(cInt choice, PlayerManager& manager, cSet settings)
{
	cInt actualPlayers = static_cast<int>(manager.getSize());
	cInt maxPlayers = settings.getNbPlayers();
	bool showMenu7 = (settings.getAllowMultiTeamPlayers() 
		&& (maxPlayers - actualPlayers <= NBPLAYERINMULTITEAMMAX))
		|| (maxPlayers == actualPlayers);

	switch (choice)
	{
		case 1:
			if (actualPlayers >= maxPlayers)
				PrintUtils::addError("Option invalide.");
			else
				handleAddPlayer(manager, settings);
			return (false);

		case 2:
			if (manager.isEmpty())
				PrintUtils::addError("Option invalide.");
			else
				handleModifyPlayer(manager, settings);
			return (false);

		case 3:
			if (manager.isEmpty())
				PrintUtils::addError("Option invalide.");
			else
				handleDeletePlayer(manager);
			return (false);

		case 4:
			if (actualPlayers >= maxPlayers)
				PrintUtils::addError("Option invalide.");
			else
				handleImport(manager);
			return (false);

		case 5:
			if (manager.isEmpty())
				PrintUtils::addError("Option invalide.");
			else
				handleExport(manager);
			return (false);

		case 6:
			if (manager.isEmpty())
				PrintUtils::addError("Option invalide.");
			else
				handleDisplay(manager);
			return (false);

		case 7:
			if (!showMenu7)
			{
				PrintUtils::addError("Option invalide.");
				return (false);
			}

			return (true);

		default:
			PrintUtils::addError("Option invalide.");
			return (false);
	}
}

/****************************************************************************************************/
/*	ADD																								*/
/****************************************************************************************************/

void				PlayerCLI::handleAddPlayer(PlayerManager& manager, cSet settings)
{
	cString lastName = CLIUtils::askString("Nom", "");
	cString firstName = CLIUtils::askString("Prenom", "");
	cString pseudo = CLIUtils::askString("Pseudo", "");
	cGender gender = askGender(settings);

	if (!manager.addPlayer(pseudo, lastName, firstName, gender))
	{
		PrintUtils::addError("Impossible d'ajouter le participant. Verifiez le pseudo, le quota et le genre.");
		return;
	}

	PrintUtils::addSuccess(std::format("Nouveau participant avec le pseudo {} ajoute avec succes.", pseudo));
}

/****************************************************************************************************/
/*	MODIFY																							*/
/****************************************************************************************************/

void				PlayerCLI::handleModifyPlayer(PlayerManager& manager, cSet settings)
{
	if (manager.isEmpty())
	{
		PrintUtils::addError("Aucun participant a modifier.");
		return;
	}

	PlayerViewer::displayAll(manager.getPlayers());

	int id = CLIUtils::askInt("ID du participant a modifier", 0, static_cast<int>(manager.getSize() > 0 ? manager.getPlayers().back()->getId() : 0), -1);

	pPlayer player = manager.getPlayerById(static_cast<size_t>(id));

	if (!player)
	{
		PrintUtils::addError(std::format("Le participant avec l'id {} est introuvable.", id));
		return;
	}

	cString lastName = CLIUtils::askString("Nom", player->getLastName());
	cString firstName = CLIUtils::askString("Prenom", player->getFirstName());
	cString pseudo = CLIUtils::askString("Pseudo", player->getPseudo());
	cGender gender = askGender(settings, static_cast<int>(player->getGenderInt()));

	if (!manager.modifyPlayer(player->getId(), pseudo, lastName, firstName, gender))
	{
		PrintUtils::addError("Impossible de modifier le participant. Verifiez le pseudo et le genre.");
		return;
	}

	PrintUtils::addSuccess(std::format("Le participant avec le pseudo {} a ete modifie avec succes.", player->getPseudo()));
}

/****************************************************************************************************/
/*	DELETE																							*/
/****************************************************************************************************/

void				PlayerCLI::handleDeletePlayer(PlayerManager& manager)
{
	if (manager.isEmpty())
	{
		PrintUtils::addError("Aucun participant a supprimer.");
		return;
	}

	PlayerViewer::displayAll(manager.getPlayers());

	cInt id = CLIUtils::askInt("ID du participant a supprimer", 0, 999999, -1);

	pPlayer player = manager.getPlayerById(static_cast<size_t>(id));

	if (!player)
	{
		PrintUtils::addError(std::format("Aucun participant avec l'ID {} n'a ete trouve.", id));
		return;
	}

	cBool confirm = CLIUtils::askBool(std::format("Supprimer {} ?", player->getPseudo()), false);

	if (!confirm)
	{
		PrintUtils::addSuccess("Suppression annulee.");
		return;
	}

	if (manager.removePlayer(player->getId()))
		PrintUtils::addSuccess(std::format("Le participant {} a ete supprime avec succes.", player->getPseudo()));
	else
		PrintUtils::addError("Impossible de supprimer le participant.");
}

/****************************************************************************************************/
/*	IMPORT																							*/
/****************************************************************************************************/

void				PlayerCLI::handleImport(PlayerManager& manager)
{
	CLIUtils::checkInterruption();

	cString path = CLIUtils::askString("Chemin du fichier CSV", "joueurs.csv");

	if (path.empty())
	{
		PrintUtils::addError("Chemin vide. Import annule.");
		return;
	}

	cInt imported = Importer::importPlayers(path, manager);

	if (imported > 0)
		PrintUtils::addSuccess(std::format("Import termine. {} participant(s) present(s).", manager.getSize()));
	else
		PrintUtils::addError("Impossible d'importer les participants.");
}

/****************************************************************************************************/
/*	EXPORT																							*/
/****************************************************************************************************/

void				PlayerCLI::handleExport(const PlayerManager& manager)
{
	if (manager.isEmpty())
	{
		PrintUtils::addError("Aucun participant a exporter.");
		return;
	}

	cString path = CLIUtils::askString("Nom du fichier CSV", "export_joueurs.csv");

	if (path.empty())
	{
		PrintUtils::addError("Chemin vide. Export annule.");
		return;
	}
}

/****************************************************************************************************/
/*	DISPLAY																							*/
/****************************************************************************************************/

void				PlayerCLI::handleDisplay(const PlayerManager& manager)
{
	if (manager.isEmpty())
	{
		PrintUtils::addError("Aucun participant enregistre.");
		return;
	}

	std::cout
		<< Color::YELLOW
		<< "\n\t1.\tAfficher tous les participants\n"
		<< "\t2.\tAfficher un participant par ID\n"
		<< Color::RESET;

	cInt choice = CLIUtils::askInt("Choix", 1, 2, 1);

	if (choice == 1)
	{
		PlayerViewer::displayAll(manager.getPlayers());
		CLIUtils::waitForEnter();
		return;
	}

	cInt id = CLIUtils::askInt("ID du participant", 0, static_cast<int>(manager.getPlayers().size()), -1);

	pPlayer player = manager.getPlayerById(static_cast<size_t>(id));

	if (!player)
	{
		PrintUtils::addError(std::format("Aucun participant avec cet ID {}.", id));
		return;
	}

	PlayerViewer::displayOne(*player);
	CLIUtils::waitForEnter();
}

/****************************************************************************************************/
/*	HELPER																							*/
/****************************************************************************************************/

Gender				PlayerCLI::askGender(cSet settings, cInt currentGenderInt)
{
	while (true)
	{
		cInt gender = CLIUtils::askInt("Sexe (0 = HOMME, 1 = FEMME)", 0, 1, currentGenderInt);

		cGender selectedGender = static_cast<Gender>(gender);

		if (!settings.getIsMixed() && selectedGender != settings.getTournamentGender())
		{
			std::cout
				<< Color::RED
				<< "[!] ERREUR : Ce tournoi est exclusivement reserve "
				<< "aux participants de genre "
				<< (settings.getTournamentGender() == Gender::MALE
					? "HOMME"
					: "FEMME")
				<< ".\n"
				<< Color::RESET;
			continue;
		}

		return (selectedGender);
	}
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				PlayerCLI::handleMenuPlayer(PlayerManager& manager, cSet settings)
{
	try
	{
		while (true)
		{
			CLIUtils::handleTitle(TitleViewer::players);
			PrintUtils::handleMessages();

			if (!manager.isEmpty())
				PlayerViewer::displayAll(manager.getPlayers());

			menuPlayer(manager, settings);

			CLIUtils::checkInterruption();

			cString input = CLIUtils::input();

			if (input.empty())
				continue;

			if (input == "r" || input == "R")
				return;

			auto choice = CLIUtils::parseInt(input);

			if (choice.has_value())
			{
				if (executeChoice(choice.value(), manager, settings))
					return;
			}
			else
				PrintUtils::addError("Saisie invalide.");
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}