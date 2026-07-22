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
# include "./Team.hpp"
# include "./Phase.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VP_MATCH	=	std::vector<Match*>;
using				VP_TEAM		=	std::vector<Team*>;
using				CVP_MATCH	=	const std::vector<Match*>&;
using               CVP_TEAM	=	const std::vector<Team*>&;

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
		C_STRING					getName() const;
		[[nodiscard]]
		CVP_TEAM					getTeams() const;
		[[nodiscard]]
		CVP_MATCH					getMatches() const;

		//	METHOD
		void						addTeam(Team* team);
		void						generateMatches(int nbSetsPerEncounter);
		void						sortTeams();
		bool						allMatchesFinished() const;

		[[nodiscard]]
		VP_TEAM						getQualifiers() const;
};

#endif
