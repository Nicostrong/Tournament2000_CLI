//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <csignal>
#include <cstdlib>

//	LOGICAL
#include "../includes/class/Participant.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

//	CLI
#include "../includes/cli/SettingsCLI.hpp"
#include "../includes/cli/TournamentCLI.hpp"
#include "../includes/cli/ParticipantCLI.hpp"
#include "../includes/utils/FormatUtils.hpp"
#include "../includes/utils/PrintUtils.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				V_STRING	=	std::vector<std::string>;
using				VP_PART		=	std::vector<Participant*>;
using				CVP_PART	=	const std::vector<Participant*>&;

/********************/
/*	SIGNALS			*/
/********************/

static VP_PART*		g_pendingParticipants = nullptr;

/**
 * Fonction appelee automatiquement lors de la reception d un SIGINT (Ctrl+C).
 */
static void			handleSigint(int signum)
{
	std::cout << "\n\n\033[1;31m[!] Interruption (SIGINT) detectee. Nettoyage de la memoire...\033[0m\n";

	if (g_pendingParticipants)
	{
		for (Participant* p : *g_pendingParticipants)
			delete p;
		
		g_pendingParticipants->clear();
		VP_PART().swap(*g_pendingParticipants);
		g_pendingParticipants = nullptr;
	}

	std::cout << "Memoire liberee. Fermeture du programme.\n";
	std::exit(signum);
}

/********************/
/*	HELPERS			*/
/********************/

static void			clearInput()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * Affiche le menu de preparation avec l etat courant des settings et participants.
 */
static void			printPreparationMenu(const Settings& s, CVP_PART participants)
{
	V_STRING		errors;
	const bool		settingsValid = const_cast<Settings&>(s).isValid(errors);
	const int		current = static_cast<int>(participants.size());
	const int		required = s.getNbPlayers();
	const bool		enoughPlayers = required == 0 ? 0 : (s.getAllowMultiTeamPlayers()
									? s.canAccommodate(current)
									: current >= required);

	std::cout << "\n===========================================================\n";
	std::cout << "\t\tMENU DE PREPARATION DU TOURNOI\n";
	std::cout << "===========================================================\n";
	std::cout << "\t\tConfig  : " << (settingsValid  ? "\033[1;32m[OK]\033[0m" : "\033[1;31m[INVALIDE]\033[0m") << "\n";
	std::cout << "\t\tJoueurs : " << current << " / " << required
			  << (enoughPlayers ? "  \033[1;32m[OK]\033[0m" : "  \033[1;31m[INCOMPLET]\033[0m") << "\n";
	std::cout << "===========================================================\n";
	std::cout << "\t\t1. Configurer le tournoi\n";
	std::cout << "\t\t2. Gerer les participants\n";
	std::cout << "\t\t3. Lancer le tournoi\n";
	std::cout << "\t\tq. Quitter\n";
	std::cout << "===========================================================\n";
	std::cout << "Choix : ";
}

/**
 * Verifie que le tournoi peut etre lance et affiche les erreurs le cas echeant.
 * Retourne true si tout est OK.
 */
static bool			canLaunch(const Settings& s, CVP_PART participants)
{
	V_STRING		errors;

	if (!const_cast<Settings&>(s).isValid(errors))
	{
		std::cout << "\n\033[1;31m[!] Configuration invalide :\033[0m\n";

		for (const STRING& e : errors)
			std::cout << "   - " << e << "\n";

		return (false);
	}

	const int		current = static_cast<int>(participants.size());
	const int		required = s.getNbPlayers();

	if (participants.empty())
	{
		std::cout << "\n\033[1;31m[!] Aucun participant inscrit.\033[0m\n";
		return (false);
	}

	if (!s.getAllowMultiTeamPlayers() && current < required)
	{
		std::cout << "\n\033[1;31m[!] Pas assez de participants : "
				  << current << "/" << required << " inscrits.\033[0m\n";
		return (false);
	}

	if (s.getAllowMultiTeamPlayers() && !s.canAccommodate(current))
	{
		std::cout << "\n\033[1;31m[!] Pas assez de joueurs meme avec recyclage : "
				  << current << "/" << required << ".\033[0m\n";
		return (false);
	}

	return (true);
}

/********************/
/*	MAIN			*/
/********************/

int					main()
{
	Settings		mySettings;
	VP_PART			pendingParticipants;
	char			choice;

	g_pendingParticipants = &pendingParticipants;
	
	std::signal(SIGINT, handleSigint);

	PrintUtils::clear();
	PrintUtils::banner();

	while (true)
	{
		printPreparationMenu(mySettings, pendingParticipants);

		if (!(std::cin >> choice))
		{
			clearInput();
			std::cout << "\033[1;31m[!] Saisie invalide.\033[0m\n";
			continue ;
		}

		clearInput();

		if (choice == 'q')
		{
			for (Participant* p : pendingParticipants)
				delete p;

			std::cout << "Au revoir !\n";
			return (0);
		}

		if (choice == '1')
		{
			SettingsCLI::setupWizard(mySettings);
			std::cout << mySettings;
		}
		else if (choice == '2')
		{
			ParticipantCLI::handleMenu(pendingParticipants);
		}
		else if (choice == '3')
		{
			if (!canLaunch(mySettings, pendingParticipants))
				continue ;

			std::cout << "\n\033[1;33mLancer le tournoi avec " << pendingParticipants.size()
					  << " participant(s) ? (o/n) : \033[0m";

			STRING	confirm;

			std::getline(std::cin, confirm);
			FormatUtils::trim(confirm);

			if (confirm.empty() || std::tolower(confirm[0]) != 'o')
			{
				std::cout << "Lancement annule.\n";
				continue ;
			}

			break ;
		}
		else
			std::cout << "\033[1;33m[!] Option invalide.\033[0m\n";
	}

	Tournament		myTournament(mySettings, pendingParticipants);

	if (!myTournament.initializeTournament())
	{
		std::cout << "\033[1;31m[!] Echec de l'initialisation du tournoi.\033[0m\n";

		for (Participant* p : pendingParticipants)
			delete p;

		return (1);
	}

	std::cout << "\033[1;32m\n[v] Tournoi initialise ! Bonne chance a tous !\033[0m\n";

	TournamentCLI::displayMenu(myTournament);

	return (0);
}