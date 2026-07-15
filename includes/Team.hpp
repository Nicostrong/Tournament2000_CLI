//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef TEAM_HPP
# define TEAM_HPP

//	STDLIB
# include <string>
# include <vector>

//	INCLUDES
# include "./Participant.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef std::vector<Participant*>	V_PART;

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

		bool						_isEliminated;
		bool						_hasMultiTeamPlayer;

		STRING						_name;

		V_PART						_members;

		Team(const STRING& );
		Team(const Team& );

		Team&						operator=(const Team& );

	public:

		Team();
		~Team();

		//	GETTER
		size_t						getSize() const;
		bool						getIsEliminated() const;
		bool						getHasMultiTeamPlayer() const;
		int							getId() const;
		int							getPoint() const;
		int							getScoreMarked() const;
		int							getScoreAgainst() const;
		int							getScoreDiff() const;
		C_STRING					getName() const;
		const V_PART&				getMembers() const;

		//	SETTER
		void						setIsEliminated(bool value);
		void						setHasMultiTeamPlayer(bool value);
		void						setName(C_STRING value);

		//	METHOD
		bool						isComplete(int requiredSize) const;
		void						addMember(Participant* member);
		void						addPoint(int point);
		void						addScoreMarked(int score);
		void						addScoreAgainst(int score);
};

#endif
