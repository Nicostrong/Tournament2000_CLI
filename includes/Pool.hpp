//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef POOL_HPP
# define POOL_HPP

//	STDLIB
# include <string>
# include <vector>

//	INCLUDES
# include "./Match.hpp"
#include "Phase.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef std::vector<Match*>			V_MATCH;
typedef std::vector<Team*>			V_TEAM;

class Pool
{
	private:

		static int					_idCounter;

		STRING						_name;

		V_TEAM						_teams;
		V_MATCH						_matches;

		Pool(const STRING& );
		Pool(const Pool& );

		Pool&						operator=(const Pool& );

	public:

		Pool();
		~Pool();

		//	GETTER
		const STRING&				getName() const;
		const V_TEAM&				getTeams() const;
		const V_MATCH&				getMatches() const;

		//	METHOD
		void						addTeam(Team* team);
		void						generateMatches(int nbSetsPerEncounter);
		void						sortTeams();

		V_TEAM						getQualifiers() const;
};

#endif
