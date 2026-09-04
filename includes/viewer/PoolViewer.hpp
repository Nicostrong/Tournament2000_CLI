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
class				Tournament;

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

using				pTour			=	Tournament*;
using				cTour			=	const Tournament&;
using				cpTour			=	const Tournament*;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				PoolViewer
{
	private:

		static void					writeSimpleTable(std::ostream& out, cPool pool, cBool toFile);
		static void					writeCompleteTable(std::ostream& out, cPool pool, cBool toFile);

	public:

		static void					showPoolStanding(cPool pool);
		static void					showDetailsPoolStanding(cPool pool);
		static void					showPoolsListWithStatus(cTour tournament);

		static void					printAll(Tournament& tournament);

};
