//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>
# include <fstream>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Pool;
class				Team;
class				Match;
class				Phase;
class				Player;
class				Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				pPlayer			=	Player*;
using				vpPlayer		=	std::vector<Player*>;
using				cvpPlayer		=	const std::vector<Player*>&;

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

using				pPlayer			=	Player*;
using				cPlayer			=	const Player&;
using				cpPlayer			=	const Player*;
using				vpPlayer			=	std::vector<Player*>;
using				cvpPlayer			=	const std::vector<Player*>&;

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

		// --- Players ---
		static bool exportPlayersToCSV(const std::vector<Player*>& players, const std::string& filename);
		static bool exportPlayersToTxt(const std::vector<Player*>& players, const std::string& filename);

		// --- Teams ---
		static bool exportTeamsToTxt(const std::vector<Team*>& teams, const std::string& filename);
		static bool exportTeamsToTxt(const Tournament& tournament, const std::string& filename);

		// --- Matches ---
		static bool exportMatchesToTxt(const std::vector<Match*>& matches, const std::string& filename, const std::string& title = "MATCHES");

		// --- Phase ---
		static bool exportPhaseToTxt(const Phase* phase, const std::string& filename);

		// --- Pool ---
		static bool exportPoolToTxt(const Pool* pool, const std::string& filename);
		static bool exportPoolsToTxt(const std::vector<Pool*>& pools, const std::string& filename);
		static bool exportPoolsToTxt(const Tournament& tournament, const std::string& filename);

		// --- Tournament ---
		static bool exportTournamentToTxt(const Tournament& tournament, const std::string& filename);

		/*static bool					exportTournamentToTxt(cTour tournament, cString filename);
		static bool					exportPhaseToTxt(pPhase phase, cString filename);
		static bool					exportPoolsToTxt(cTour tournament, cString filename);
		static bool					exportToTxt(cPool pool, cString filename);

		static bool					exportPlayersToCSV(cvpPlayer participants, cString filename);

		static bool					exportTournamentToJson(cTour tournament, cString filename);

	private:

		static void					writeHeader(std::ofstream& out, cTour tournament);
		static void					writePools(std::ofstream& out, cTour tournament);
		static void					writePoolMatches(std::ofstream& out, cPool pool);
		static void					writePoolStandings(std::ofstream& out, cPool pool);
		static void					writeEncounterBlock(std::ofstream& out, cvpMatch matches,
										size_t startIdx, int nbSets, int encounterNum);
		static void					writePhaseBlock(std::ofstream& out, const Phase* phase);
		static void					writePhaseResults(std::ofstream& out, cPhase phase);
		static void					writePalmares(std::ofstream& out, cTour tournament);

		static void					writeTable(std::ostream& out, cPool pool, bool toFile);*/

};