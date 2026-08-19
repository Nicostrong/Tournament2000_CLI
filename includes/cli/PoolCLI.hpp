//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

#ifndef POOLCLI_HPP
# define POOLCLI_HPP

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include "../Global.hpp"

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class PoolCLI
{
	private:
		
		static void					writeMatches(std::ostream& out, cPool pool, bool toFile);
		static void					writeTable(std::ostream& out, cPool pool, bool toFile);

	public:

		static void					displayTable(cPool pool);
		static void					displayFullTable(cPool pool);
		static void					displayMatches(cPool pool);
		static void					displayPoolDetails(cPool pool);
};

#endif
