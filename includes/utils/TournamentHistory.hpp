//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

#ifndef TOURNAMANTHISTORY_HPP
# define TOURNAMANTHISTORY_HPP

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <map>

#include "../Global.hpp"

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class TournamentHistory
{
	private:

		mcpPartvpMatch				_participantMatches;
		vString						_globalEvents;

	public:

		//	CANONICAL
		TournamentHistory() = default;
		TournamentHistory(const TournamentHistory& ) = delete;
		TournamentHistory&			operator=(const TournamentHistory& ) = delete;
        ~TournamentHistory() = default;

		//	METHODS
        void						logEvent(cString event);
        void						recordMatch(cpMatch match);
        void						exportParticipantSummary(cpPart p, cString filename) const;

};

#endif
