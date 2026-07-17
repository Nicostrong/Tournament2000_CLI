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
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef const std::vector<Match*>&	CVP_MATCH;

class TournamentCLI
{
	public:

		static void					displayMenu(Tournament& tournament);
		static void					displayFullBracket(Tournament& tournament);

	private:

		static void					handlePhase(Phase* phase, C_STRING phaseName);
		static void					handleMatchList(CVP_MATCH matches, C_STRING title);
		static void					clearInput();
		static STRING				getTeamNameOrPlaceholder(Phase* phase, size_t matchIdx, int teamSide);
};

#endif
