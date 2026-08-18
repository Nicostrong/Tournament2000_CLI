//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

//	STDLIB
#include <format>
#include <limits>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <exception>

//	INCLUDES
#include "../includes/Color.hpp"
#include "../includes/Global.hpp"
#include "../includes/Constantes.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/utils/FormatUtils.hpp"
#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/CheckerCSV.hpp"
#include "../includes/cli/ParticipantCLI.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VP_PART		=	std::vector<Participant*>;
using				CVP_PART	=	const std::vector<Participant*>&;

//	STATIC VARIABLES
VP_PART				ParticipantCLI::partList;

/**************************************************************************************************/
/*	EXCEPTION																					  */
/**************************************************************************************************/

namespace
{
	struct	UserInterruptedException : public std::exception {};

	void checkInterruption()
	{
		if (!g_running || std::cin.eof())
			throw UserInterruptedException();
	}
}

/************/
/*	PUBLIC	*/
/************/

void				ParticipantCLI::handleMenuParticipant(VP_PART& participants, const Settings& settings)
{
	try
	{
		int choice = -1;
		bool showMenu = false;

		while (choice != 0)
		{
			showMenu = !participants.empty();

			handleTitle();
			PrintUtils::handleMessages();
			handleList();
			menuParticipant(participants, settings, showMenu);
			checkInterruption();

			if (!(std::cin >> choice))
			{
				checkInterruption();

				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				PrintUtils::addError("Saisie invalide.");
				continue ;
			}

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			switch (choice)
			{
				case 1:
					handleAddParticipant(participants, settings);
					break ;

				case 2:
					if (!showMenu)
						break ;
					handleModifyParticipant(participants, settings);
					break ;

				case 3:
					if (!showMenu)
						break ;
					handleDeleteParticipant(participants);
					break ;

				case 4:
					handleImport(participants, settings);
					break ;

				case 5:
					if (!showMenu)
						break ;
					handleExport(participants);
					break;

				case 6:
					if (!showMenu)
						break ;
					handledisplay(participants);
					break ;

				case 7:
					return ;

				default:
					PrintUtils::addError("Option invalide.");
					continue ;
			}
		}
	}
	catch (const UserInterruptedException&)
	{
		return ;
	}
}

/************/
/*	MENU	*/
/************/

/**
 *	Affiche le menu Player avec sous-menu sous conditions
 */
void				ParticipantCLI::menuParticipant(CVP_PART participants, const Settings& settings,  const bool showMenu)
{
	const int nbPlayersRequired = settings.getAllowMultiTeamPlayers()	? (settings.getNbPlayers() - NBPLAYERINMULTITEAMMAX)
																		: settings.getNbPlayers();
	const bool enougtPlayers = participants.size() >= static_cast<size_t>(nbPlayersRequired);

	if (!enougtPlayers)
		std::cout << Color::BBLUE;
	else
		std::cout << Color::BGREEN;

	std::cout << "\n  Nb de joueurs enregistre pour le tournoi :\t\t" << participants.size() << " / " << settings.getNbPlayers() << "\n";
	std::cout << "  Nb de joueurs minimum requis pour le tournoi :\t" << nbPlayersRequired << "\n\n" << Color::RESET;
	std::cout << Color::YELLOW << "\t1.\t" << Color::RESET << "Ajouter un nouveau participant\n";

	if (showMenu)
	{
		std::cout << Color::YELLOW << "\t2.\t" << Color::RESET << "Modifier un participant\n";
		std::cout << Color::YELLOW << "\t3.\t" << Color::RESET << "Supprimer un participant\n";
	}

	std::cout << Color::YELLOW << "\t4.\t" << Color::RESET << "Importer des participants (CSV)\n";

	if (showMenu)
	{
		std::cout << Color::YELLOW << "\t5.\t" << Color::RESET << "Exporter les participants (CSV)\n";
		std::cout << Color::YELLOW << "\t6.\t" << Color::RESET << "Afficher un/des participant(s)\n";
	}

	if (enougtPlayers)
		std::cout << Color::YELLOW << "\t7.\t" << Color::RESET << "Lancer le tournoi\n\n";

	std::cout << "\tChoix : ";
}

/****************/
/*	SUBMENU		*/
/****************/

Participant*		ParticipantCLI::create(CVP_PART participants, const Settings& settings)
{
	STRING pseudo;
	STRING lastName;
	STRING firstName;
	int gender = -1;

	while (lastName.empty())
	{
		checkInterruption();
		std::cout << "Nom : ";

		if (!std::getline(std::cin, lastName))
			checkInterruption();

		FormatUtils::trim(lastName);
	}

	while (firstName.empty())
	{
		checkInterruption();
		std::cout << "Prenom : ";

		if (!std::getline(std::cin, firstName))
			checkInterruption();

		FormatUtils::trim(firstName);
	}

	while (pseudo.empty())
	{
		checkInterruption();
		std::cout << "Pseudo : ";

		if (!std::getline(std::cin, pseudo))
			checkInterruption();

		FormatUtils::trim(pseudo);
		FormatUtils::capitalize(pseudo);

		if (!checkPseudo(pseudo, participants))
		{
			std::cout << Color::RED << "[!] Ce pseudo est deja utilise. Choisissez-en un autre.\n" << Color::RESET;
			pseudo.clear();
		}
	}

	while (gender != 0 && gender != 1)
	{
		checkInterruption();
		std::cout << "Sexe (0 = HOMME, 1 = FEMME) : ";

		if (!(std::cin >> gender))
		{
			checkInterruption();
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << Color::RED << "[!] Saisie invalide. Entrez 0 pour HOMME ou 1 pour FEMME.\n" << Color::RESET;
			gender = -1;
			continue ;
		}

		if (gender != 0 && gender != 1)
		{
			std::cout << Color::RED << "[!] Saisie invalide. Entrez 0 pour HOMME ou 1 pour FEMME.\033[0m\n" << Color::RESET;
			gender = -1;
			continue ;
		}

		if (!settings.getIsMixed() && static_cast<Participant::Gender>(gender) != settings.getTournamentGender())
		{
			std::cout << Color::RED << "[!] ERREUR : Ce tournoi est exclusivement reserve aux participants de genre "
					  << (settings.getTournamentGender() == Participant::MALE ? "HOMME" : "FEMME") << ".\n" << Color::RESET;
			gender = -1;
		}
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	PrintUtils::addSuccess(std::format("Nouveau participant avec le pseudo {} ajoute avec succes.", pseudo));

	return (new Participant(pseudo, lastName, firstName, static_cast<Participant::Gender>(gender)));
}

void				ParticipantCLI::destroy(const size_t id, VP_PART& participants)
{
	for (auto it = participants.begin(); it != participants.end(); ++it)
	{
		if ((*it)->getId() == id)
		{
			PrintUtils::addSuccess(std::format("Le participant avec l'id {} et le pseudo {}, a ete supprime avec succes.", id, (*it)->getPseudo()));
			delete *it;
			participants.erase(it);
			return ;
		}
	}

	PrintUtils::addError(std::format("Aucun participant avec l'ID {} n'a ete trouve.", id));
}

void				ParticipantCLI::modify(const size_t id, CVP_PART participants, const Settings& settings)
{
	Participant* pToModify = nullptr;

	for (Participant* p: participants)
		if (p->getId() == id)
			pToModify = p;

	if (!pToModify)
	{
		PrintUtils::addError(std::format("Le participant avec l'id {} est introuvable.", id));
		return ;
	}

	STRING newPseudo;
	STRING newLastName;
	STRING newFirstName;
	int genderInput = -1;

	checkInterruption();
	std::cout << "Nom [" << pToModify->getLastName() << "] (Entree = conserver) : ";

	if (!std::getline(std::cin, newLastName))
		checkInterruption();

	FormatUtils::trim(newLastName);

	if (!newLastName.empty())
		pToModify->setLastName(newLastName);

	checkInterruption();
	std::cout << "Prenom [" << pToModify->getFirstName() << "] (Entree = conserver) : ";

	if (!std::getline(std::cin, newFirstName))
		checkInterruption();

	FormatUtils::trim(newFirstName);

	if (!newFirstName.empty())
		pToModify->setFirstName(newFirstName);

	while (true)
	{
		checkInterruption();
		std::cout << "Pseudo [" << pToModify->getPseudo() << "] (Entree = conserver) : ";

		if (!std::getline(std::cin, newPseudo))
			checkInterruption();

		FormatUtils::trim(newPseudo);

		if (newPseudo.empty() || newPseudo == pToModify->getPseudo())
			break ;

		if (!checkPseudo(newPseudo, participants))
		{
			std::cout << Color::RED << "Ce pseudo est deja utilise. Choisissez-en un autre.\n" << Color::RESET;
			continue ;
		}

		pToModify->setPseudo(newPseudo);
		break ;
	}

	while (genderInput != 0 && genderInput != 1)
	{
		checkInterruption();
		std::cout << "Sexe (0 = HOMME, 1 = FEMME) [" << pToModify->getGenderStr() << "] (Entree = conserver) : ";

		STRING gInput;

		if (!std::getline(std::cin, gInput))
			checkInterruption();

		FormatUtils::trim(gInput);

		if (gInput.empty())
			break ;

		try
		{
			genderInput = std::stoi(gInput);

			if (genderInput != 0 && genderInput != 1)
			{
				std::cout << Color::YELLOW << "[!] Entrez 0 (HOMME) ou 1 (FEMME).\n" << Color::RESET;
				genderInput = -1;
			}
			else if (!settings.getIsMixed() && static_cast<Participant::Gender>(genderInput) != settings.getTournamentGender())
			{
				std::cout << Color::RED << "[!] ERREUR : Ce tournoi est exclusivement "
						  << (settings.getTournamentGender() == Participant::MALE ? "HOMME" : "FEMME") << ".\n" << Color::RESET;
				genderInput = -1;
			}
			else
				pToModify->setGender(static_cast<Participant::Gender>(genderInput));
		}
		catch (const UserInterruptedException&)
		{
			throw;
		}
		catch (...)
		{
			std::cout << Color::RED << "[!] Saisie invalide.\n" << Color::RESET;
		}
	}

	PrintUtils::addSuccess(std::format("Le participant avec le pseudo {} a ete modifie avec succes.", pToModify->getPseudo()));
}

void				ParticipantCLI::displayOne(const Participant& p)
{
	std::cout << p;
}

void				ParticipantCLI::displayAll(CVP_PART participants)
{
	if (participants.empty())
	{
		PrintUtils::addError("Aucun participant enregistre.");
		return ;
	}

	std::cout << "\n=============== LISTE DES PARTICIPANTS  (" << participants.size() << ") ===============\n";

	size_t wId = 2;
	size_t wPseudo = 6;
	size_t wNom = 3;
	size_t wPrenom = 6;
	size_t wGenre = 5;
	size_t wElim = 7;
	size_t wMulti = 5;

	for (const Participant* p : participants)
	{
		wId = std::max(wId, std::to_string(p->getId()).length());
		wPseudo = std::max(wPseudo, p->getPseudo().length());
		wNom = std::max(wNom, p->getLastName().length());
		wPrenom = std::max(wPrenom, p->getFirstName().length());
		wGenre = std::max(wGenre, p->getGenderStr().length());
	}

	wId += 2; wPseudo += 2; wNom += 2; wPrenom += 2; wGenre += 2; wElim += 2; wMulti += 2;

	auto printSeparator = [&]() {
		std::cout << '+' << STRING(wId, '-')
		          << '+' << STRING(wPseudo, '-')
		          << '+' << STRING(wNom, '-')
		          << '+' << STRING(wPrenom, '-')
		          << '+' << STRING(wGenre, '-')
		          << '+' << STRING(wElim, '-')
		          << '+' << STRING(wMulti, '-') << "+\n";
	};

	printSeparator();
	std::cout << std::format("|{:<{}}|{:<{}}|{:<{}}|{:<{}}|{:<{}}|{:<{}}|{:<{}}|\n",
		" ID", wId,
		" Pseudo", wPseudo,
		" Nom", wNom,
		" Prenom", wPrenom,
		" Genre", wGenre,
		" Elimine", wElim,
		" Multi", wMulti);
	printSeparator();

	for (const Participant* p : participants)
	{
		STRING strElim = p->getIsEliminated() ? "Oui" : "Non";
		STRING strMulti = p->getIsMultiTeamPlayer() ? "Oui" : "Non";

		std::cout << std::format("| {:<{}}| {:<{}}| {:<{}}| {:<{}}| {:<{}}| {:<{}}| {:<{}}|\n",
			p->getId(), wId - 1,
			p->getPseudo(), wPseudo - 1,
			p->getLastName(), wNom - 1,
			p->getFirstName(), wPrenom - 1,
			p->getGenderStr(), wGenre - 1,
			strElim, wElim - 1,
			strMulti, wMulti - 1);
	}
	printSeparator();
	std::cout << std::endl;
}

/************/
/*	IMPORT	*/
/************/

VP_PART				ParticipantCLI::importFromCSV(C_STRING filename)
{
	VP_PART list;

	if (!CheckerCSV::validateParticipantCSV(filename))
		return (list);

	std::ifstream file(filename);

	STRING line;
	bool isFirstLine = true;

	while (std::getline(file, line))
	{
		FormatUtils::trim(line);

		if (line.empty())
			continue ;

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

bool				ParticipantCLI::exportToCSV(CVP_PART participants, C_STRING filename)
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
				<< (p->getGenderInt() == Participant::MALE ? "0" : "1")
				<< "\n";

	file.close();

	PrintUtils::addSuccess(std::format("Tous les participants ont bien ete exportes dans le fichier {}.", filename));

	return (true);
}

/************/
/*	HANDLER	*/
/************/

void				ParticipantCLI::handleAddParticipant(VP_PART& participants, const Settings& settings)
{
	if (participants.size() >= static_cast<size_t>(settings.getNbPlayers()))
	{
		PrintUtils::addError("Impossible d'ajouter un participant : le nombre maximum de joueurs est deja atteint.");
		return ;
	}

	Participant*	newParticipant = create(participants, settings);

	participants.push_back(newParticipant);
	PrintUtils::addSuccess("Participant ajoute avec succes !");
}

void				ParticipantCLI::handleModifyParticipant(CVP_PART& participants, const Settings& settings)
{
	if (participants.empty())
	{
		PrintUtils::addError("Aucun participant a modifier.");
		return ;
	}

	displayAll(participants);

	std::cout << "\nEntrez l'ID du participant a modifier : ";

	size_t id;

	if (!(std::cin >> id))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		PrintUtils::addError(std::format("L'id saisi est invalide => {}.", id));
		return ;
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	modify(id, participants, settings);
}

void				ParticipantCLI::handleDeleteParticipant(VP_PART& participants)
{
	if (participants.empty())
	{
		PrintUtils::addError("Aucun participant a supprimer.");
		return ;
	}

	displayAll(participants);

	std::cout << "\nEntrez l'ID du participant a supprimer : ";

	size_t id;

	if (!(std::cin >> id))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		PrintUtils::addError(std::format("L'id saisi est invalide => {}.", id));
		return ;
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << "Confirmer la suppression ? (o/n) : ";

	STRING confirm;

	std::getline(std::cin, confirm);
	FormatUtils::trim(confirm);

	if (confirm.empty() || std::tolower(confirm[0]) != 'o')
	{
		PrintUtils::addSuccess("Suppression annulee.");
		return ;
	}

	destroy(id, participants);
}

void				ParticipantCLI::handleImport(VP_PART& participants, const Settings& settings)
{
	checkInterruption();
	std::cout << "\nEntrez le chemin du fichier CSV a importer (ex: joueurs.csv) : ";

	STRING path;

	if (!std::getline(std::cin, path))
		checkInterruption();

	FormatUtils::trim(path);

	if (path.empty())
	{
		PrintUtils::addError("Chemin vide. Import annule.");
		return ;
	}

	CVP_PART imported = importFromCSV(path);

	if (imported.empty())
	{
		PrintUtils::addError("Aucun participant importe. Verifiez le fichier.");
		return ;
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

	const int added = static_cast<int>(imported.size()) - skipped;

	PrintUtils::addSuccess(std::format("{} participant(s) importe(s) et {} participant(s) ignore(s).", added, skipped));
}

void				ParticipantCLI::handleExport(CVP_PART participants)
{
	if (participants.empty())
	{
		PrintUtils::addError("Aucun participant a exporter.");
		return ;
	}

	std::cout << "\nEntrez le nom du fichier CSV a generer (ex: export_joueurs.csv) : ";

	STRING path;

	std::getline(std::cin, path);
	FormatUtils::trim(path);

	if (path.empty())
	{
		PrintUtils::addError("Chemin vide. Export annule.");
		return ;
	}

	if (exportToCSV(participants, path))
		PrintUtils::addSuccess(std::format("{} participant(s) exporte(s) dans {}.", participants.size(), path));
	else
		PrintUtils::addError("Echec de l'exportation. Verifiez le chemin.");
}

void				ParticipantCLI::handledisplay(CVP_PART participants)
{
	if (participants.empty())
	{
		PrintUtils::addError("Aucun participant enregistre.");
		return ;
	}

	std::cout << "\n1. Afficher tous les participants\n";
	std::cout << "2. Afficher un participant par ID\n";
	std::cout << "Choix : ";

	int choice;

	if (!(std::cin >> choice))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		PrintUtils::addError("La saisie est invalide.");
		return ;
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	if (choice == 1)
		partList = participants;
	else if (choice == 2)
	{
		std::cout << "Entrez l'ID du participant : ";

		size_t id;

		if (!(std::cin >> id))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			PrintUtils::addError(std::format("L'id {} est invalide.", id));
			return ;
		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		bool found = false;

		for (Participant* p : participants)
		{
			if (p->getId() == id)
			{
				partList.push_back(p);
				found = true;
				break ;
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
	if (partList.empty())
		return ;

	displayAll(partList);
	partList.clear();
}

void				ParticipantCLI::handleTitle()
{
	PrintUtils::clear();
	PrintUtils::banner();
	PrintUtils::players();
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
bool				ParticipantCLI::checkPseudo(C_STRING pseudo, CVP_PART participants)
{
	for (const Participant* p: participants)
		if (p->getPseudo() == pseudo)
			return (false);

	return (true);
}

Participant*		ParticipantCLI::extractParticipantFromLine(C_STRING line, bool isFirstLine)
{
	std::stringstream ss(line);
	STRING pseudo;
	STRING firstName;
	STRING lastName;
	STRING genderStr;

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

	Participant::Gender gender = Participant::MALE;

	if (genderStr == "1")
		gender = Participant::FEMALE;

	return (new Participant(pseudo, lastName, firstName, gender));
}

/*STRING				ParticipantCLI::printToTerminal(const Participant& p)
{
	return (std::format(
		"-----------------------------\n"
		"|  Participant\n"
		"-----------------------------\n"
		"|  Pseudo             : {}\n"
		"|  Prenom             : {}\n"
		"|  Nom                : {}\n"
		"|  Genre              : {}\n"
		"|  Elimine            : {}\n"
		"|  Multi-equipe       : {}\n"
		"-----------------------------\n",
		p.getPseudo(),
		p.getFirstName(),
		p.getLastName(),
		p.getGenderStr(),
		p.getIsEliminated() ? "Oui" : "Non",
		p.getIsMultiTeamPlayer() ? "Oui" : "Non"
	));
}

STRING				ParticipantCLI::printToCSV(const Participant& p)
{
	return (std::format("{},{},{},{}\n", p.getPseudo(), p.getFirstName(), p.getLastName(), p.getGenderInt()));
}*/

/********************/
/*	PRINT METHOD	*/
/********************/

std::ostream&		operator<<(std::ostream& os, const Participant& p)
{
	os << "------------------------------------------------\n";
	os << "|  Participant\t" << p.getId() << "\n";
	os << "------------------------------------------------\n";
	os << "|  Pseudo       : " << p.getPseudo() << std::endl;
	os << "|  Nom          : " << p.getLastName() << std::endl;
	os << "|  Prenom       : " << p.getFirstName() << std::endl;
	os << "|  Genre        : " << p.getGenderStr() << std::endl;
	os << "|  Elimine      : " << (p.getIsEliminated() ? "Oui" : "Non") << std::endl;
	os << "|  Multi-equipe : " << (p.getIsMultiTeamPlayer() ? "Oui" : "Non") << std::endl;
	os << "------------------------------------------------\n";

	return (os);
}