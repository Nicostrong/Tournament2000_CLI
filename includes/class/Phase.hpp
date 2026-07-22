//
// Created by Nicolas Fordoxcel on 24/06/2026.
//

#ifndef PHASE_HPP
# define PHASE_HPP

//	STDLIB
# include <string>
# include <vector>

//	INCLUDES
# include "./Match.hpp"
# include "./Team.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VP_MATCH	=	std::vector<Match*>;
using				VP_TEAM		=	std::vector<Team*>;

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
		Phase(STRING name, int nbSets);
		Phase(const Phase& ) = delete;
		Phase&						operator=(const Phase& ) = delete;
		~Phase();

		// GETTER
		[[nodiscard]]
		const STRING&				getName() const;
		[[nodiscard]]
		int							getNbSetToPlay() const;
		[[nodiscard]]
		bool						getIsFinished() const;
		[[nodiscard]]
		const VP_MATCH&				getMatches() const;
		[[nodiscard]]
		VP_TEAM						getWinners() const;
		[[nodiscard]]
		VP_TEAM						getLosers() const;

		//	SETTER
		void						setIsFinished(bool isFinished);

		//	METHOD
		void						addEncounter(Team* a, Team* b);
		[[nodiscard]]
		bool						isFinished() const;
};

#endif