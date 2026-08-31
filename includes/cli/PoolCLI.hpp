//
// Created by Nicolas Fordoxcel on 15/06/2026.
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
class				Team;
class				Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				pPool			=	Pool*;
using				cPool			=	const Pool&;

using				cTour			=	const Tournament&;

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
		static void					menuPool(pPool pool);
		static void					submenuPool(pPool pool, Tournament& tournament);

		static void					executeChoice(int choice, pPool pool, Tournament& tournament);

		static void					manageSinglePool(cPool pool);

		static bool					checkPoolId(int id, Tournament& tournament);

	public:

		static void					handleMenuPool(Tournament& tournament);

};
