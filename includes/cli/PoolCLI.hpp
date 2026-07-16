//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

#ifndef POOLCLI_HPP
# define POOLCLI_HPP

//	STDLIB

//	INCLUDES
# include "../class/Pool.hpp"

//	TYPEDEF

//	STATIC VARIABLES

class PoolCLI
{
	public:

		static void					displayTable(const Pool& pool);
		static void					displayMatches(const Pool& pool);
		static void					displayPoolDetails(const Pool& pool);
};

#endif
