//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include "../class/Player.hpp"

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Settings;
class				PlayerManager;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cSet			=	const Settings&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 * Gere uniquement l'interaction CLI avec les participants.
 */
class				PlayerCLI
{
	private:

		static void					menuPlayer(const PlayerManager& manager, cSet settings);
		static bool					executeChoice(int choice, PlayerManager& manager, cSet settings);

		// ACTIONS UTILISATEUR
		static void					handleAddPlayer(PlayerManager& manager, cSet settings);
		static void					handleModifyPlayer(PlayerManager& manager, cSet settings);
		static void					handleDeletePlayer(PlayerManager& manager);
		static void					handleImport(PlayerManager& manager);
		static void					handleExport(const PlayerManager& manager);
		static void					handleDisplay(const PlayerManager& manager);

		// SAISIE
		static Gender				askGender(cSet settings, int currentGenderInt = -1);

	public:

		static void					handleMenuPlayer( PlayerManager& manager,cSet settings);

};
