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
# include "./Phase.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef std::vector<Match*>			VP_MATCH;
typedef std::vector<Team*>			VP_TEAM;

class Pool
{
	private:

		static int					_idCounter;
		STRING						_name;
		VP_TEAM						_teams;
		VP_MATCH					_matches;

	public:

		//	CANONICAL
		Pool();
		explicit Pool(C_STRING ) = delete;
		Pool(const Pool& ) = delete;
		Pool&						operator=(const Pool& ) = delete;
		~Pool();

		//	GETTER
		[[nodiscard]]
		const STRING&				getName() const;
		[[nodiscard]]
		const VP_TEAM&				getTeams() const;
		[[nodiscard]]
		const VP_MATCH&				getMatches() const;

		//	METHOD
		void						addTeam(Team* team);
		void						generateMatches(int nbSetsPerEncounter);
		void						sortTeams();

		[[nodiscard]]
		VP_TEAM						getQualifiers() const;
};

#endif
