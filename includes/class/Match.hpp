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

using				pTeam			=	Team*;

/****************************************************************************************************/
/*	STRUCT																							*/
/****************************************************************************************************/

struct				ScoreRules
{
	int				scoreToWin;
	int				scoreMaxToWin;
	int				diffPointsToWin;
};

/****************************************************************************************************/
/*	CLASSE																							*/
/****************************************************************************************************/

/**
 * la classe Match represente un set entre deux Teams
 */
class				Match
{
	private:

		pTeam						_teamA;
		pTeam						_teamB;
		int							_scoreA;
		int							_scoreB;
		bool						_isFinished;
		ScoreRules					_rules;

		[[nodiscard]]
		bool						isDraw() const;
		[[nodiscard]]
		bool						checkScore(int sA, int sB) const;
		void						applyStats(int sA, int sB, int multiplier) const;

	public:

		Match() = delete;
		Match(pTeam a, pTeam b, ScoreRules rules);

		Match(const Match&) = delete;
		Match& operator=(const Match&) = delete;

		Match(Match&&) = delete;
		Match& operator=(Match&&) = delete;

		~Match() = default;

		//	GETTER
		[[nodiscard]]
		pTeam						getTeamA() const;
		[[nodiscard]]
		pTeam						getTeamB() const;
		[[nodiscard]]
		int							getScoreA() const;
		[[nodiscard]]
		int							getScoreB() const;
		[[nodiscard]]
		bool						isFinished() const;

		//	SETTER
		void						setScoreA(int value);
		void						setScoreB(int value);
		void						setTeamA(pTeam value);
		void						setTeamB(pTeam value);
		void						setIsFinished(bool value);

		//	METHOD
		[[nodiscard]]
		pTeam						getWinner() const;
		[[nodiscard]]
		pTeam						getLoser() const;
		void						modifyScore(int sA, int sB);
		bool						setScore(int sA, int sB);
		void						resetScore();

};
