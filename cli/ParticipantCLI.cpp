//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

//	STDLIB
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>

//	INCLUDES
#include "../includes/class/Settings.hpp"
#include "../includes/cli/ParticipantCLI.hpp"
#include "../includes/utils/FormatUtils.hpp"
#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/CheckerCSV.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VP_PART		=	std::vector<Participant*>;
using				CVP_PART	=	const std::vector<Participant*>&;

//	STATIC VARIABLES

/********************/
/*	MENU			*/
/********************/

void				ParticipantCLI::menu()
{
	PrintUtils::clear();
	PrintUtils::banner();
	PrintUtils::players();
	std::cout	<< "\n========== MENU PARTICIPANT ==========\n";
	std::cout	<< "1. Ajouter un nouveau participant\n";
	std::cout	<< "2. Modifier un participant\n";
	std::cout	<< "3. Supprimer un participant\n";
	std::cout	<< "4. Importer des participants (CSV)\n";
	std::cout	<< "5. Exporter les participants (CSV)\n";
	std::cout	<< "6. Afficher un/des participant(s)\n";
	std::cout	<< "0. Revenir au menu principale\n";
	std::cout	<< "Choix : ";
}

void				ParticipantCLI::handleMenu(VP_PART& participants)
{
	int				choice = -1;

	while (choice != 0)
	{
		menu();

		std::cin >> choice;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\033[1;31m[!] Saisie invalide.\033[0m\n";

			continue ;
		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		switch (choice)
		{
			case 1:
				handleAddParticipant(participants);
				break ;

			case 2:
				handleModifyParticipant(participants);
				break ;

			case 3:
				handleDeleteParticipant(participants);
				break ;

			case 4:
				handleImport(participants);
				break ;

			case 5:
				handleExport(participants);
				break;

			case 6:
				handledisplay(participants);
				break ;

			case 0:
				return ;

			default:
				std::cout << "\033[1;33m[!] Option invalide.\033[0m\n";
				continue ;
		}
	}
}

/********************/
/*	SUBMENU			*/
/********************/

Participant*		ParticipantCLI::create(CVP_PART participants)
{
	STRING			pseudo;
	STRING			lastName;
	STRING			firstName;
	int				gender = -1;

	while (lastName.empty())
	{
		std::cout << "Nom : ";
		std::getline(std::cin, lastName);
		FormatUtils::trim(lastName);
	}

	while (firstName.empty())
	{
		std::cout << "Prenom : ";
		std::getline(std::cin, firstName);
		FormatUtils::trim(firstName);
	}

	while (pseudo.empty())
	{
		std::cout << "Pseudo : ";
		std::getline(std::cin, pseudo);
		FormatUtils::trim(pseudo);

		if (!checkPseudo(pseudo, participants))
		{
			std::cout << "\033[1;31m[!] Ce pseudo est deja utilise. Choisissez-en un autre.\033[0m\n";
			pseudo.clear();
		}
	}

	while (gender != 0 && gender != 1)
	{
		std::cout << "Sexe (0 = HOMME, 1 = FEMME) : ";
		std::cin >> gender;

		if (std::cin.fail() || (gender != 0 && gender != 1))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\033[1;31m[!] Saisie invalide. Entrez 0 pour HOMME ou 1 pour FEMME.\033[0m\n";
			gender = -1;
		}
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return (new Participant(pseudo, lastName, firstName, static_cast<Participant::Gender>(gender)));
}

void				ParticipantCLI::destroy(const size_t id, VP_PART& participants)
{
	for (auto it = participants.begin(); it != participants.end(); ++it)
	{
		if ((*it)->getId() == id)
		{
			delete *it;
			participants.erase(it);
			std::cout << "\033[1;32m[v] Participant supprime avec succes.\033[0m\n";
			return ;
		}
	}

	std::cout << "\033[1;31m[!] Aucun participant avec l'ID " << id << " n'a ete trouve.\033[0m\n";
}

void				ParticipantCLI::modify(const size_t id, CVP_PART participants)
{
	Participant*	pToModify = nullptr;

	for (Participant* p: participants)
		if (p->getId() == id)
			pToModify = p;

	if (!pToModify)
	{
		std::cout << "\033[1;31m[!] Participant introuvable (ID " << id << ").\033[0m\n";
		return ;
	}

	STRING			newPseudo;
	STRING			newLastName;
	STRING			newFirstName;
	int				genderInput = -1;

	std::cout << "Nom [" << pToModify->getLastName() << "] (Entree = conserver) : ";
	std::getline(std::cin, newLastName);
	FormatUtils::trim(newLastName);

	if (!newLastName.empty())
		pToModify->setLastName(newLastName);

	std::cout << "Prenom [" << pToModify->getFirstName() << "] (Entree = conserver) : ";
	std::getline(std::cin, newFirstName);
	FormatUtils::trim(newFirstName);

	if (!newFirstName.empty())
		pToModify->setFirstName(newFirstName);

	while (true)
	{
		std::cout << "Pseudo [" << pToModify->getPseudo() << "] (Entree = conserver) : ";
		std::getline(std::cin, newPseudo);
		FormatUtils::trim(newPseudo);

		if (newPseudo.empty())
			break ;

		if (newPseudo == pToModify->getPseudo())
			break ;

		if (!checkPseudo(newPseudo, participants))
		{
			std::cout << "\033[1;31m[!] Ce pseudo est deja utilise. Choisissez-en un autre.\033[0m\n";
			continue ;
		}

		pToModify->setPseudo(newPseudo);
		break ;
	}

	while (genderInput != 0 && genderInput != 1)
	{
		std::cout << "Sexe (0 = HOMME, 1 = FEMME) [" << pToModify->getGenderStr() << "] (Entree = conserver) : ";

		STRING		gInput;

		std::getline(std::cin, gInput);
		FormatUtils::trim(gInput);

		if (gInput.empty())
			break ;

		try
		{
			genderInput = std::stoi(gInput);

			if (genderInput != 0 && genderInput != 1)
			{
				std::cout << "\033[1;31m[!] Entrez 0 (HOMME) ou 1 (FEMME).\033[0m\n";
				genderInput = -1;
			}
			else
				pToModify->setGender(static_cast<Participant::Gender>(genderInput));
		}
		catch (...)
		{
			std::cout << "\033[1;31m[!] Saisie invalide.\033[0m\n";
		}
	}

	std::cout << "\033[1;32m[v] Participant modifie avec succes.\033[0m\n";
	std::cout << *pToModify;
}

void				ParticipantCLI::displayOne(const Participant& p)
{
	std::cout << p;
}

void				ParticipantCLI::displayAll(CVP_PART participants)
{
	if (participants.empty())
	{
		std::cout << "\033[1;33m[!] Aucun participant enregistre.\033[0m\n";
		return ;
	}

	PrintUtils::clear();
	std::cout << "\n========== LISTE DES PARTICIPANTS (" << participants.size() << ") ==========\n";

	for (const Participant* p: participants)
		std::cout << *p;
}

/********************/
/*	IMPORT			*/
/********************/

VP_PART				ParticipantCLI::importFromCSV(C_STRING filename)
{
	VP_PART			list;
	V_STRING		validationErrors;

	if (!CheckerCSV::validateParticipantCSV(filename, validationErrors))
	{
		std::cerr << "\n\033[1;31m[!] ERREUR : Le fichier CSV contient des erreurs de format.\033[0m\n";

		for (const auto& err : validationErrors)
			std::cerr << "  -> " << err << "\n";

		return (list);
	}

	std::ifstream	file(filename);

	if (!file.is_open())
	{
		std::cerr << "\033[1;31m[!] Impossible d'ouvrir le fichier : " << filename << "\033[0m\n";
		return (list);
	}

	STRING			line;
	bool			isFirstLine = true;

	while (std::getline(file, line))
	{
		FormatUtils::trim(line);

		if (line.empty())
			continue ;

		std::stringstream			ss(line);
		STRING						pseudo;
		STRING						firstName;
		STRING						lastName;
		STRING						genderStr;

		std::getline(ss, pseudo, ',');
		std::getline(ss, lastName, ',');
		std::getline(ss, firstName, ',');
		std::getline(ss, genderStr, ',');

		FormatUtils::trim(pseudo);
		FormatUtils::trim(lastName);
		FormatUtils::trim(firstName);
		FormatUtils::trim(genderStr);

		if (isFirstLine && pseudo == "pseudo")
		{
			isFirstLine = false;
			continue ;
		}

		Participant::Gender			gender = Participant::MALE;

		if (genderStr == "1")
			gender = Participant::FEMALE;

		list.push_back(new Participant(pseudo, lastName, firstName, gender));
	}

	file.close();

	return (list);
}

/********************/
/*	EXPORT			*/
/********************/

bool				ParticipantCLI::exportToCSV(CVP_PART participants, C_STRING filename)
{
	std::ofstream	file(filename);

	if (!file.is_open())
		return (false);

	file << "Pseudo,Nom,Prenom,Genre\n";

	for (const Participant* p : participants)
		file	<< p->getPseudo() << ","
				<< p->getLastName() << ","
				<< p->getFirstName() << ","
				<< (p->getGenderInt() == Participant::MALE ? "0" : "1")
				<< "\n";

	file.close();

	return (true);
}

/********************/
/*	HANDLER			*/
/********************/

void				ParticipantCLI::handleAddParticipant(VP_PART& participants)
{
	Participant*	newParticipant = create(participants);

	participants.push_back(newParticipant);

	std::cout << "\033[1;32m[v] Participant ajoute avec succes !\033[0m\n";
	std::cout << *newParticipant;
}

void				ParticipantCLI::handleModifyParticipant(VP_PART& participants)
{
	if (participants.empty())
	{
		std::cout << "\033[1;33m[!] Aucun participant a modifier.\033[0m\n";
		return ;
	}

	displayAll(participants);

	std::cout << "\nEntrez l'ID du participant a modifier : ";

	size_t			id;

	if (!(std::cin >> id))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\033[1;31m[!] Saisie invalide.\033[0m\n";
		return ;
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	modify(id, participants);
}

void				ParticipantCLI::handleDeleteParticipant(VP_PART& participants)
{
	if (participants.empty())
	{
		std::cout << "\033[1;33m[!] Aucun participant a supprimer.\033[0m\n";
		return ;
	}

	displayAll(participants);

	std::cout << "\nEntrez l'ID du participant a supprimer : ";

	size_t			id;

	if (!(std::cin >> id))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\033[1;31m[!] Saisie invalide.\033[0m\n";
		return ;
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	std::cout << "Confirmer la suppression ? (o/n) : ";

	STRING			confirm;

	std::getline(std::cin, confirm);
	FormatUtils::trim(confirm);

	if (confirm.empty() || std::tolower(confirm[0]) != 'o')
	{
		std::cout << "Suppression annulee.\n";
		return ;
	}

	destroy(id, participants);
}

void				ParticipantCLI::handleImport(VP_PART& participants)
{
	std::cout << "\nEntrez le chemin du fichier CSV a importer (ex: joueurs.csv) : ";

	STRING			path;

	std::getline(std::cin, path);
	FormatUtils::trim(path);

	if (path.empty())
	{
		std::cout << "\033[1;31m[!] Chemin vide. Import annule.\033[0m\n";
		return ;
	}

	VP_PART			imported = importFromCSV(path);

	if (imported.empty())
	{
		std::cout << "\033[1;31m[!] Aucun participant importe. Verifiez le fichier.\033[0m\n";
		return ;
	}

	int				skipped = 0;

	for (Participant* p : imported)
	{
		if (!checkPseudo(p->getPseudo(), participants))
		{
			std::cout << "\033[1;33m[!] Pseudo '" << p->getPseudo() << "' deja utilise -> ignore.\033[0m\n";
			delete p;
			skipped++;
		}
		else
			participants.push_back(p);
	}

	const int		added = static_cast<int>(imported.size()) - skipped;

	std::cout << "\033[1;32m[v] " << added << " participant(s) importe(s)";

	if (skipped > 0)
		std::cout << " (" << skipped << " doublon(s) ignore(s))";

	std::cout << ".\033[0m\n";
}

void				ParticipantCLI::handleExport(CVP_PART participants)
{
	if (participants.empty())
	{
		std::cout << "\033[1;33m[!] Aucun participant a exporter.\033[0m\n";
		return ;
	}

	std::cout << "\nEntrez le nom du fichier CSV a generer (ex: export_joueurs.csv) : ";

	STRING			path;

	std::getline(std::cin, path);
	FormatUtils::trim(path);

	if (path.empty())
	{
		std::cout << "\033[1;31m[!] Chemin vide. Export annule.\033[0m\n";
		return ;
	}

	if (exportToCSV(participants, path))
		std::cout << "\033[1;32m[v] " << participants.size() << " participant(s) exporte(s) dans '" << path << "'.\033[0m\n";
	else
		std::cout << "\033[1;31m[!] Echec de l'exportation. Verifiez le chemin.\033[0m\n";
}

void				ParticipantCLI::handledisplay(CVP_PART participants)
{
	if (participants.empty())
	{
		std::cout << "\033[1;33m[!] Aucun participant enregistre.\033[0m\n";
		return ;
	}

	std::cout << "\n1. Afficher tous les participants\n";
	std::cout << "2. Afficher un participant par ID\n";
	std::cout << "Choix : ";

	int				choice;

	if (!(std::cin >> choice))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\033[1;31m[!] Saisie invalide.\033[0m\n";
		return ;
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	if (choice == 1)
	{
		displayAll(participants);
	}
	else if (choice == 2)
	{
		std::cout << "Entrez l'ID du participant : ";

		size_t		id;

		if (!(std::cin >> id))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\033[1;31m[!] Saisie invalide.\033[0m\n";
			return ;
		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		bool		found = false;

		for (const Participant* p : participants)
		{
			if (p->getId() == id)
			{
				displayOne(*p);
				found = true;
				break ;
			}
		}

		if (!found)
			std::cout << "\033[1;31m[!] Aucun participant avec l'ID " << id << ".\033[0m\n";
	}
	else
		std::cout << "\033[1;33m[!] Option invalide.\033[0m\n";
}

/********************/
/*	HELPER			*/
/********************/

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