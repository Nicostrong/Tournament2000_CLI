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
using				cvInt			=	const std::vector<int>&;

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
		static void					menuTournament(cTour tournament);

		//	Handlers de saisie
		static void					executeChoice(cInt choice, Tournament& tournament);

		static void					handleEliminationPhase(pPhase phase, const std::function<void()>& generateFn,
										cString phaseName, cString successMsg, cString errorMsg);
		static void					handleExport(Tournament& tournament);
		static cpPhase				getPhaseByMenuChoice(cTour tournament, cInt choice);
		
		//	Helpers d affichage
		//static void					handleMatchList(cvpMatch matches, cString title);
		static void					handlePhase(pPhase phase, cString phaseName);
		
};