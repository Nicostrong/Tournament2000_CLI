//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <array>
# include <string>

# include "../Color.hpp"

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cString			=	const std::string&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

static constexpr std::array<const char*, 6>	rainbowColors =
{
	Color::BRED,
	Color::BGREEN,
	Color::BYELLOW,
	Color::BBLUE,
	Color::BMAGENTA,
	Color::BCYAN
};

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				TitleViewer
{
	private:

		static void					printRainbow(cString text);
		static const char*			getRandomColor();

	public:

		static void					banner();
		static void					setting();
		static void					players();
		static void					tournament();
		static void					teams();
		static void					pools();
		static void					matches();
		static void					sixteenths();
		static void					heighths();
		static void					quarters();
		static void					semis();
		static void					thirdPlace();
		static void					final();
		static void					exportMenu();
		static void					printSeparator(char c = '=');
		static void					printSeparator(cString color, char c = '=');

};
