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

using				vMenuItem		=	std::vector<MenuItem>;

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

/**
 *	Gestion de l affichage du menu principale
 */
void				PlayerCLI::displayMenuUI()
{
	CLIUtils::handleTitle(TitleViewer::players);
	PrintUtils::handleMessages();
	CLIUtils::checkInterruption();
}

/**
 *	Creation dynamique du menu
 */
vMenuItem			PlayerCLI::generateMenuPlayer(const PlayerManager& manager, cSet settings)
{
	std::vector<MenuItem> menuLst;
	cInt actualPlayers = static_cast<int>(manager.getSize());
	cInt maxPlayers = settings.getNbPlayers();
	cBool showMenu7 = (settings.getAllowMultiTeamPlayers()
		&& (maxPlayers - actualPlayers <= NBPLAYERINMULTITEAMMAX))
		|| (maxPlayers == actualPlayers);
	
	if (actualPlayers < maxPlayers)
		menuLst.push_back({"1", "Ajouter un nouveau participant"});

	if (!manager.isEmpty())
	{
		menuLst.push_back({"2", "Modifier un participant"});
		menuLst.push_back({"3", "Supprimer un participant"});
	}

	if (actualPlayers < maxPlayers)
		menuLst.push_back({"4", "Importer des participants (CSV)"});

	if (!manager.isEmpty())
	{
		menuLst.push_back({"5", "Exporter des participants (CSV)"});
		menuLst.push_back({"6", "Afficher un/des participant(s)"});
	}

	if (showMenu7)
		menuLst.push_back({"7", "Lancer le tournoi"});

	return (menuLst);
}

/**
 *	Aiguillage du menu
 */
bool				PlayerCLI::executeChoice(cInt choice, PlayerManager& manager, cSet settings)
{
	switch (choice)
	{
		case 1:
			handleAddPlayer(manager, settings);
			return (false);

		case 2:
			handleModifyPlayer(manager, settings);
			return (false);

		case 3:
			handleDeletePlayer(manager);
			return (false);

		case 4:
			handleImport(manager);
			return (false);

		case 5:
			handleExport(manager);
			return (false);

		case 6:
			handleDisplay(manager);
			return (false);

		case 7:
			return (true);

		default:
			PrintUtils::addError("Option invalide.");
			return (false);
	}
}

/********************/
/*  HANDLER ACTION	*/
/********************/

/**
 *	Ajouter un player
 */
void				PlayerCLI::handleAddPlayer(PlayerManager& manager, cSet settings)
{
	cString lastName = CLIUtils::askString("Nom", "");
	cString firstName = CLIUtils::askString("Prenom", "");
	cString pseudo = CLIUtils::askString("Pseudo", "");
	cGender gender = askGender(settings);

	if (!manager.addPlayer(pseudo, lastName, firstName, gender))
		return (PrintUtils::addError("Impossible d'ajouter le participant. Verifiez le pseudo, le quota et le genre."));

	PrintUtils::addSuccess(std::format("Nouveau participant avec le pseudo {} ajoute avec succes.", pseudo));
}

/**
 *	Modifier un player
 */
void				PlayerCLI::handleModifyPlayer(PlayerManager& manager, cSet settings)
{
	if (manager.isEmpty())
		return (PrintUtils::addError("Aucun participant a modifier."));

	PlayerViewer::showFullTableOfPlayers(manager.getPlayers());

	int id = CLIUtils::askInt("ID du participant a modifier", 0, static_cast<int>(manager.getSize() > 0 ? manager.getPlayers().back()->getId() : 0), -1);

	pPlayer player = manager.getPlayerById(static_cast<size_t>(id));

	if (!player)
		return (PrintUtils::addError(std::format("Le participant avec l'id {} est introuvable.", id)));

	cString lastName = CLIUtils::askString("Nom", player->getLastName());
	cString firstName = CLIUtils::askString("Prenom", player->getFirstName());
	cString pseudo = CLIUtils::askString("Pseudo", player->getPseudo());
	cGender gender = askGender(settings, static_cast<int>(player->getGenderInt()));

	if (!manager.modifyPlayer(player->getId(), pseudo, lastName, firstName, gender))
		return (PrintUtils::addError("Impossible de modifier le participant. Verifiez le pseudo et le genre."));

	PrintUtils::addSuccess(std::format("Le participant avec le pseudo {} a ete modifie avec succes.", player->getPseudo()));
}

/**
 *	Supprimer un player
 */
void				PlayerCLI::handleDeletePlayer(PlayerManager& manager)
{
	if (manager.isEmpty())
		return (PrintUtils::addError("Aucun participant a supprimer."));

	PlayerViewer::showFullTableOfPlayers(manager.getPlayers());

	cInt id = CLIUtils::askInt("ID du participant a supprimer", 0, static_cast<int>(manager.getPlayers().size()), -1);

	pPlayer player = manager.getPlayerById(static_cast<size_t>(id));

	if (!player)
		return (PrintUtils::addError(std::format("Aucun participant avec l'ID {} n'a ete trouve.", id)));

	cBool confirm = CLIUtils::askBool(std::format("Supprimer {} ?", player->getPseudo()), false);

	if (!confirm)
		return (PrintUtils::addSuccess("Suppression annulee."));

	if (manager.removePlayer(player->getId()))
		PrintUtils::addSuccess(std::format("Le participant {} a ete supprime avec succes.", player->getPseudo()));
	else
		PrintUtils::addError("Impossible de supprimer le participant.");
}

/**
 *	Importer des players
 */
void				PlayerCLI::handleImport(PlayerManager& manager)
{
	CLIUtils::checkInterruption();

	cString path = CLIUtils::askString("Chemin du fichier CSV", "joueurs.csv");

	if (path.empty())
		return (PrintUtils::addError("Chemin vide. Import annule."));

	cInt imported = Importer::importPlayers(path, manager);

	if (imported > 0)
		PrintUtils::addSuccess(std::format("Import termine. {} participant(s) present(s).", manager.getSize()));
	else
		PrintUtils::addError("Impossible d'importer les participants.");
}

/**
 *	Exporter des players
 */
void				PlayerCLI::handleExport(const PlayerManager& manager)
{
	if (manager.isEmpty())
		return (PrintUtils::addError("Aucun participant a exporter."));

	cString path = CLIUtils::askString("Nom du fichier CSV", "export_joueurs.csv");

	if (path.empty())
		return (PrintUtils::addError("Chemin vide. Export annule."));
}

/**
 *	Afficher un/des player/s
 */
void				PlayerCLI::handleDisplay(const PlayerManager& manager)
{
	if (manager.isEmpty())
		return (PrintUtils::addError("Aucun participant enregistre."));

	std::cout
		<< Color::YELLOW
		<< "\n\t1.\tAfficher tous les participants\n"
		<< "\t2.\tAfficher un participant par ID\n"
		<< Color::RESET;

	cInt choice = CLIUtils::askInt("Choix", 1, 2, 1);

	if (choice == 1)
	{
		PlayerViewer::showFullTableOfPlayers(manager.getPlayers());
		CLIUtils::waitForEnter();
		return;
	}

	cInt id = CLIUtils::askInt("ID du participant", 0, static_cast<int>(manager.getPlayers().size()), -1);

	pPlayer player = manager.getPlayerById(static_cast<size_t>(id));

	if (!player)
		return (PrintUtils::addError(std::format("Aucun participant avec cet ID {}.", id)));

	PlayerViewer::showPlayerCard(*player);
	CLIUtils::waitForEnter();
}

/**
 *	Demander le genre
 */
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

/**
 *	Menu principale du menu Player
 */
void				PlayerCLI::handleMenuPlayer(PlayerManager& manager, cSet settings)
{
	try
	{
		while (true)
		{
			displayMenuUI();

			if (!manager.isEmpty())
				PlayerViewer::showFullTableOfPlayers(manager.getPlayers());

			vMenuItem menu = generateMenuPlayer(manager, settings);

			CLIUtils::displayMenu(menu);

			cString input = CLIUtils::askMenuChoice(menu);

			if (input.empty())
				continue;

			if (input == "R")
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