//
// Created by Nicolas Fordoxcel on 28/08/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>
# include <memory>

# include "../factory/TeamFactory.hpp"

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

using				pTeam			=	Team*;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;
using				uTeam			=	std::unique_ptr<Team>;
using				vuTeam			=	std::vector<std::unique_ptr<Team>>;

using				cSet			=	const Settings&;

using				pPlayer			=	Player*;
using				cPlayer			=	const Player&;
using				vpPlayer		=	std::vector<Player*>;
using				cvpPlayer		=	const std::vector<Player*>&;
using				uPlayer			=	std::unique_ptr<Player>;
using				vuPlayer		=	std::vector<std::unique_ptr<Player>>;

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 *	S occupe de gerer les Teams
 */
class				TeamManager
{
	private:

		TeamFactory					_teamFactory;
		vuTeam						_teams;

	public:

		explicit TeamManager(cSet settings);

		TeamManager(const TeamManager&) = delete;
		TeamManager& operator=(const TeamManager&) = delete;

		TeamManager(TeamManager&&) = delete;
		TeamManager& operator=(TeamManager&&) = delete;

		~TeamManager() = default;
		

		// GETTER
		[[nodiscard]]
		vpTeam						getTeams() const;
		[[nodiscard]]
		pTeam						getTeamById(int id) const;
		[[nodiscard]]
		size_t						getSize() const;
		[[nodiscard]]
		bool						isEmpty() const;

		// MANAGEMENT
		void						clean();
		void						generateTeams(cvpPlayer players);
};