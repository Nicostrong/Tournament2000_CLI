//
// Created by Nicolas Fordoxcel on 04/09/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>

# include "./CLIUtils.hpp"

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;

using				cInt			=	const int;

using				cBool			=	const bool;

using				vMenuItem		=	std::vector<MenuItem>;

using				pTour			=	Tournament*;
using				cTour			=	const Tournament&;
using				cpTour			=	const Tournament*;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				ShowCLI
{
	private:

		//	Affichage du menu
		static vMenuItem			displayMenuUI(cTour tournament);
		static vMenuItem			menuShow(cTour tournament);

		//	Handlers de saisie
		static void					executeChoice(cInt choice, Tournament& tournament);

	public:

		//	Menu principal
		static void					handleMenuShow(Tournament& tournament);

};