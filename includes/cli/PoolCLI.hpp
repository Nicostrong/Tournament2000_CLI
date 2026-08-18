//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

#ifndef POOLCLI_HPP
# define POOLCLI_HPP

//	STDLIB

//	INCLUDES
# include "../class/Pool.hpp"

//	TYPEDEF
using				C_STRING	=	const std::string&;

//	STATIC VARIABLES

class PoolCLI
{
	private:
		
		static void					writeMatches(std::ostream& out, const Pool& pool, bool toFile);
		static void					writeTable(std::ostream& out, const Pool& pool, bool toFile);

	public:

		static void					displayTable(const Pool& pool);
		static void					displayFullTable(const Pool& pool);
		static void					displayMatches(const Pool& pool);
		static void					displayPoolDetails(const Pool& pool);
};

#endif
