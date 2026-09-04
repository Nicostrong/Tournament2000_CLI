//
// Created by Nicolas Fordoxcel on 21/08/2026.
//
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
;
class				Pool;
class				Match;
class				Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cBool			=	const bool;

using				pMatch			=	Match*;
using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

using				pPool			=	Pool*;
using				cPool			=	const Pool&;
using				cpPool			=	const Pool*;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/


class				MatchViewer
{
	public:

		static void					showMatchTitle(cMatch match);
		static void					showExtendedTableOfAllMatchesInPool(cvpMatch matches);
		static void					showDetailsTableOfAllMatchesInPool(cPool pool);
		static void					showAllMatchesWithStatusInPool(cPool pool);

		static void					printAll(Tournament& tournament);

};

