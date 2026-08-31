//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <vector>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Pool;
class				Match;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				pMatch			=	Match*;
using				vpMatch			=	std::vector<Match*>;

using				pPool			=	Pool*;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/


class				MatchCLI
{
	private:

		static void					displayMenuUI(vpMatch matches, pPool pool);
		static void					menuMatch(pMatch match);
		static void					submenuMatch(pMatch match);

		static void					executeChoice(int choice, pMatch match);

		static void					handleSaveScore(pMatch match);
		static void					handleModifyScore(pMatch match);

		static bool					checkMatchId(int id, size_t size);

	public:

		static void					handleMenuMatch(vpMatch matches, pPool pool);

};
