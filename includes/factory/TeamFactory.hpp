//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <map>
# include <string>
# include <vector>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Team;
class				Pool;
class				Settings;
class				Participant;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

using				pPool			=	Pool*;
using				cPool			=	const Pool&;
using				cpPool			=	const Pool*;
using				vpPool			=	std::vector<Pool*>;
using				cvpPool			=	const std::vector<Pool*>&;

using				pTeam			=	Team*;
using				cTeam			=	const Team&;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;
using				mpTeam			=	std::map<Team*, Team*>;

using				pSet			=	Settings*;
using				cSet			=	const Settings&;
using				cpSet			=	const Settings*;

using				pPart			=	Participant*;
using				cPart			=	const Participant&;
using				cpPart			=	const Participant*;
using				vpPart			=	std::vector<Participant*>;
using				cvpPart			=	const std::vector<Participant*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				TeamFactory
{
	private:

		struct TeamCreationCtx
		{
			vpPart*					males			=	nullptr;
			vpPart*					females			=	nullptr;
			vpPart*					minoritary		=	nullptr;
			vpPart*					majoritary		=	nullptr;
			vpPart*					missingPool		=	nullptr;

			size_t					minIdx			=	0;
			size_t					majIdx			=	0;
			size_t					missIdx			=	0;

			int						missing			=	0;
		};

		//	METHOD
		bool						teamsShareMember(cpTeam a, cpTeam b);
		bool						poolHasConflict(cpPool pool, cpTeam incoming);
		
		void						createTeamsUniplayer();
		void						createMixedTeams();
		void						createDoubleTeams();
		
		static void					generateMissingPool(const TeamCreationCtx& ctx);
		static void					cloneForEqualGenders(const TeamCreationCtx& ctx);
		static void					cloneForUnequalGenders(const TeamCreationCtx& ctx);

		void						createStandardMixedTeams(TeamCreationCtx& ctx);
		void						createMissingMixedTeams(TeamCreationCtx& ctx);
		void						createUnigenreTeams(TeamCreationCtx& ctx);

		bool						checkMissingPlayers(int& missing) const;

		//	GETTER
		[[nodiscard]]
		vpPart						getAllMales() const;
		[[nodiscard]]
		vpPart						getAllFemales() const;
		[[nodiscard]]
		vpPart						getMultiTeamsPlayers(vpPart participants) const;

	public:

		static vpTeam				createTeams(const vpPart& participants,cpSet settings);

};
