//
// Created by Nicolas Fordoxcel on 24/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>
# include <memory>

/*******************±±*********************************************************************************/
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

using				cSet			=	const Settings&;

using				vpMatch			=	std::vector<Match*>;
using				vuMatch			=	std::vector<std::unique_ptr<Match>>;

using				pTeam			=	Team*;
using				vpTeam			=	std::vector<Team*>;

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
		vuMatch						_matches;

		[[nodiscard]]
		pTeam						getEncounterWinner(size_t index) const;

	public:

		Phase() = delete;
		Phase(String name, int nbSets);

		Phase(const Phase&) = delete;
		Phase& operator=(const Phase&) = delete;

		Phase(Phase&&) = delete;
		Phase& operator=(Phase&&) = delete;

		~Phase() = default;

		// GETTER
		[[nodiscard]]
		cString						getName() const;
		[[nodiscard]]
		int							getNbSetToPlay() const;
		[[nodiscard]]
		bool						getIsFinished() const;
		[[nodiscard]]
		vpMatch						getMatches() const;
		[[nodiscard]]
		vpTeam						getWinners() const;
		[[nodiscard]]
		vpTeam						getLosers() const;

		//	SETTER
		void						setIsFinished(bool isFinished);

		//	METHOD
		void						addEncounter(pTeam a, pTeam b, cSet settings);
		[[nodiscard]]
		bool						isFinished() const;

};
