//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>
# include <csignal>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Settings;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;

using				cInt			=	const int;
using				vInt			=	std::vector<int>;
using				cvInt			=	const std::vector<int>;

using				cBool			=	const bool;

using				pSet			=	Settings*;
using				cSet			=	const Settings&;
using				cpSet			=	const Settings*;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

extern volatile std::sig_atomic_t	g_running;

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				SettingsCLI
{

	public:

		static void					setupWizard(Settings& s);

	private:

		static int					inputInt(cString prompt, cInt min, cInt max, cInt defaultVal);
		static int					inputIntList(cString prompt, cvInt allowedValues, cInt defaultVal);
		static bool					inputBool(cString prompt, cBool defaultVal);
		static String				inputString(cString prompt, cString defaultVal);
		static void					setupPlayers(pSet s);
		static void					setupPools(pSet s);
		static void					setupMatchRules(pSet s);
		static void					setupPhaseSets(pSet s);

};

std::ostream&		operator<<(std::ostream& os, cSet s);