//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>
# include <functional>

# include "./CLIUtils.hpp"

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

using				cInt			=	const int;

using				pPhase			=	Phase*;
using				cpPhase			=	const Phase*;

using				cTour			=	const Tournament&;

using				vMenuItem		=	std::vector<MenuItem>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				TournamentCLI
{
	public:

		//	Menu principal
		static void					handleMenuTournament(Tournament& tournament);

	private:

		//	Affichage du menu
		static vMenuItem			displayMenuUI(cTour tournament);
		static vMenuItem			generateMenuTournament(cTour tournament);

		//	Handlers de saisie
		static void					executeChoice(cInt choice, Tournament& tournament);

		static void					handleEliminationPhase(pPhase phase, std::function<bool()> generateFn,
										cString phaseName, cString successMsg, cString errorMsg);
		
};