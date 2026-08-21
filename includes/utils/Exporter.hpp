//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Pool;
class				Match;
class				Phase;
class				Tournament;
class				Participant;

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

using				pPart			=	Participant*;
using				cPart			=	const Participant&;
using				cpPart			=	const Participant*;
using				vpPart			=	std::vector<Participant*>;
using				cvpPart			=	const std::vector<Participant*>&;

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
class				Exporter
{
	public:

		// Exports TXT
		static bool					exportTournamentToTxt(cTour tournament, cString filename);
		static bool					exportPhaseToTxt(cpPhase phase, cString filename);
		static bool					exportPoolsToTxt(cTour tournament, cString filename);
		static bool					exportToTxt(cPool pool, cString filename);

		// Exports CSV
		static bool					exportParticipantsToCSV(cvpPart participants, cString filename);

		// Exports JSON
		//static bool					exportTournamentToJson(cTour tournament, cString filename);

	private:

		// Helpers
		static void					writeHeader(std::ofstream& out, cTour tournament);
		static void					writePools(std::ofstream& out, cTour tournament);
		static void					writePoolMatches(std::ofstream& out, cPool pool);
		static void					writePoolStandings(std::ofstream& out, cPool pool);
		static void					writeEncounterBlock(std::ofstream& out, cvpMatch matches,
										size_t startIdx, int nbSets, int encounterNum);
		static void					writePhaseBlock(std::ofstream& out, const Phase* phase);
		static void					writePhaseResults(std::ofstream& out, cPhase phase);
		static void					writePalmares(std::ofstream& out, cTour tournament);
		
		//	POOL
		static void					writeMatches(std::ostream& out, cPool pool, bool toFile);
		static void					writeTable(std::ostream& out, cPool pool, bool toFile);

};