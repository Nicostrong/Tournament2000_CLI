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

class				Team;
class				Match;
class				Settings;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

using				pSet			=	Settings*;

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
		bool						_isFinished;

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
		[[nodiscard]]
		cBool						getIsFinished() const;

		//	METHOD
		void						addTeam(pTeam team);
		void						generateMatches(cInt nbSetsPerEncounter, pSet settings);
		void						sortTeams();
		void						checkPoolIsFinished();
		bool						allMatchesFinished() const;
		bool						containsTeam(cpTeam team) const;

		[[nodiscard]]
		vpTeam						getQualifiers() const;

};
