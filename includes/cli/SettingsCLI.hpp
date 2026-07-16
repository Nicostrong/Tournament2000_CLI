//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

#ifndef SETTINGSCLI_H
# define SETTINGSCLI_H

//	STDLIB
# include <string>

//	INCLUDES
# include "../class/Settings.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef const std::vector<int>&		CV_INT;

class SettingsCLI
{

	public:

		static void					setupWizard(Settings& s);
		static void					display(const Settings& s);
		static void					editMenu(Settings& s);

	private:

		static int					inputInt(C_STRING prompt, int min, int max, int defaultVal);
		static int					inputIntList(C_STRING prompt, CV_INT allowedValues, int defaultVal);
		static bool					inputBool(C_STRING prompt, bool defaultVal);
		static std::string			inputString(C_STRING prompt, C_STRING defaultVal);

};

#endif