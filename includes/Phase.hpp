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
typedef std::vector<Match*>			V_MATCH;
typedef std::vector<Team*>			V_TEAM;

//	STATIC VARIABLES

class Phase
{
	private:
	
		STRING						_name;

		int							_nbSetsToPlay;

		bool						_isFinished;

		V_MATCH						_matches;

		Phase();
		Phase(const Phase& );

		Phase&						operator=(const Phase& );

	public:

		Phase(const STRING& name, int nbSets);
		~Phase();
		
		// GETTER
		const STRING&				getName() const;
		const V_MATCH&				getMatches() const;
		V_TEAM						getWinners() const;

		//	METHOD
		void						addEncounter(Team* a, Team* b);
		bool						isFinished() const;
};

#endif