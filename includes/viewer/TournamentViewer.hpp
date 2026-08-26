//
// Created by Nicolas Fordoxcel on 15/07/2026.
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

class				Phase;
class				Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;

using				cInt			=	const int;
using				vInt			=	std::vector<int>;
using				cvInt			=	const std::vector<int>&;

using				cBool			=	const bool;

using				pPhase			=	Phase*;
using				cPhase			=	const Phase&;
using				cpPhase			=	const Phase*;
using				vpPhase			=	std::vector<Phase*>;
using				cvpPhase		=	const std::vector<Phase*>&;

using				pTour			=	Tournament*;
using				cTour			=	const Tournament&;
using				cpTour			=	const Tournament*;


/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 *	La classe Exporter gere toutes les exportations du programme
 */
class				TournamentViewer
{
	private:

		static String				getTeamNameOrPlaceholder(cpPhase phase, size_t matchIdx, cInt teamPos);

	public:

		static void					displayFullBracket(cTour tournament);
		static void					displayPodium(cTour tournament);

};
