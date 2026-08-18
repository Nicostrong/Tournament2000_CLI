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

	private:

		//	Affichage du menu
		static void					displayMenuUI(const Tournament& tournament);
		static void					handleTitle();
		static void					menuTournament(const Tournament& tournament);

		//	Handlers de saisie
		static void					clearInput();
		static STRING				fetchInput();
		static int					parseChoice(C_STRING input);
		static void					executeChoice(int choice, Tournament& tournament);
		static STRING				promptFilename(C_STRING prompt);
		static void					handlePoolSelection(Tournament& tournament);
		static void					handleEliminationPhase(Phase* phase,
														   const std::function<void()>& generateFn,
														   C_STRING phaseName,
														   C_STRING successMsg,
														   C_STRING errorMsg);
		static void					handleExport(Tournament& tournament);
		static const Phase*			getPhaseByMenuChoice(const Tournament& tournament, int choice);

		//	Verification de phase
		static bool					isPoolsFinished(const Tournament& tournament);
		static bool					isSixteenthUnlocked(const Tournament& tournament);
		static bool					isHeighthUnlocked(const Tournament& tournament);
		static bool					isQuartersUnlocked(const Tournament& tournament);
		static bool					isSemisUnlocked(const Tournament& tournament);
		static bool					isFinalUnlocked(const Tournament& tournament);
		static bool					isThirdUnlocked(const Tournament& tournament);
		
		//	Helpers d affichage
		static void					handleMatchList(CVP_MATCH matches, C_STRING title);
		static void					handlePhase(Phase* phase, C_STRING phaseName);
		
};

#endif
