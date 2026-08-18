//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef MATCH_HPP
# define MATCH_HPP

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
using				cvpPart		=	const std::vector<Participant*>&;ng				cpvPool		=	const std::vector<Pool*>;

//	STATIC VARIABLES

/**
 * la classe Match represente un set entre deux Teams
 */
class Match
{
	private:

		Team*						_teamA;
		Team*						_teamB;
		int							_scoreA;
		int							_scoreB;
		bool						_isFinished;

	public:

		//	CANONICAL
		Match() = delete;
		Match(Team* a, Team* b);
		Match(cMatch match) = delete;
		Match&						operator=(cMatch match) = delete;
		~Match() = default;

		//	GETTER
		[[nodiscard]]
		Team*						getTeamA() const;
		[[nodiscard]]
		Team*						getTeamB() const;
		[[nodiscard]]
		int							getScoreA() const;
		[[nodiscard]]
		int							getScoreB() const;
		[[nodiscard]]
		bool						isFinished() const;

		//	SETTER
		void						setScore(int sA, int sB);

		//	METHOD
		[[nodiscard]]
		Team*						getWinner() const;
		[[nodiscard]]
		Team*						getLoser() const;
};

#endif
