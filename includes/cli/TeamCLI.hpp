//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <csignal>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				pTeam			=	Team*;
using				cTeam			=	const Team&;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;

using				pTour			=	Tournament*;
using				cTour			=	const Tournament&;
using				cpTour			=	const Tournament*;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

extern volatile std::sig_atomic_t	g_running;

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				TeamCLI
{
	private:

		static void					displayMenuUI(cTour tournament);
		static void					handleTitle();
		static void					menuTeam();
		static void					submenuTeam(pTeam team);

		static void					clearInput();
		static String				fetchInput();
		static int					parseChoice(cString input);
		static void					executeChoice(cInt choice, pTeam team);

		//static void					handleShowAllTeams(vpTeam allTeams);
		static void					handleShowOneTeams(pTeam team);
		static void					handleModifyTeamName(pTeam team);
		static void					handleModifyTeamMember(pTeam team);
		static void					handleDisqualifiedTeam(pTeam team);

		static bool					checkTeamId(int id, Tournament& tournament);

		//static void					handleTeamManagement(Tournament& tournament);
		//static void					handleSingleTeamEdit(Team* team, Tournament& tournament);
		

	public:

		static void					handleMenuTeam(Tournament& tournament);
};

