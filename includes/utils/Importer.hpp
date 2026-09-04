//
// Created by Nicolas Fordoxcel on 15/07/2026.
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
class				PlayerManager;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cBool			=	const bool;

using				pPlayer			=	Player*;
using				cPlayer			=	const Player&;
using				cpPlayer		=	const Player*;
using				vpPlayer		=	std::vector<Player*>;
using				cvpPlayer		=	const std::vector<Player*>&;
using				uPlayer			=	std::unique_ptr<Player>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 *	La classe Importer gere toutes les importations depuis les fichiers externes
 */
class				Importer
{
	private:

		static uPlayer				extractPlayerFromLine(cString line, cBool isFirstLine);

	public:

		Importer() = delete;
		
		static int				importPlayers(cString path, PlayerManager& playerManager);
};