//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Team;
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

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				TeamCLI
{
	private:

		static void					displayMenuUI(cTour tournament);
		static void					menuTeam(pTeam team);
		static void					submenuTeam(pTeam team, Tournament& tournament);

		static void					executeChoice(cInt choice, pTeam team, Tournament& tournament);

		static void					handleModifyTeamName(pTeam team);
		static void					handleModifyTeamMember(pTeam team, Tournament& tournament);
		static void					handleDisqualifiedTeam(pTeam team, Tournament& tournament);

		static bool					checkTeamId(int id, Tournament& tournament);
		static cpPool				findTeamPool(cpTeam team, Tournament& tournament);
		static bool					isPlayerInPool(cpPart player, cpPool pool);
		static vpPart				getEligibleSubstitutes(pTeam team, Tournament& tournament);
		static int					selectMemberIndex(pTeam team);
		static pPart				selectSubstitutePlayer(vpPart candidates);

	public:

		static void					handleMenuTeam(Tournament& tournament);
};

