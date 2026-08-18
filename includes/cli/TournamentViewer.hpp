//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef TOURNAMENTVIEWER_HPP
# define TOURNAMENTVIEWER_HPP

//	STDLIB
# include <string>

//	INCLUDES
# include "../class/Tournament.hpp"
# include "../class/Participant.hpp"
# include "../class/Phase.hpp"

//	TYPEDEF
using				STRING		=	std::string;

//	GLOBAL VARIABLES

/**
 *	La classe Exporter gere toutes les exportations du programme
 */
class TournamentViewer
{
	private:

		static STRING				getTeamNameOrPlaceholder(const Phase* phase, size_t matchIdx, int teamPos);

	public:

		static void					displayFullBracket(const Tournament& tournament);
		static void					displayPodium(const Tournament& tournament);

};

#endif