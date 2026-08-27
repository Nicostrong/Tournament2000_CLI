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
#include <sstream>
#include <iostream>
#include <exception>

#include "../includes/class/Settings.hpp"
#include "../includes/class/Participant.hpp"

#include "../includes/utils/CheckerCSV.hpp"
#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/FormatUtils.hpp"

#include "../includes/viewer/TitleViewer.hpp"
#include "../includes/viewer/ParticipantViewer.hpp"

#include "../includes/cli/CLIUtils.hpp"
#include "../includes/cli/ParticipantCLI.hpp"

#include "../includes/Color.hpp"
#include "../includes/Constantes.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

vpPart				ParticipantCLI::_partList;
bool				ParticipantCLI::_canAdd = false;
bool				ParticipantCLI::_hasParticipants = false;
bool				ParticipantCLI::_enoughPlayers = false;

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/************/
/*	MENU	*/
/************/

/**
 *	Affiche le menu Player avec sous-menu sous conditions
*/
void				ParticipantCLI::menuParticipant(cvpPart participants, cSet settings, cBool showMenu)
{
	cInt nbPlayersRequired = settings.getAllowMultiTeamPlayers()	? (settings.getNbPlayers() - NBPLAYERINMULTITEAMMAX)
																	: settings.getNbPlayers();
	const char* color = !_enoughPlayers ? Color::BBLUE : Color::BGREEN;

	TitleViewer::printSeparator(color);
	std::cout << color << "\n  Nb de joueurs enregistre pour le tournoi :\t\t" << participants.size() << " / " << settings.getNbPlayers() << "\n";
	std::cout << color << "  Nb de joueurs minimum requis pour le tournoi :\t" << nbPlayersRequired << "\n\n";
	TitleViewer::printSeparator(color);

	std::vector<MenuItem> items = {};
	
	if (_canAdd)
		items.push_back({'1', "Ajouter un nouveau participant"});

	if (showMenu)
	{
		items.push_back({'2', "Modifier un participant"});
		items.push_back({'3', "Supprimer un participant"});
	}

	items.push_back({'4', "Importer des participants (CSV)"});

	if (showMenu)
	{
		items.push_back({'5', "Exporter des participants (CSV)"});
		items.push_back({'6', "Afficher un/des participant(s)"});
	}

	if (_enoughPlayers)
		items.push_back({'7', "Lancer le tournoi"});

	CLIUtils::displayMenu("PLAYERS", items);
}

void				ParticipantCLI::executeChoice(int choice, vpPart& participants, cSet settings)
{
	switch (choice)
	{
		case 1:
			if (!_canAdd)
				PrintUtils::addError("Option invalide.");
			else
				handleAddParticipant(participants, settings);
			break;

		case 2:
			if (!_hasParticipants)
				PrintUtils::addError("Option invalide.");
			else
				handleModifyParticipant(participants, settings);
			break;

		case 3:
			if (!_hasParticipants)
				PrintUtils::addError("Option invalide.");
			else
				handleDeleteParticipant(participants);
			break;

		case 4:
			handleImport(participants, settings);
			break;

		case 5:
			if (!_hasParticipants)
				PrintUtils::addError("Option invalide.");
			else
				handleExport(participants);
			break;

		case 6:
			if (!_hasParticipants)
				PrintUtils::addError("Option invalide.");
			else
				handledisplay(participants);
			break;

		case 7:
			if (!_enoughPlayers)
				PrintUtils::addError("Option invalide.");
			else
				return;
			break;

		default:
			PrintUtils::addError("Option invalide.");
			break;
	}
}

/****************/
/*	SUBMENU		*/
/****************/

Participant*		ParticipantCLI::create(cvpPart participants, cSet settings)
{
	(void)participants;
	
	String lastName = CLIUtils::askString("Nom", "");
	String firstName = CLIUtils::askString("Prenom", "");
	String pseudo = CLIUtils::askString("Pseudo", "");
	Gender gender = askGender(settings);

	PrintUtils::addSuccess(std::format("Nouveau participant avec le pseudo {} ajoute avec succes.", pseudo));

	return (new Participant(pseudo, lastName, firstName, gender));
}

void				ParticipantCLI::destroy(const size_t id, vpPart& participants)
{
	for (auto it = participants.begin(); it != participants.end(); ++it)
	{
		if ((*it)->getId() == id)
		{
			PrintUtils::addSuccess(std::format("Le participant avec l'id {} et le pseudo {}, a ete supprime avec succes.", id, (*it)->getPseudo()));
			delete *it;
			participants.erase(it);
			return;
		}
	}

	PrintUtils::addError(std::format("Aucun participant avec l'ID {} n'a ete trouve.", id));
}

void				ParticipantCLI::modify(const size_t id, cvpPart participants, cSet settings)
{
	Participant* pToModify = nullptr;

	for (Participant* p: participants)
		if (p->getId() == id)
			pToModify = p;

	if (!pToModify)
	{
		PrintUtils::addError(std::format("Le participant avec l'id {} est introuvable.", id));
		return;
	}

	pToModify->setLastName(CLIUtils::askString("Nom", pToModify->getLastName()));
	pToModify->setFirstName(CLIUtils::askString("Prenom", pToModify->getFirstName()));
	pToModify->setPseudo(CLIUtils::askString("Pseudo", pToModify->getPseudo()));
	pToModify->setGender(askGender(settings, pToModify->getGenderInt()));

	PrintUtils::addSuccess(std::format("Le participant avec le pseudo {} a ete modifie avec succes.", pToModify->getPseudo()));
}

/************/
/*	IMPORT	*/
/************/

vpPart				ParticipantCLI::importFromCSV(cString filename)
{
	vpPart list;

	if (!CheckerCSV::validateParticipantCSV(filename))
		return (list);

	std::ifstream file(filename);

	String line;
	bool isFirstLine = true;

	while (std::getline(file, line))
	{
		FormatUtils::trim(line);

		if (line.empty())
			continue;

		Participant* newParticipant = extractParticipantFromLine(line, isFirstLine);
		
		if (newParticipant != nullptr)
			list.push_back(newParticipant);

		isFirstLine = false;
	}

	file.close();

	return (list);
}

/************/
/*	EXPORT	*/
/************/

bool				ParticipantCLI::exportToCSV(cvpPart participants, cString filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
	{
		PrintUtils::addError(std::format("Impossible d'ouvrir le fichier : {}", filename));
		return (false);
	}

	file << "Pseudo,Nom,Prenom,Genre\n";

	for (const Participant* p : participants)
		file	<< p->getPseudo() << ","
				<< p->getLastName() << ","
				<< p->getFirstName() << ","
				<< (p->getGenderInt() == Gender::MALE ? "0" : "1")
				<< "\n";

	file.close();

	PrintUtils::addSuccess(std::format("Tous les participants ont bien ete exportes dans le fichier {}.", filename));

	return (true);
}

/************/
/*	HANDLER	*/
/************/

void				ParticipantCLI::handleAddParticipant(vpPart& participants, cSet settings)
{
	if (participants.size() >= static_cast<size_t>(settings.getNbPlayers()))
	{
		PrintUtils::addError("Impossible d'ajouter un participant : le nombre maximum de joueurs est deja atteint.");
		return;
	}

	Participant* newParticipant = create(participants, settings);

	participants.push_back(newParticipant);
	PrintUtils::addSuccess("Participant ajoute avec succes !");
}

void				ParticipantCLI::handleModifyParticipant(cvpPart participants, cSet settings)
{
	if (participants.empty())
	{
		PrintUtils::addError("Aucun participant a modifier.");
		return;
	}

	ParticipantViewer::displayAll(participants);

	std::cout << "\nEntrez l'ID du participant a modifier : ";

	size_t id;

	if (!(std::cin >> id))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		PrintUtils::addError(std::format("L'id saisi est invalide => {}.", id));
		return;
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	modify(id, participants, settings);
}

void				ParticipantCLI::handleDeleteParticipant(vpPart& participants)
{
	if (participants.empty())
	{
		PrintUtils::addError("Aucun participant a supprimer.");
		return;
	}

	ParticipantViewer::displayAll(participants);

	std::cout << "\nEntrez l'ID du participant a supprimer : ";

	size_t id;

	if (!(std::cin >> id))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		PrintUtils::addError(std::format("L'id saisi est invalide => {}.", id));
		return;
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << "Confirmer la suppression ? (o/n) : ";

	String confirm;

	std::getline(std::cin, confirm);
	FormatUtils::trim(confirm);

	if (confirm.empty() || std::tolower(confirm[0]) != 'o')
	{
		PrintUtils::addSuccess("Suppression annulee.");
		return;
	}

	destroy(id, participants);
}

void				ParticipantCLI::handleImport(vpPart& participants, cSet settings)
{
	CLIUtils::checkInterruption();
	std::cout << "\nEntrez le chemin du fichier CSV a importer (ex: joueurs.csv) : ";

	String path;

	if (!std::getline(std::cin, path))
		CLIUtils::checkInterruption();

	FormatUtils::trim(path);

	if (path.empty())
	{
		PrintUtils::addError("Chemin vide. Import annule.");
		return;
	}

	cvpPart imported = importFromCSV(path);

	if (imported.empty())
	{
		PrintUtils::addError("Aucun participant importe. Verifiez le fichier.");
		return;
	}

	int skipped = 0;
	bool maxReachedMsgShown = false;

	for (Participant* p : imported)
	{
		if (participants.size() >= static_cast<size_t>(settings.getNbPlayers()))
		{
			if (!maxReachedMsgShown)
			{
				PrintUtils::addError("Le quota du tournoi est atteint.\n\t\tLes joueurs restants n'ont pas pu etre ajoutes.");
				maxReachedMsgShown = true;
			}

			delete p;
			skipped++;
		}
		else if (!checkPseudo(p->getPseudo(), participants))
		{
			PrintUtils::addError(std::format("Pseudo: {} deja utilise -> ignore.", p->getPseudo()));
			delete p;
			skipped++;
		}
		else if (!settings.getIsMixed() && p->getGenderInt() != settings.getTournamentGender())
		{
			PrintUtils::addError(std::format("Participant {} refuse (genre incompatible avec ce tournoi unisexe) -> ignore.", p->getPseudo()));
			delete p;
			skipped++;
		}
		else
			participants.push_back(p);
	}

	cInt added = static_cast<int>(imported.size()) - skipped;

	PrintUtils::addSuccess(std::format("{} participant(s) importe(s) et {} participant(s) ignore(s).", added, skipped));
}

void				ParticipantCLI::handleExport(cvpPart participants)
{
	if (participants.empty())
	{
		PrintUtils::addError("Aucun participant a exporter.");
		return;
	}

	std::cout << "\nEntrez le nom du fichier CSV a generer (ex: export_joueurs.csv) : ";

	String path;

	std::getline(std::cin, path);
	FormatUtils::trim(path);

	if (path.empty())
	{
		PrintUtils::addError("Chemin vide. Export annule.");
		return;
	}

	if (exportToCSV(participants, path))
		PrintUtils::addSuccess(std::format("{} participant(s) exporte(s) dans {}.", participants.size(), path));
	else
		PrintUtils::addError("Echec de l'exportation. Verifiez le chemin.");
}

void				ParticipantCLI::handledisplay(cvpPart participants)
{
	if (participants.empty())
	{
		PrintUtils::addError("Aucun participant enregistre.");
		return;
	}

	std::cout << Color::YELLOW << "\n\t1.\t" << Color::RESET << "Afficher tous les participants\n";
	std::cout << Color::YELLOW << "\t2.\t" << Color::RESET << "Afficher un participant par ID\n";
	std::cout << "Choix : ";

	int choice;

	if (!(std::cin >> choice))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		PrintUtils::addError("La saisie est invalide.");
		return;
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	if (choice == 1)
		_partList = participants;
	else if (choice == 2)
	{
		std::cout << Color::YELLOW << "Entrez l'ID du participant : " << Color::RESET;

		size_t id;

		if (!(std::cin >> id))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			PrintUtils::addError(std::format("L'id {} est invalide.", id));
			return;
		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		bool found = false;

		for (Participant* p : participants)
		{
			if (p->getId() == id)
			{
				_partList.push_back(p);
				found = true;
				break;
			}
		}

		if (!found)
			PrintUtils::addError(std::format("Aucun participant avec cet ID {}.", id));
	}
	else
		PrintUtils::addError("Option invalide.");
}

void				ParticipantCLI::handleList()
{
	if (_partList.empty())
		return;

	ParticipantViewer::displayAll(_partList);
	_partList.clear();
}

/************/
/*	HELPER	*/
/************/

/**
 * Verifie si un pseudo est deja enregistre dans la liste des participants
 * @param pseudo pseudo a evaluer
 * @param participants liste des participants deja enregistre
 * @return TRUE si le pseudo est libre
 * @return FALSE si le pseudo est deja existant
 */
bool				ParticipantCLI::checkPseudo(cString pseudo, cvpPart participants)
{
	for (cpPart p: participants)
		if (p->getPseudo() == pseudo)
			return (false);

	return (true);
}

Participant*		ParticipantCLI::extractParticipantFromLine(cString line, cBool isFirstLine)
{
	std::stringstream ss(line);
	String pseudo;
	String firstName;
	String lastName;
	String genderStr;

	std::getline(ss, pseudo, ',');
	std::getline(ss, lastName, ',');
	std::getline(ss, firstName, ',');
	std::getline(ss, genderStr, ',');

	FormatUtils::trim(pseudo);
	FormatUtils::trim(lastName);
	FormatUtils::trim(firstName);
	FormatUtils::trim(genderStr);

	if (isFirstLine && pseudo == "pseudo")
		return (nullptr);

	Gender gender = Gender::MALE;

	if (genderStr == "1")
		gender = Gender::FEMALE;

	return (new Participant(pseudo, lastName, firstName, gender));
}

void				ParticipantCLI::updateState(cvpPart participants, cSet settings)
{
	_hasParticipants = !participants.empty();
	_canAdd = participants.size() < static_cast<size_t>(settings.getNbPlayers());

	cInt nbPlayersRequired = settings.getAllowMultiTeamPlayers()
		? (settings.getNbPlayers() - NBPLAYERINMULTITEAMMAX)
		: settings.getNbPlayers();

	_enoughPlayers = participants.size() >= static_cast<size_t>(nbPlayersRequired);
}

Gender				ParticipantCLI::askGender(cSet settings, int currentGenderInt)
{
	while (true)
	{
		int g = CLIUtils::askInt("Sexe (0 = HOMME, 1 = FEMME)", 0, 1, currentGenderInt);
		
		if (!settings.getIsMixed() && static_cast<Gender>(g) != settings.getTournamentGender())
		{
			std::cout << Color::RED << "[!] ERREUR : Ce tournoi est exclusivement reserve aux participants de genre "
					<< (settings.getTournamentGender() == Gender::MALE ? "HOMME" : "FEMME") << ".\n" << Color::RESET;
			continue;
		}

		return (static_cast<Gender>(g));
	}
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				ParticipantCLI::handleMenuParticipant(vpPart& participants, cSet settings)
{
	try
	{
		while (true)
		{
			updateState(participants, settings);
			CLIUtils::handleTitle(TitleViewer::players);
			PrintUtils::handleMessages();
			handleList();
			menuParticipant(participants, settings, _hasParticipants);
			CLIUtils::checkInterruption();

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

			executeChoice(choice.value(), participants, settings);
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}


