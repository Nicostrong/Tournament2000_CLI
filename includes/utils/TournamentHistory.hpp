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
typedef std::string								STRING;
typedef const std::string&						C_STRING;
typedef std::vector<std::string>				V_STRING;
typedef std::vector<const Match*>				V_MATCH;
typedef std::map<const Participant*, V_MATCH>	M_C_PART_V_MATCH;

class TournamentHistory
{
	private:

		M_C_PART_V_MATCH			_participantMatches;
		V_STRING					_globalEvents;

	public:

		//	CANONICAL
		TournamentHistory();
		TournamentHistory(const TournamentHistory& ) = delete;
		TournamentHistory&			operator=(const TournamentHistory& ) = delete;
        ~TournamentHistory();

		//	METHODS
        void						logEvent(C_STRING event);
        void						recordMatch(const Match* match);
        void						exportParticipantSummary(const Participant* p, C_STRING filename) const;
        void						exportFullHistory(C_STRING filename) const;

};

#endif
