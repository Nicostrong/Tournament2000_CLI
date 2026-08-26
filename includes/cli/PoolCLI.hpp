//
// Created by Nicolas Fordoxcel on 15/06/2026.
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

class				Pool;
class				Team;
class				Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				pPool			=	Pool*;
using				cPool			=	const Pool&;
using				cpPool			=	const Pool*;
using				vpPool			=	std::vector<Pool*>;
using				cvpPool			=	const std::vector<Pool*>&;

using				pTeam			=	Team*;
using				cTeam			=	const Team&;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;

using				pTour			=	Tournament*;
using				cTour			=	const Tournament&;
using				cpTour			=	const Tournament*;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				PoolCLI
{
	private:

		static void					displayMenuUI(cTour tournament);
		static void					handleTitle();
		static void					menuPool(pPool pool);
		static void					submenuPool(pPool pool, Tournament& tournament);

		static void					clearInput();
		static String				fetchInput();
		static int					parseChoice(cString input);
		static void					executeChoice(cInt choice, pPool pool, Tournament& tournament);

		static void					manageSinglePool(cPool pool);

		static bool					checkTeamId(int id, Tournament& tournament);

	public:

		static void					handleMenuPool(Tournament& tournament);

};
