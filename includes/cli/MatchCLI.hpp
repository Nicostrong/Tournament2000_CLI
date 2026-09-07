//
// Created by Nicolas Fordoxcel on 14/06/2026.
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

class				Match;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				pMatch			=	Match*;
using				vpMatch			=	std::vector<Match*>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/


class				MatchCLI
{
	private:

		static void					displayMenuUI(vpMatch matches, cString title = "");
		static void					menuMatch(pMatch match);

		static void					executeChoice(cInt choice, pMatch match);

		static void					handleSaveScore(pMatch match);
		static void					handleModifyScore(pMatch match);

		static bool					checkMatchId(int id, size_t size);

	public:

		static void					handleMenuMatch(vpMatch matches, cString title = "");
		static void					submenuMatch(pMatch match);

};
