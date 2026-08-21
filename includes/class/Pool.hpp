//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Match;
class				Team;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

using				pMatch			=	Match*;
using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

using				pTeam			=	Team*;
using				cTeam			=	const Team&;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				Pool
{
	private:

		static int					_idCounter;
		String						_name;
		vpTeam						_teams;
		vpMatch						_matches;

	public:

		Pool();
		explicit Pool(cString ) = delete;
		Pool(const Pool& ) = delete;
		Pool&						operator=(const Pool& ) = delete;
		~Pool();

		//	GETTER
		[[nodiscard]]
		cString						getName() const;
		[[nodiscard]]
		cvpTeam						getTeams() const;
		[[nodiscard]]
		cvpMatch					getMatches() const;

		//	METHOD
		void						addTeam(pTeam team);
		void						generateMatches(cInt nbSetsPerEncounter);
		void						sortTeams();
		bool						allMatchesFinished() const;

		[[nodiscard]]
		vpTeam						getQualifiers() const;

};
