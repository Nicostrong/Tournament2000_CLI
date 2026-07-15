//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

#ifndef TOURNAMENTCLI_HPP
# define TOURNAMENTCLI_HPP

//	STDLIB
# include <string>
# include <vector>
# include <cstddef>

//	INCLUDES
# include "./Tournament.hpp"
# include "./Match.hpp"
# include "./Phase.hpp"

//	TYPEDEF

class TournamentCLI
{
	public:

		static void									displayMenu(Tournament& tournament);
		static void									displayFullBracket(Tournament& tournament);

	private:

		static void									handlePhase(Phase* phase, const std::string& phaseName);
		static void									handleMatchList(const std::vector<Match*>& matches, const std::string& title);
		static void									clearInput();
		static std::string							getTeamNameOrPlaceholder(Phase* phase, size_t matchIdx, int teamSide);
};

#endif
