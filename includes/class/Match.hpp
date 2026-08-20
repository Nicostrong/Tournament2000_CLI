//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <vector>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Team;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/
using				cInt			=	const int;

using				pTeam			=	Team*;
using				cTeam			=	const Team&;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;

/****************************************************************************************************/
/*	CLASSE																							*/
/****************************************************************************************************/

/**
 * la classe Match represente un set entre deux Teams
 */
class				Match
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
		Match(const Match& match) = delete;
		Match&						operator=(const Match& match) = delete;
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
		void						setScore(cInt sA, cInt sB);

		//	METHOD
		[[nodiscard]]
		Team*						getWinner() const;
		[[nodiscard]]
		Team*						getLoser() const;
};
