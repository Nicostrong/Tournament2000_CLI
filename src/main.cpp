//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB
#include <iostream>
#include <vector>
#include <string>

//	LOGICAL
#include "../includes/Participant.hpp"
#include "../includes/Settings.hpp"
#include "../includes/Tournament.hpp"

//	CLI
#include "../includes/SettingsCLI.hpp"
#include "../includes/TournamentCLI.hpp"
#include "../includes/ParticipantCLI.hpp"
#include "../includes/PoolCLI.hpp"

int main()
{
	Settings mySettings; 
	std::vector<Participant> pendingParticipants;
	bool isTournamentReady = false;

	while (!isTournamentReady)
	{
		std::vector<std::string> dummyErrors;
		bool settingsAreValid = mySettings.isValid(dummyErrors);
		
		std::cout << "\n========== MENU DE PREPARATION ==========\n";
		std::cout << "1. Configurer le tournoi (Valide : " << (settingsAreValid ? "Oui" : "Non") << ")\n";
		std::cout << "2. Ajouter un participant (" << pendingParticipants.size() << "/" << mySettings.getNbPlayers() << " inscrits)\n";
		std::cout << "3. Lancer le tournoi\n";
		std::cout << "0. Quitter\n";
		std::cout << "Choix : ";

		std::string input;
		std::getline(std::cin, input);
		
		int choice = -1;

		try
		{
			choice = std::stoi(input);
		}
		catch (...) {}

		switch (choice)
		{
			case 1:
				SettingsCLI::setupWizard(mySettings);
				SettingsCLI::display(mySettings);
				break ;

			case 2:
				pendingParticipants.push_back(ParticipantCLI::create());
				std::cout << "Participant ajoute avec succes !\n";
				break ;

			case 3:
			{
				if (!settingsAreValid)
				{
					std::cout << "\n[!] Impossible : La configuration actuelle est invalide. Allez dans le menu 1.\n";
					break ;
				}

				if (pendingParticipants.size() < (size_t)mySettings.getNbPlayers() && !mySettings.getAllowMultiTeamPlayers())
				{
					std::cout << "\n[!] Impossible : Pas assez de participants. (" << pendingParticipants.size() << "/" << mySettings.getNbPlayers() << ").\n";
					break ;
				}

				if (pendingParticipants.empty())
				{
					std::cout << "\n[!] Impossible : Vous devez inscrire des participants.\n";
					break ;
				}

				isTournamentReady = true;
				break ;
			}

			case 0:
				return (0);

			default:
				std::cout << "Choix invalide.\n";
				break ;
		}
	}

	Tournament t(mySettings);

	for (const auto& p : pendingParticipants)
		t.addParticipant(p);

	if (t.initializeTournament())
	{
		std::cout << "\n============================================\n";
		std::cout << "   TOURNOI INITIALISE AVEC SUCCES ! " << std::endl;
		std::cout << "============================================\n";

		for (const auto& pool : t.getPools())
			PoolCLI::displayPoolDetails(*pool);

		TournamentCLI::displayMenu(t);
	}
	else
		std::cerr << "Une erreur inattendue est survenue lors de l'initialisation." << std::endl;

	return (0);
}