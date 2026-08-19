//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef TOURNAMENTVIEWER_HPP
# define TOURNAMENTVIEWER_HPP

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

/**
 *	La classe Exporter gere toutes les exportations du programme
 */
class TournamentViewer
{
	private:

		static String				getTeamNameOrPlaceholder(cpPhase phase, size_t matchIdx, cInt teamPos);

	public:

		static void					displayFullBracket(cTour tournament);
		static void					displayPodium(cTour tournament);

};

#endif