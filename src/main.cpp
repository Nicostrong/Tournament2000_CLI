//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <csignal>
#include <iostream>

#include "../includes/controller/TournamentController.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cInt		=	const int;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

inline volatile std::sig_atomic_t	g_running = -1;

/****************************************************************************************************/
/*	SIGNAL																							*/
/****************************************************************************************************/

/**
 * Fonction appelee automatiquement lors de la reception d un SIGINT (Ctrl+C).
 */
static void			handleSigint(cInt signum)
{
	(void)signum;

	std::cout << Color::RED << "\n\n[!] Interruption detectee. Arret du programme...\n" << Color::RESET;

	g_running = false;
}

/****************************************************************************************************/
/*	MAIN																							*/
/****************************************************************************************************/

int					main()
{
	struct sigaction sa = {};

	sa.sa_handler = handleSigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	sigaction(SIGINT, &sa, nullptr);
	
	TournamentController tournament;

	tournament.run();

	return (0);
}