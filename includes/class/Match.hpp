//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef MATCH_HPP
# define MATCH_HPP

//	STDLIB

//	INCLUDES
# include "./Team.hpp"

//	TYPEDEF

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
		Match(const Match& match) = delete;
		Match&						operator=(const Match& match) = delete;
		~Match();

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
