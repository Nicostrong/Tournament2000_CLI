//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>
# include <memory>

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

using				cSet			=	const Settings&;

using				vpMatch			=	std::vector<Match*>;
using				vuMatch			=	std::vector<std::unique_ptr<Match>>;

using				pTeam			=	Team*;
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
		vuMatch						_matches;
		bool						_isFinished;

	public:

		Pool();

		Pool(cString) = delete;

		Pool(const Pool&) = delete;
		Pool& operator=(const Pool&) = delete;

		Pool(Pool&&) = delete;
		Pool& operator=(Pool&&) = delete;

		~Pool() = default;

		//	GETTER
		[[nodiscard]]
		cString						getName() const;
		[[nodiscard]]
		cvpTeam						getTeams() const;
		[[nodiscard]]
		vpTeam&						getTeamsMutable();
		[[nodiscard]]
		vpMatch						getMatches() const;
		[[nodiscard]]
		cBool						getIsFinished() const;

		//	METHOD
		void						addTeam(pTeam team);
		void						generateMatches(int nbSetsPerEncounter, cSet settings);
		void						sortTeams();
		void						checkPoolIsFinished();
		[[nodiscard]]
		bool						allMatchesFinished() const;
		[[nodiscard]]
		bool						containsTeam(cpTeam team) const;
		[[nodiscard]]
		vpTeam						getQualifiers() const;

};
