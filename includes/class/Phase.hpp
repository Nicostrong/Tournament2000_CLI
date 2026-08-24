//
// Created by Nicolas Fordoxcel on 24/06/2026.
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
/*	CLASSE																							*/
/****************************************************************************************************/

class				Phase
{
	private:
	
		String						_name;
		int							_nbSetsToPlay;
		bool						_isFinished;
		vpMatch						_matches;

		Team*						getEncounterWinner(size_t index) const;

	public:

		Phase() = delete;
		Phase(String name, int nbSets);
		Phase(const Phase& ) = delete;
		Phase&						operator=(const Phase& ) = delete;
		~Phase();

		// GETTER
		[[nodiscard]]
		cString						getName() const;
		[[nodiscard]]
		int							getNbSetToPlay() const;
		[[nodiscard]]
		bool						getIsFinished() const;
		[[nodiscard]]
		cvpMatch					getMatches() const;
		[[nodiscard]]
		vpTeam						getWinners() const;
		[[nodiscard]]
		vpTeam						getLosers() const;

		//	SETTER
		void						setIsFinished(cBool isFinished);

		//	METHOD
		void						addEncounter(pTeam a, pTeam b);
		[[nodiscard]]
		bool						isFinished() const;

};
