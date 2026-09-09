//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <vector>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Pool;
class				Team;
class				Player;
class				Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				pTeam			=	Team*;
using				cpTeam			=	const Team*;

using				cTour			=	const Tournament&;

using				pTeam			=	Team*;
using				cpTeam			=	const Team*;

using				pPool			=	Pool*;

using				pPlayer			=	Player*;
using				vpPlayer		=	std::vector<Player*>;

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

		static void					executeChoice(int choice, pTeam team, Tournament& tournament);

		static void					handleModifyTeamName(pTeam team);
		static void					handleModifyTeamMember(pTeam team, Tournament& tournament);
		static void					handleDisqualifiedTeam(pTeam team, Tournament& tournament);

		static bool					checkTeamId(int id, Tournament& tournament);
		static pPool				findTeamPool(cpTeam team, Tournament& tournament);
		static bool					isPlayerInPool(pPlayer player, pPool pool);
		static vpPlayer				getEligibleSubstitutes(pTeam team, Tournament& tournament);
		static int					selectMemberIndex(pTeam team);
		static pPlayer				selectSubstitutePlayer(vpPlayer candidates);

	public:

		static void					handleMenuTeam(Tournament& tournament);
};

