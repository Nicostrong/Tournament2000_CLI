//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

#ifndef TOURNAMENTCLI_HPP
# define TOURNAMENTCLI_HPP

//	STDLIB
# include <string>
# include <vector>
# include <functional>

//	INCLUDES
# include "../class/Tournament.hpp"
# include "../class/Match.hpp"
# include "../class/Phase.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VP_MATCH	=	std::vector<Match*>;
using				CVP_MATCH	=	const std::vector<Match*>&;
using				CVP_POOL	=	const std::vector<Pool*>;

class TournamentCLI
{
	public:

		//	Menu principal
		static void					handleMenuTournament(Tournament& tournament);

		//	Affichage terminal
		static void					displayFullBracket(Tournament& tournament);
		static void					displayPodium(const Tournament& tournament);

		//	Export fichier
		static bool					exportPhaseToTxt(const Phase* phase, C_STRING filename);
		static bool					exportTournamentToTxt(const Tournament& tournament, C_STRING filename);

	private:

		//	Affichage du menu
		static void					menuTournament(const Tournament& tournament);
		static void					handleTitle();

		//	Verification de phase
		static bool					isPoolsFinished(const Tournament& tournament);
		static bool					isSixteenthUnlocked(const Tournament& tournament);
		static bool					isHeighthUnlocked(const Tournament& tournament);
		static bool					isQuartersUnlocked(const Tournament& tournament);
		static bool					isSemisUnlocked(const Tournament& tournament);
		static bool					isFinalUnlocked(const Tournament& tournament);
		
		//	Handlers de saisie
		static STRING				promptFilename(C_STRING prompt);
		static void					handlePoolSelection(Tournament& tournament);
		static void					handleEliminationPhase(Phase* phase,
														   const std::function<void()>& generateFn,
														   C_STRING phaseName,
														   C_STRING successMsg,
														   C_STRING errorMsg);
		static void					handleExportPhase(Tournament& tournament);
		static const Phase*			getPhaseByMenuChoice(const Tournament& tournament, int choice);
		static bool					exportPoolsToFile(const Tournament& tournament, C_STRING filename);

		//	Helpers d affichage
		static void					handleMatchList(CVP_MATCH matches, C_STRING title);
		static void					handlePhase(Phase* phase, C_STRING phaseName);
		static STRING				getTeamNameOrPlaceholder(const Phase* phase, size_t matchIdx, int teamPos);
		static void					clearInput();

		//	Helpers d ecriture des fichiers
		static void					writeHeader(std::ofstream& out, const Tournament& tournament);
		static void					writePools(std::ofstream& out, const Tournament& tournament);
		static void					writePhaseBlock(std::ofstream& out, const Phase* phase);
		static void					writePalmares(std::ofstream& out, const Tournament& tournament);
		static void					writePoolMatches(std::ofstream& out, const Pool& pool);
		static void					writePoolStandings(std::ofstream& out, const Pool& pool);
		static void					writeEncounterBlock(std::ofstream& out, CVP_MATCH matches,
														 size_t startIdx, int nbSets, int encounterNum);
		static void					writePhaseResults(std::ofstream& out, const Phase& phase);
};

#endif
