//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <cstring>
#include <csignal>
#include <iostream>

#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"
#include "../includes/class/Participant.hpp"

#include "../includes/cli/SettingsCLI.hpp"
#include "../includes/cli/TournamentCLI.hpp"
#include "../includes/cli/ParticipantCLI.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				vpPart			=	std::vector<Participant*>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

inline volatile std::sig_atomic_t	g_running = -1;

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

namespace
{
	enum class		AppState
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

/****************************************************************************************************/
/*	SIGNAL																							*/
/****************************************************************************************************/

/**
 * Fonction appelee automatiquement lors de la reception d un SIGINT (Ctrl+C).
 */
static void			handleSigint(cInt signum)
{
	(void)signum;

	std::cout << Color::RED << "\n\n[!] Interruption detectee. Arret du programme...\n" << Color::RESET;

	g_running = false;
}

/****************************************************************************************************/
/*	RUNNING																							*/
/****************************************************************************************************/

/**
 *	Lance la phase de parametrage des settings
 */
static AppState		runSettingsPhase(Settings& settings)
{
	vString errors;
	
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
static AppState		runPlayersPhase(cSet settings, vpPart& participants)
{
	while (g_running)
	{
		ParticipantCLI::handleMenuParticipant(participants, settings);
		
		if (!g_running)
			return (AppState::EXIT);
		
		return (AppState::INIT_TOURNAMENT);
	}
	return (AppState::EXIT);
}

/**
 *	Lance la phase de pool du tournoi
 */
static AppState		runPoolsPhase(Tournament& myTournament)
{
	while (g_running)
	{
		TournamentCLI::handleMenuTournament(myTournament);
		
		if (!g_running)
			return (AppState::EXIT);
		
		return (AppState::EXIT);
	}
	return (AppState::EXIT);
}

/****************************************************************************************************/
/*	MAIN																							*/
/****************************************************************************************************/

int					main()
{
	Settings mySettings;
	vpPart	pendingParticipants;
	Tournament* myTournament = nullptr;

	AppState currentState = AppState::SETTING;

	struct sigaction sa = {};

	sa.sa_handler = handleSigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, nullptr);

	while (g_running && currentState != AppState::EXIT)
	{
		switch (currentState)
		{
			case AppState::SETTING:
				currentState = runSettingsPhase(mySettings);
				break;
				
			case AppState::PLAYER:
				currentState = runPlayersPhase(mySettings, pendingParticipants);
				break;

			case AppState::INIT_TOURNAMENT:
				if (!g_running)
					break;

				myTournament = new Tournament(&mySettings, pendingParticipants);

				if (!myTournament->initializeTournament())
				{
					std::cout << "[!] Erreur critique lors de l'initialisation.\n";
					currentState = AppState::EXIT;
				}
				else
					currentState = AppState::POOL_PHASE;
				break;
				
			case AppState::POOL_PHASE:
				currentState = runPoolsPhase(*myTournament);
				break;
			/*
			case AppState::SIXTEENTH_PHASE:
				currentState = runKnockoutPhase(*myTournament, currentState);
				break;
			
			case AppState::EIGHTH_PHASE:
				currentState = runEighthsPhase(*myTournament, currentState);
				break;

			case AppState::QUARTER_PHASE:
				currentState = runQuarterPhase(*myTournament, currentState);
				break;

			case AppState::SEMI_PHASE:
				currentState = runSemisPhase(*myTournament, currentState);
				break;

			case AppState::FINAL_PHASE:
				currentState = runFinalPhase(*myTournament, currentState);
				break;

			case AppState::THIRD_PHASE:
				currentState = runThirdPhase(*myTournament, currentState);
				break;*/

			case AppState::EXIT:
				break;
			
			default:
				currentState = AppState::EXIT;
				break;
		}
	}

	if (myTournament)
		delete myTournament;
	else if (!pendingParticipants.empty())
		for (Participant* p : pendingParticipants)
			delete p;

	std::cout << Color::BBLUE << "\nAu revoir !\n" << Color::RESET;

	return (0);
}