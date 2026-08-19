//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

#ifndef SETTINGSCLI_H
# define SETTINGSCLI_H

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include "../Global.hpp"

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class SettingsCLI
{

	public:

		static void					setupWizard(Settings& s);

	private:

		static int					inputInt(cString prompt, cInt min, cInt max, cInt defaultVal);
		static int					inputIntList(cString prompt, cvInt allowedValues, cInt defaultVal);
		static bool					inputBool(cString prompt, cBool defaultVal);
		static String				inputString(cString prompt, cString defaultVal);
		static void					setupPlayers(Settings& s);
		static void					setupPools(cSet s);
		static void					setupMatchRules(cSet s);
		static void					setupPhaseSets(cSet s);

};

std::ostream&		operator<<(std::ostream& os, cSet s);

#endif