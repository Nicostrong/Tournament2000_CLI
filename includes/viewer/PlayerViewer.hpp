//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>
# include <ostream>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Player;
class				Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				pPlayer			=	Player*;
using				cPlayer			=	const Player&;
using				cpPlayer		=	const Player*;
using				vpPlayer		=	std::vector<Player*>;
using				cvpPlayer		=	const std::vector<Player*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				PlayerViewer
{
	public:

		static void					showPlayerCard(cPlayer p);
		static void					showFullTableOfPlayers(cvpPlayer playerss);

		static void					printAll(Tournament& tournament);

};

std::ostream&		operator<<(std::ostream& os, cPlayer p);

