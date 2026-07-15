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
#include "./Match.hpp"
#include "./Participant.hpp"

//	TYPEDEF

class TournamentHistory
{
	private:

		std::map<const Participant*, std::vector<const Match*>>		_participantMatches;
		std::vector<std::string>									_globalEvents;

		TournamentHistory(const TournamentHistory& );

		TournamentHistory&			operator=(const TournamentHistory& );

	public:

		TournamentHistory();
        ~TournamentHistory();

        void						logEvent(const std::string& event);
        void						recordMatch(const Match* match);
        void						exportParticipantSummary(const Participant* p,
																const std::string& filename) const;
        void						exportFullHistory(const std::string& filename) const;

};

#endif
