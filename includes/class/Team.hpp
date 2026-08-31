//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Player;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				pPlayer			=	Player*;
using				vpPlayer		=	std::vector<Player*>;
using				cvpPlayer		=	const std::vector<Player*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 * la classe Team represente un enssemble de Player qui joueront enssemble
 */
class				Team
{
	private:

		static int					_idCounter;

		int							_id;
		int							_point;
		int							_scoreMarked;
		int							_scoreAgainst;
		bool						_isMixed;
		bool						_isEliminated;
		bool						_isDisqualified;
		bool						_hasMultiTeamPlayer;
		String						_name;
		vpPlayer					_members;

	public:

		Team();
		explicit Team(cString ) = delete;
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
		bool						getIsDisqualified() const;
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
		cString						getName() const;
		[[nodiscard]]
		cvpPlayer					getMembers() const;

		//	SETTER
		void						setIsMixed(bool value);
		void						setIsEliminated(bool value);
		void						setIsDisqualified(bool value);
		void						setHasMultiTeamPlayer(bool value);
		void						setName(String value);

		//	METHOD
		[[nodiscard]]
		bool						isComplete(int requiredSize) const;
		void						addMember(Player* member);
		void						renameTeam();
		void						addPoint(int point);
		void						addScoreMarked(int score);
		void						addScoreAgainst(int score);
		bool						hasMember(pPlayer p) const;
		bool						sharesMemberWith(const Team* other) const;
		bool						replaceMember(size_t index, Player* newMember);
		void						disqualifyTeam(bool action);

};
