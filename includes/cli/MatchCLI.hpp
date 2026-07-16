//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#ifndef MATCHCLI_HPP
# define MATCHCLI_HPP

//	STDLIB

//	INCLUDES
# include "../class/Match.hpp"

//	TYPEDEF

//	STATIC VARIABLES

class MatchCLI
{
	public:

		static void					display(const Match& match);
		static void					inputScore(Match& match);
};

#endif
