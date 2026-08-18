//
// Created by Nicolas Fordoxcel on 24/06/2026.
//

#ifndef PHASE_HPP
# define PHASE_HPP

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

//	STATIC VARIABLES

class Phase
{
	private:
	
		string						_name;
		int							_nbSetsToPlay;
		bool						_isFinished;
		vpMatch						_matches;

	public:

		//	CANONICAL
		Phase() = delete;
		Phase(string name, int nbSets);
		Phase(cPhase ) = delete;
		Phase&						operator=(cPhase ) = delete;
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
		void						setIsFinished(bool isFinished);

		//	METHOD
		void						addEncounter(Team* a, Team* b);
		[[nodiscard]]
		bool						isFinished() const;
};

#endif