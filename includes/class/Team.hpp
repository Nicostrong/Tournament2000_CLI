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

class				Participant;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

using				pPart			=	Participant*;
using				cPart			=	const Participant&;
using				cpPart			=	const Participant*;
using				vpPart			=	std::vector<Participant*>;
using				cvpPart			=	const std::vector<Participant*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 * la classe Team represente un enssemble de Participant qui joueront enssemble
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
		vpPart						_members;

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
		cvpPart						getMembers() const;

		//	SETTER
		void						setIsMixed(cBool value);
		void						setIsEliminated(cBool value);
		void						setIsDisqualified(cBool value);
		void						setHasMultiTeamPlayer(cBool value);
		void						setName(cString value);

		//	METHOD
		[[nodiscard]]
		bool						isComplete(cInt requiredSize) const;
		void						addMember(Participant* member);
		void						renameTeam();
		void						addPoint(cInt point);
		void						addScoreMarked(cInt score);
		void						addScoreAgainst(cInt score);
		bool						hasMember(cpPart p) const;
		bool						sharesMemberWith(const Team* other) const;
		bool						replaceMember(size_t index, Participant* newMember);
		void						disqualifyTeam(bool action);

};
