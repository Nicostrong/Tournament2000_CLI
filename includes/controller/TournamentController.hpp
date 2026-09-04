//
// Created by Nicolas Fordoxcel on 30/08/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <memory>
# include <csignal>

# include "../class/Player.hpp"
# include "../class/Pool.hpp"


/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Settings;
class				Tournament;
class				PlayerManager;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cSet			=	const Settings&;
using				uSet			=	std::unique_ptr<Settings>;

using				uPlayerMan		=	std::unique_ptr<PlayerManager>;

using				uTour			=	std::unique_ptr<Tournament>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

extern volatile std::sig_atomic_t g_running;

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				TournamentController
{
	private:
	
		enum class					State
		{
			SETTING,
			PLAYER,
			INIT_TOURNAMENT,
			RUN_TOURNAMENT,
			EXIT
		};

		uSet						_settings;
		uPlayerMan					_playerManager;
		uTour						_tournament;

		State						handleSettingPhase();
		State						handlePlayerPhase();
		State						handleInitPhase();
		State						handleRunPhase();

	public:

		TournamentController();

		TournamentController(const TournamentController&) = delete;
		TournamentController& operator=(const TournamentController&) = delete;
		
		~TournamentController();
		
		void						run();

};