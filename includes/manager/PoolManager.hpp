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
class				Pool;
class				Player;
class				Settings;

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
using				uPool			=	std::unique_ptr<Pool>;
using				vuPool			=	std::vector<std::unique_ptr<Pool>>;

using				pTeam			=	Team*;
using				cTeam			=	const Team&;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;
using				mpTeam			=	std::map<Team*, Team*>;

using				cSet			=	const Settings&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 *	S occupe de la gestion des pools
 */
class				PoolManager
{
	private:
		cSet						_settings;
		vuPool						_pools;

		void						createEmptyPools(int nbPools);
		void						distributeTeamsToPools(cvpTeam teams, int nbPools);
		void						resolvePoolConflicts(int nbPools);
		void						generatePoolMatches() const;
		[[nodiscard]]
		bool						teamsShareMember(pTeam a, pTeam b);

	public:

		explicit PoolManager(cSet settings);

		PoolManager(const PoolManager&) = delete;
		PoolManager& operator=(const PoolManager&) = delete;

		PoolManager(PoolManager&&) = delete;
		PoolManager& operator=(PoolManager&&) = delete;

		~PoolManager() = default;

		//	GETTER
		[[nodiscard]]
		vpPool						getPools() const;

		//	METHOD
		void						clean();
		void						generatePools(cvpTeam teams);
		void						sortAllPools() const;
		[[nodiscard]]
		bool						isPoolsFinished() const;
		void						applyPoolDisqualification(pTeam team);
		void						unDisqualifyTeam(pTeam team);

};
