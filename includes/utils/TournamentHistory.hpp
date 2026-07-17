//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

#ifndef TOURNAMANTHISTORY_HPP
# define TOURNAMANTHISTORY_HPP

//	STDLIB
#include <string>
#include <vector>
#include <map>

//	INCLUDES
#include "../class/Match.hpp"
#include "../class/Participant.hpp"

//	TYPEDEF
using				C_STRING			=	const std::string&;
using				V_STRING			=	std::vector<std::string>;
using				M_CP_PART_VP_MATCH	=	std::map<const Participant*, std::vector<Match*>>;

class TournamentHistory
{
	private:

		M_CP_PART_VP_MATCH			_participantMatches;
		V_STRING					_globalEvents;

	public:

		//	CANONICAL
		TournamentHistory() = default;
		TournamentHistory(const TournamentHistory& ) = delete;
		TournamentHistory&			operator=(const TournamentHistory& ) = delete;
        ~TournamentHistory() = default;

		//	METHODS
        void						logEvent(C_STRING event);
        void						recordMatch(Match* match);
        void						exportParticipantSummary(const Participant* p, C_STRING filename) const;
        //void						exportFullHistory(C_STRING filename) const;

};

#endif
