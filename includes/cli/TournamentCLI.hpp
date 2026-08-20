//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>
# include <csignal>
# include <functional>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Match;
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
using				cvInt			=	const std::vector<int>;

using				cBool			=	const bool;

using				pMatch			=	Match*;
using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

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

extern volatile std::sig_atomic_t	g_running;

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
		static void					displayMenuUI(cTour tournament);
		static void					handleTitle();
		static void					menuTournament(cTour tournament);

		//	Handlers de saisie
		static void					clearInput();
		static String				fetchInput();
		static int					parseChoice(cString input);
		static void					executeChoice(cInt choice, Tournament& tournament);
		static String				promptFilename(cString prompt);
		static void					handlePoolSelection(Tournament& tournament);
		static void					handleEliminationPhase(pPhase phase, const std::function<void()>& generateFn,
										cString phaseName, cString successMsg, cString errorMsg);
		static void					handleExport(Tournament& tournament);
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
		static void					handlePhase(pPhase phase, cString phaseName);
		
};