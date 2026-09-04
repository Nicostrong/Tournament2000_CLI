//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <map>
# include <string>
# include <vector>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Match;
class				Player;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;

using				pMatch			=	Match*;
using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

using				pPlayer			=	Player*;
using				cPlayer			=	const Player&;
using				cpPlayer		=	const Player*;
using				vpPlayer		=	std::vector<Player*>;
using				cvpPlayer		=	const std::vector<Player*>&;

using				mcpPlayervpMatch	=	std::map<const Player*, std::vector<Match*>>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				TournamentHistory
{
	private:

		mcpPlayervpMatch			_playerMatches;
		vString						_globalEvents;

	public:

		//	CANONICAL
		TournamentHistory() = default;

		TournamentHistory(const TournamentHistory&) = delete;
		TournamentHistory& operator=(const TournamentHistory& ) = delete;

		TournamentHistory(TournamentHistory&&) = delete;
		TournamentHistory& operator=(TournamentHistory&&) = delete;

        ~TournamentHistory() = default;

		//	METHODS
        void						logEvent(cString event);
        void						recordMatch(pMatch match);
        void						exportPlayerSummary(cpPlayer p, cString filename) const;

};
