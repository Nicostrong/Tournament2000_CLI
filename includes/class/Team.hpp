//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef TEAM_HPP
# define TEAM_HPP

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

/**
 * la classe Team represente un enssemble de Participant qui joueront enssemble
 */
class Team
{
	private:

		static int					_idCounter;

		int							_id;
		int							_point;
		int							_scoreMarked;
        int							_scoreAgainst;
		bool						_isMixed;
		bool						_isEliminated;
		bool						_hasMultiTeamPlayer;
		STRING						_name;
		VP_PART						_members;


	public:

		//	CANONICAL
		Team();
		explicit Team(C_STRING ) = delete;
		Team(const Team& ) = delete;
		Team&						operator=(const Team& ) = delete;
		~Team() = default;

		//	GETTER
		[[nodiscard]]
		size_t						getSize() const;
		[[nodiscard]]
		bool						getIsMixed() const;
		[[nodiscard]]
		bool						getIsEliminated() const;
		[[nodiscard]]
		bool						getHasMultiTeamPlayer() const;
		[[nodiscard]]
		int							getId() const;
		[[nodiscard]]
		int							getPoint() const;
		[[nodiscard]]
		int							getScoreMarked() const;
		[[nodiscard]]
		int							getScoreAgainst() const;
		[[nodiscard]]
		int							getScoreDiff() const;
		[[nodiscard]]
		C_STRING					getName() const;
		[[nodiscard]]
		CVP_PART					getMembers() const;

		//	SETTER
		void						setIsMixed(bool value);
		void						setIsEliminated(bool value);
		void						setHasMultiTeamPlayer(bool value);
		void						setName(C_STRING value);

		//	METHOD
		[[nodiscard]]
		bool						isComplete(int requiredSize) const;
		void						addMember(Participant* member);
		void						renameTeam();
		void						addPoint(int point);
		void						addScoreMarked(int score);
		void						addScoreAgainst(int score);
};

#endif
