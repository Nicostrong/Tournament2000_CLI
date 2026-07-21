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
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				CV_INT		=	const std::vector<int>&;

class SettingsCLI
{

	public:

		static void					setupWizard(Settings& s);
		//static void					display(const Settings& s);
		//static void					editMenu(Settings& s);
		//STRING						printToJSON(const Settings& s);

	private:

		static int					inputInt(C_STRING prompt, int min, int max, int defaultVal);
		static int					inputIntList(C_STRING prompt, CV_INT allowedValues, int defaultVal);
		static bool					inputBool(C_STRING prompt, bool defaultVal);
		static std::string			inputString(C_STRING prompt, C_STRING defaultVal);

};

std::ostream&		operator<<(std::ostream& os, const Settings& s);

#endif