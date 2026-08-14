//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <cstring>

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

#include "../includes/Global.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				V_STRING	=	std::vector<std::string>;
using				VP_PART		=	std::vector<Participant*>;
using				CVP_PART	=	const std::vector<Participant*>&;

namespace
{
	//	ENUM
	enum class			AppState
	{
		SETTING,
		PLAYER,
		INIT_TOURNAMENT,
		POOL_PHASE,
		SIXTEENTH_PHASE,
		EIGHTH_PHASE,
		QUARTER_PHASE,
		SEMI_PHASE,
		FINAL_PHASE,
		THIRD_PHASE,
		EXIT
	};
}

/************/
/*	SIGNALS	*/
/************/

/**
 * Fonction appelee automatiquement lors de la reception d un SIGINT (Ctrl+C).
 */
static void			handleSigint(const int signum)
{
	(void)signum;

	std::cout << "\n\n\033[1;31m[!] Interruption detectee. Arret du programme...\033[0m\n";

	g_running = false;
}

/************/
/*	HELPERS	*/
/************/

/**
 *	Vide le buffer de l input
 */
/*static void			clearInput()
{
	if (!g_running)
		return ;
	
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}*/

/************/
/*	RUNNING	*/
/************/

/**
 *	Lance la phase de parametrage des settings
 */
static AppState			runSettingsPhase(Settings& settings)
{
	V_STRING errors;
	
	while (g_running)
	{
		SettingsCLI::setupWizard(settings);
		
		if (!g_running)
			return (AppState::EXIT);
		
		if (settings.isValid(errors))
			return (AppState::PLAYER);
			
		std::cout << "Configuration invalide. Veuillez corriger.\n";
	}

	return (AppState::EXIT);
}

/**
 *	Lance la phase de gestion des joueurs
 */
static AppState			runPlayersPhase(const Settings& settings, VP_PART& participants)
{
	while (g_running)
	{
		ParticipantCLI::handleMenu(participants); 
		
		if (!g_running)
			return (AppState::EXIT);
		
		const int current = static_cast<int>(participants.size());
		const int required = settings.getNbPlayers();
		
		if (current >= required)
		{
			std::cout << "\nNombre de joueurs atteint ! Impossible d'en ajouter plus.\n";
			std::cout << "Appuyez sur Entrée pour lancer la génération du tournoi...\n";
			
			STRING dummy;
			std::getline(std::cin, dummy);

			if (!g_running)
				return (AppState::EXIT);
			
			return (AppState::INIT_TOURNAMENT);
		}
	}
	return (AppState::EXIT);
}

/************/
/*	MAIN	*/
/************/

int					main()
{
	Settings mySettings;
	VP_PART pendingParticipants;
	Tournament* myTournament = nullptr;

	AppState currentState = AppState::SETTING;

	struct sigaction sa;

	std::memset(&sa, 0, sizeof(sa));

	sa.sa_handler = handleSigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, NULL);

	while (g_running && currentState != AppState::EXIT)
	{
		switch (currentState)
		{
			case AppState::SETTING:
				currentState = runSettingsPhase(mySettings);
				break ;
				
			case AppState::PLAYER:
				currentState = runPlayersPhase(mySettings, pendingParticipants);
				break ;

			case AppState::INIT_TOURNAMENT:
				if (!g_running)
					break ;

				myTournament = new Tournament(mySettings, pendingParticipants);

				if (!myTournament->initializeTournament())
				{
					std::cout << "[!] Erreur critique lors de l'initialisation.\n";
					currentState = AppState::EXIT;
				}
				else
					currentState = AppState::POOL_PHASE;
				break ;
				
			/*case AppState::POOL_PHASE:
				currentState = runPoolsPhase(*myTournament);
				break ;

			case AppState::SIXTEENTH_PHASE:
				currentState = runKnockoutPhase(*myTournament, currentState);
				break ;
			
			case AppState::EIGHTH_PHASE:
				currentState = runEighthsPhase(*myTournament, currentState);
				break ;

			case AppState::QUARTER_PHASE:
				currentState = runQuarterPhase(*myTournament, currentState);
				break ;

			case AppState::SEMI_PHASE:
				currentState = runSemisPhase(*myTournament, currentState);
				break ;

			case AppState::FINAL_PHASE:
				currentState = runFinalPhase(*myTournament, currentState);
				break ;

			case AppState::THIRD_PHASE:
				currentState = runThirdPhase(*myTournament, currentState);
				break ;*/

			case AppState::EXIT:
				break ;
			
			default:
				currentState = AppState::EXIT;
				break ;
		}
	}

	if (myTournament)
		delete myTournament;

	if (!pendingParticipants.empty())
		for (Participant* p : pendingParticipants)
			delete p;

	std::cout << "\nAu revoir !\n";

	return (0);
}