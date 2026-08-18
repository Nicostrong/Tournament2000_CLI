//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef POOL_HPP
# define POOL_HPP

//	STDLIB
# include <string>
# include <vector>

//	INCLUDES
# include "./Pool.hpp"
# include "./Team.hpp"
# include "./Match.hpp"
# include "./Phase.hpp"
# include "./Settings.hpp"
# include "./Tournament.hpp"
# include "./Participant.hpp"

//	TYPEDEF
using				string		=	std::string;
using				cString		=	const std::string&;

using				cPool		=	const Pool&;
using				cTeam		=	const Team&;
using				cMatch		=	const Match&;
using				cPhase		=	const Phase&;
using				cSet		=	const Settings&;
using				cPart		=	const Participant&;

using				vpPool		=	std::vector<Pool*>;
using				vpTeam		=	std::vector<Team*>;
using				vpMatch		=	std::vector<Match*>;
using				vpPhase		=	std::vector<Phase*>;
using				vpPart		=	std::vector<Participant*>;

using				cvpPool		=	const std::vector<Pool*>&;
using				cvpTeam		=	const std::vector<Team*>&;
using				cvpMatch	=	const std::vector<Match*>&;
using				cvpPhase	=	const std::vector<Phase*>&;
using				cvpPart		=	const std::vector<Participant*>&;

class Pool
{
	private:

		static int					_idCounter;
		string						_name;
		vpTeam						_teams;
		vpMatch						_matches;

	public:

		//	CANONICAL
		Pool();
		explicit Pool(cString ) = delete;
		Pool(cPool ) = delete;
		Pool&						operator=(cPool ) = delete;
		~Pool();

		//	GETTER
		[[nodiscard]]
		cString						getName() const;
		[[nodiscard]]
		cvpTeam						getTeams() const;
		[[nodiscard]]
		cvpMatch					getMatches() const;

		//	METHOD
		void						addTeam(Team* team);
		void						generateMatches(int nbSetsPerEncounter);
		void						sortTeams();
		bool						allMatchesFinished() const;

		[[nodiscard]]
		vpTeam						getQualifiers() const;
};

#endif
