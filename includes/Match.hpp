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

		Match();
		Match(const Match& match);

		Match&						operator=(const Match& match);

	public:

		Match(Team* a, Team* b);
		~Match();

		//	GETTER
		Team*						getTeamA() const;
		Team*						getTeamB() const;
		int							getScoreA() const;
		int							getScoreB() const;
		bool						isFinished() const;

		//	SETTER
		void						setScore(int sA, int sB);

		//	METHOD
		Team*						getWinner() const;
		Team*						getLoser() const;
};

#endif
