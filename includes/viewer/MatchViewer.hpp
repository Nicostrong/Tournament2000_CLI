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

# include <string>
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

using				String			=	std::string;
using				cString			=	const std::string&;

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
		static void					showExtendedTableOfAllMatches(cvpMatch matches);
		static void					showDetailsTableOfAllMatches(cvpMatch matches, cString titleOfStage);
		static void					showAllMatchesWithStatus(cvpMatch matches);

		static void					printAll(Tournament& tournament);

};

