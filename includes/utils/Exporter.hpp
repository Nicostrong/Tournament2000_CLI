//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef EXPORTER_HPP
# define EXPORTER_HPP

//	STDLIB
# include <string>
# include <vector>

//	INCLUDES
# include "../class/Tournament.hpp"
# include "../class/Participant.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				CVP_PART	=	const std::vector<Participant*>& ;
using				CVP_MATCH	=	const std::vector<Match*>&;

//	GLOBAL VARIABLES

/**
 *	La classe Exporter gere toutes les exportations du programme
 */
class Exporter
{
	public:

		// Exports TXT
		static bool					exportTournamentToTxt(const Tournament& tournament, C_STRING filename);
		static bool					exportPhaseToTxt(const Phase* phase, C_STRING filename);
		static bool					exportPoolsToTxt(const Tournament& tournament, C_STRING filename);

		// Exports CSV
		static bool					exportParticipantsToCSV(CVP_PART participants, C_STRING filename);

		// Exports JSON
		static bool					exportTournamentToJson(const Tournament& tournament, C_STRING filename);

	private:

		// Helpers
		static void					writeHeader(std::ofstream& out, const Tournament& tournament);
		static void					writePools(std::ofstream& out, const Tournament& tournament);
		static void					writePoolMatches(std::ofstream& out, const Pool& pool);
		static void					writePoolStandings(std::ofstream& out, const Pool& pool);
		static void					writeEncounterBlock(std::ofstream& out, CVP_MATCH matches,
														size_t startIdx, int nbSets, int encounterNum);
		static void					writePhaseBlock(std::ofstream& out, const Phase* phase);
		static void					writePhaseResults(std::ofstream& out, const Phase& phase);
		static void					writePalmares(std::ofstream& out, const Tournament& tournament);
};

#endif