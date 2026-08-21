//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <fstream>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Pool;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				pPool			=	Pool*;
using				cPool			=	const Pool&;
using				cpPool			=	const Pool*;
using				vpPool			=	std::vector<Pool*>;
using				cvpPool			=	const std::vector<Pool*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				PoolViewer
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
