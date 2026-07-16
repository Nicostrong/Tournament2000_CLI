//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB
#include <iostream>
#include <vector>
#include <string>

//	LOGICAL
#include "../includes/class/Participant.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

//	CLI
#include "../includes/cli/SettingsCLI.hpp"
#include "../includes/cli/TournamentCLI.hpp"
#include "../includes/cli/ParticipantCLI.hpp"
#include "../includes/cli/PoolCLI.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef std::vector<std::string>	V_STRING;
typedef std::vector<Participant>	V_PART;

void				printMainMenu()
{
	std::cout	<< "\n========== MENU DE PREPARATION ==========\n";
	std::cout	<< "1. Configurer le tournoi (Valide : " << (settingsAreValid ? "Oui" : "Non") << ")\n";
	std::cout	<< "2. Ajouter un participant (" << pendingParticipants.size() << "/" << mySettings.getNbPlayers()
				<< " inscrits)\n";
	std::cout	<< "3. Lancer le tournoi\n";
	std::cout	<< "4. Importer des participants (CSV)\n";
	std::cout	<< "5. Exporter les participants (CSV)\n";
	std::cout	<< "0. Quitter\n";
	std::cout	<< "Choix : ";
}

void				executeChoice(int choice)
{
	switch (choice)
		{
			case 1:
				SettingsCLI::setupWizard(mySettings);
				SettingsCLI::display(mySettings);
				break ;

			case 2:
				pendingParticipants.push_back(ParticipantCLI::create());
				std::cout	<< "Participant ajoute avec succes !\n";
				break ;

			case 3:
			{
				if (!settingsAreValid)
				{
					std::cout	<< "\n[!] Impossible : La configuration actuelle est invalide. Allez dans le menu 1.\n";
					break ;
				}

				if (pendingParticipants.size() < (size_t)mySettings.getNbPlayers() && !mySettings.getAllowMultiTeamPlayers())
				{
					std::cout	<< "\n[!] Impossible : Pas assez de participants. (" << pendingParticipants.size() << "/" << mySettings.getNbPlayers() << ").\n";
					break ;
				}

				if (pendingParticipants.empty())
				{
					std::cout	<< "\n[!] Impossible : Vous devez inscrire des participants.\n";
					break ;
				}

				isTournamentReady = true;
				break ;
			}

			case 4:
			{
				std::cout	<< "\nEntrez le chemin du fichier CSV a importer (ex: joueurs.csv) : ";

				STRING				path;

				std::getline(std::cin, path);

				path = ParticipantCLI::trim(path);

				if (!path.empty())
				{
					V_PART			imported = ParticipantCLI::importFromCSV(path);

					if (!imported.empty())
					{
						pendingParticipants.insert(pendingParticipants.end(), imported.begin(), imported.end());
						std::cout	<< "\n[v] " << imported.size() << " participant(s) ajoute(s) au tournoi !\n";
					}
					else
						std::cout	<< "\n[!] Aucun participant n'a pu etre importe. Verifiez le fichier.\n";
				}
				break ;
			}

			case 5:
			{
				if (pendingParticipants.empty())
				{
					std::cout	<< "\n[!] Aucun participant a exporter.\n";
					break ;
				}

				std::cout	<< "\nEntrez le nom du fichier CSV a generer (ex: export_joueurs.csv) : ";

				STRING				path;

				std::getline(std::cin, path);
				path = ParticipantCLI::trim(path);

				if (!path.empty())
				{
					if (ParticipantCLI::exportToCSV(pendingParticipants, path))
						std::cout	<< "\n[v] Exportation reussie dans " << path << " !\n";
					else
						std::cout	<< "\n[!] Echec de l'exportation.\n";
				}
				break ;
			}

			case 0:
				return ;

			default:
				std::cout	<< "Choix invalide.\n";
				break ;
		}
}

int main()
{
	Settings						mySettings; 
	V_PART							pendingParticipants;
	bool							isTournamentReady = false;

	while (!isTournamentReady)
	{
		V_STRING					dummyErrors;
		bool						settingsAreValid = mySettings.isValid(dummyErrors);
		
		printMainMenu()();

		STRING						input;
		std::getline(std::cin, input);
		
		int							choice = -1;

		try
		{
			choice = std::stoi(input);
		}
		catch (...) {}

		executeChoice(choice);

	}

	Tournament						t(mySettings);

	for (const auto& p : pendingParticipants)
		t.addParticipant(p);

	if (t.initializeTournament())
	{
		std::cout	<< "\n============================================\n";
		std::cout	<< "   TOURNOI INITIALISE AVEC SUCCES ! " << std::endl;
		std::cout	<< "============================================\n";

		for (const auto& pool : t.getPools())
			PoolCLI::displayPoolDetails(*pool);

		TournamentCLI::displayMenu(t);
	}
	else
		std::cerr << "Une erreur inattendue est survenue lors de l'initialisation." << std::endl;

	return (0);
}