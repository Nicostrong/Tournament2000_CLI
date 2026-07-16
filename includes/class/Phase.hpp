//
// Created by Nicolas Fordoxcel on 24/06/2026.
//

#ifndef PHASE_HPP
# define PHASE_HPP

//	STDLIB
# include <string>
# include <vector>

//	INCLUDES
# include "Match.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef std::vector<Match*>			VP_MATCH;
typedef std::vector<Team*>			VP_TEAM;

//	STATIC VARIABLES

class Phase
{
	private:
	
		STRING						_name;
		int							_nbSetsToPlay;
		bool						_isFinished;
		VP_MATCH					_matches;

	public:

		//	CANONICAL
		Phase() = delete;
		Phase(const STRING& name, int nbSets);
		Phase(const Phase& ) = delete;
		Phase&						operator=(const Phase& ) = delete;
		~Phase();

		// GETTER
		[[nodiscard]]
		const STRING&				getName() const;
		[[nodiscard]]
		const VP_MATCH&				getMatches() const;
		[[nodiscard]]
		VP_TEAM						getWinners() const;

		//	METHOD
		void						addEncounter(Team* a, Team* b);
		[[nodiscard]]
		bool						isFinished() const;
};

#endif