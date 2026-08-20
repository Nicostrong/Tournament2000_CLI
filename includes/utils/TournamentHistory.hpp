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
class				Participant;

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

using				pPart			=	Participant*;
using				cPart			=	const Participant&;
using				cpPart			=	const Participant*;
using				vpPart			=	std::vector<Participant*>;
using				cvpPart			=	const std::vector<Participant*>&;

using				mcpPartvpMatch	=	std::map<const Participant*, std::vector<Match*>>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				TournamentHistory
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
        void						recordMatch(pMatch match);
        void						exportParticipantSummary(cpPart p, cString filename) const;

};
