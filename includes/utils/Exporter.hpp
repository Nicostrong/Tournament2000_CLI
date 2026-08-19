//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef EXPORTER_HPP
# define EXPORTER_HPP

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
class Exporter
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
		static bool					exportTournamentToJson(cTour tournament, cString filename);

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

#endif