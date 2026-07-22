//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

#ifndef TOURNAMENTCLI_HPP
# define TOURNAMENTCLI_HPP

//	STDLIB
# include <string>
# include <vector>

//	INCLUDES
# include "../class/Tournament.hpp"
# include "../class/Match.hpp"
# include "../class/Phase.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				CVP_MATCH	=	const std::vector<Match*>&;

class TournamentCLI
{
	public:

		static void					displayMenu(Tournament& tournament);
		static void					displayFullBracket(Tournament& tournament);
		static void					displayPodium(const Tournament& tournament);
		static bool					exportPhaseToTxt(const Phase* phase, C_STRING filename);
		static bool					exportTournamentToTxt(const Tournament& tournament, C_STRING filename);

	private:

		static STRING				getTeamNameOrPlaceholder(const Phase* phase, size_t matchIdx, int teamPos);
		static void					handlePhase(const Phase* phase, C_STRING phaseName);
		static void					handleMatchList(CVP_MATCH matches, C_STRING title);
		static void					clearInput();
		static void					writeHeader(std::ofstream& out, const Tournament& tournament);
		static void					writePools(std::ofstream& out, const Tournament& tournament);
		static void					writePhaseBlock(std::ofstream& out, const Phase* phase);
		static void					writePalmares(std::ofstream& out, const Tournament& tournament);
};

#endif
