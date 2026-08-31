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
# include <memory>

# include "../class/Settings.hpp"

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Team;
class				Player;
class				Settings;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

using				pTeam			=	Team*;
using				cTeam			=	const Team&;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;
using				mpTeam			=	std::map<Team*, Team*>;
using				uTeam			=	std::unique_ptr<Team>;
using				vuTeam			=	std::vector<std::unique_ptr<Team>>;

using				cSet			=	const Settings&;

using				pPlayer			=	Player*;
using				cPlayer			=	const Player&;
using				cpPlayer		=	const Player*;
using				vpPlayer		=	std::vector<Player*>;
using				cvpPlayer		=	const std::vector<Player*>&;
using				uPlayer			=	std::unique_ptr<Player>;
using				vuPlayer		=	std::vector<std::unique_ptr<Player>>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 *	S occupe de la creation des Teams
 */
class				TeamFactory
{
	private:

		cSet						_settings;

		struct						TeamCreationCtx
		{
			vpPlayer*				males = nullptr;
			vpPlayer*				females = nullptr;
			vpPlayer*				minoritary = nullptr;
			vpPlayer*				majoritary = nullptr;
			vpPlayer*				missingPool = nullptr;
			size_t					minIdx = 0;
			size_t					majIdx = 0;
			size_t					missIdx = 0;
			int						missing = 0;
		};

		[[nodiscard]]
		vpPlayer					getAllMales(cvpPlayer players) const;
		[[nodiscard]]
		vpPlayer					getAllFemales(cvpPlayer players) const;
		[[nodiscard]]
		vpPlayer					getMultiTeamsPlayers(vpPlayer players) const;
		[[nodiscard]]
		bool						checkMissingPlayers(cvpPlayer players, int& missing) const;

		void						createTeamsUniplayer(cvpPlayer players, vuTeam& teams);
		void						createDoubleTeams(cvpPlayer players, vuTeam& teams);
		void						createMixedTeams(cvpPlayer players, vuTeam& teams);

		void						generateMissingPool(TeamCreationCtx& ctx);
		void						cloneForEqualGenders(TeamCreationCtx& ctx);
		void						cloneForUnequalGenders(TeamCreationCtx& ctx);
		void						createStandardMixedTeams(TeamCreationCtx& ctx, vuTeam& teams);
		void						createMissingMixedTeams(TeamCreationCtx& ctx, vuTeam& teams);
		void						createUnigenreTeams(TeamCreationCtx& ctx, vuTeam& teams);

	public:

		explicit TeamFactory(cSet settings);
		
		TeamFactory(const TeamFactory&) = delete;
		TeamFactory& operator=(const TeamFactory&) = delete;

		TeamFactory(TeamFactory&&) = delete;
		TeamFactory& operator=(TeamFactory&&) = delete;

		~TeamFactory() = default;

		[[nodiscard]]
		vuTeam							generateTeams(cvpPlayer players);
		
};