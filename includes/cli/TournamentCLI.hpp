//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

#ifndef TOURNAMENTCLI_HPP
# define TOURNAMENTCLI_HPP

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <functional>

#include "../Global.hpp"

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class TournamentCLI
{
	public:

		//	Menu principal
		static void					handleMenuTournament(Tournament& tournament);

	private:

		//	Affichage du menu
		static void					displayMenuUI(cTour tournament);
		static void					handleTitle();
		static void					menuTournament(cTour tournament);

		//	Handlers de saisie
		static void					clearInput();
		static String				fetchInput();
		static int					parseChoice(cString input);
		static void					executeChoice(cInt choice, cTour tournament);
		static String				promptFilename(cString prompt);
		static void					handlePoolSelection(cTour tournament);
		static void					handleEliminationPhase(cpPhase phase, const std::function<void()>& generateFn,
										cString phaseName, cString successMsg, cString errorMsg);
		static void					handleExport(cTour tournament);
		static cpPhase				getPhaseByMenuChoice(cTour tournament, cInt choice);

		//	Verification de phase
		static bool					isPoolsFinished(cTour tournament);
		static bool					isSixteenthUnlocked(cTour tournament);
		static bool					isHeighthUnlocked(cTour tournament);
		static bool					isQuartersUnlocked(cTour tournament);
		static bool					isSemisUnlocked(cTour tournament);
		static bool					isFinalUnlocked(cTour tournament);
		static bool					isThirdUnlocked(cTour tournament);
		
		//	Helpers d affichage
		static void					handleMatchList(cvpMatch matches, cString title);
		static void					handlePhase(cpPhase phase, cString phaseName);
		
};

#endif
