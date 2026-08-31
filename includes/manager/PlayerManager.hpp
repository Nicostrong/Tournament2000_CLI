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

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Player;
class				Settings;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				pPlayer			=	Player*;
using				cpPlayer		=	const Player*;
using				vpPlayer		=	std::vector<Player*>;
using				cvpPlayer		=	const std::vector<Player*>&;
using				uPlayer			=	std::unique_ptr<Player>;
using				vuPlayer		=	std::vector<std::unique_ptr<Player>>;

using				cSet			=	const Settings&;

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 * Gere le cycle de vie et la collection des participants.
 *
 * PlayerManager est responsable de :
 * - la creation des participants
 * - leur stockage
 * - leur recherche
 * - leur modification
 * - leur suppression
 * - l'import/export CSV
 * - les regles de gestion liees aux participants
 *
 */
class				PlayerManager
{
	private:

		cSet						_settings;
		vuPlayer					_players;

		[[nodiscard]]
		bool						isPseudoAvailable(cString pseudo, pPlayer ignored = nullptr) const;
		[[nodiscard]]
		uPlayer						createPlayer(cString pseudo, cString lastName, cString firstName, Gender gender) const;

	public:

		explicit PlayerManager(cSet settings);

		PlayerManager(const PlayerManager&) = delete;
		PlayerManager& operator=(const PlayerManager&) = delete;

		PlayerManager(PlayerManager&&) = delete;
		PlayerManager& operator=(PlayerManager&&) = delete;

		~PlayerManager() = default;

		// GETTER
		[[nodiscard]]
		vpPlayer					getPlayers() const;
		[[nodiscard]]
		pPlayer						getPlayerById(size_t id) const;
		[[nodiscard]]
		pPlayer						getPlayerByPseudo(cString pseudo) const;
		[[nodiscard]]
		size_t						getSize() const;
		[[nodiscard]]
		bool						isEmpty() const;

		// MANAGEMENT
		bool						addPlayer(cString pseudo, cString lastName, cString firstName, Gender gender);
		bool						addPlayer(uPlayer player);
		bool						removePlayer(size_t id);
		bool						modifyPlayer(size_t id, cString pseudo, cString lastName, cString firstName, Gender gender);
		void						clear();

};