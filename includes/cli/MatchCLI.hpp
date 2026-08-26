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

class				Match;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				pMatch			=	Match*;
using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/


class				MatchCLI
{
	private:

		static void					displayMenuUI(cvpMatch matches);
		static void					handleTitle();
		static void					menuMatch(pMatch match);
		static void					submenuMatch(pMatch match);

		static void					executeChoice(cInt choice, pMatch match);

		void						handelSaveScore(pMatch match);

	public:

		static void					handleMenuMatch(cvpMatch matches);

};
