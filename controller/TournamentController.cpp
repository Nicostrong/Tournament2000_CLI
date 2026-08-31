//
// Created by Nicolas Fordoxcel on 28/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "../includes/controller/TournamentController.hpp"

#include "../includes/manager/PlayerManager.hpp"

#include "../includes/class/Team.hpp"
#include "../includes/class/Pool.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/cli/PlayerCLI.hpp"
#include "../includes/cli/SettingsCLI.hpp"
#include "../includes/cli/TournamentCLI.hpp"

#include "../includes/utils/SettingsChecker.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				vString		=	std::vector<std::string>;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

TournamentController::TournamentController()
	: _settings(std::make_unique<Settings>()), _playerManager(nullptr), _tournament(nullptr)
{}

TournamentController::~TournamentController() = default;

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

TournamentController::State			TournamentController::handleSettingPhase()
{
	vString errors;
	
	while (g_running)
	{
		SettingsCLI::setupWizard(*this->_settings);
		
		if (!g_running)
			return (State::EXIT);

		if (SettingsChecker::isValid(*this->_settings, errors))
		{
			this->_playerManager = std::make_unique<PlayerManager>(*this->_settings);
			return (State::PLAYER);
		}

		std::cout << "Configuration invalide. Veuillez corriger les erreurs.\n";
	}

	return (State::EXIT);
}

TournamentController::State			TournamentController::handlePlayerPhase()
{
	if (!this->_playerManager)
		return (State::EXIT);

	PlayerCLI::handleMenuPlayer(*this->_playerManager, *this->_settings);
	
	if (!g_running)
		return (State::EXIT);

	return (State::INIT_TOURNAMENT);
}

TournamentController::State			TournamentController::handleInitPhase()
{
	if (!g_running || !this->_playerManager)
		return (State::EXIT);

	this->_tournament = std::make_unique<Tournament>(*this->_settings, *this->_playerManager);

	if (!this->_tournament->initializeTournament())
	{
		std::cout << Color::RED << "[!] Erreur critique lors de l'initialisation du tournoi.\n" << Color::RESET;
		this->_tournament.reset();
		return (State::EXIT);
	}

	return (State::RUN_TOURNAMENT);
}

TournamentController::State			TournamentController::handleRunPhase()
{
	if (!g_running || !this->_tournament)
		return (State::EXIT);

	TournamentCLI::handleMenuTournament(*this->_tournament);

	return (State::EXIT);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				TournamentController::run()
{
	State currentState = State::SETTING;

	while (g_running && currentState != State::EXIT)
	{
		switch (currentState)
		{
			case State::SETTING:
				currentState = handleSettingPhase();
				break;

			case State::PLAYER:
				currentState = handlePlayerPhase();
				break;

			case State::INIT_TOURNAMENT:
				currentState = handleInitPhase();
				break;

			case State::RUN_TOURNAMENT:
				currentState = handleRunPhase();
				break;

			case State::EXIT:
			default:
				currentState = State::EXIT;
				break;
		}
	}
	std::cout << Color::BBLUE << "\nAu revoir !\n" << Color::RESET;
}
