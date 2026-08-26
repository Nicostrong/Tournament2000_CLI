//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <random>
#include <iostream>

#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/


/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

void				TitleViewer::printRainbow( cString text)
{
	int colorIndex = 0;

	for (const char c : text)
	{
		if (c == ' ' || c == '\n' || c == '\r' || c == '=')
			std::cout << c;
		else
		{
			std::cout << rainbowColors[colorIndex % rainbowColors.size()] << c;
			colorIndex++;
		}
	}
	std::cout << Color::RESET;
}

const char*			TitleViewer::getRandomColor()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, rainbowColors.size() - 1);

	return (rainbowColors[distrib(gen)]);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 *	Banniere du programme
 */
void				TitleViewer::banner()
{
	printRainbow(R"(
 _______                                                _
|__   __|                                              | |
   | | ___  _   _ _ __ _ __   __ _ _ __ ___   ___ _ __ | |_
   | |/ _ \| | | | '__| '_ \ / _` | '_ ` _ \ / _ \ '_ \| __|
   | | (_) | |_| | |  | | | | (_| | | | | | |  __/ | | | |_
   |_|\___/ \__,_|_|  |_| |_|\__,_|_| |_| |_|\___|_| |_|\__|

                ___   ___   ___   ___
               |__ \ / _ \ / _ \ / _ \
                  ) | | | | | | | | | |
                 / /| | | | | | | | | |
                / /_| |_| | |_| | |_| |
               |____|\___/ \___/ \___/
)");
	std::cout << Color::HIDDEN << "\nBy Nicostrong\n" <<Color::RESET;
}

/**
 *	Titre du menu Setting
 */
void				TitleViewer::setting()
{
	const char* color = getRandomColor();

    printSeparator(color);
    std::cout << color << (R"(
              _____       _   _    _
             / ____|     | |_| |_ (_)       ____
            | (___  ___  |  _|  _|| |_ _   / _  |
             \___ \/ _ \ | | | |  | | '_ \| (_| |
             ____) |  __/| ||| |  | | | | |\_,  |
            |_____/ \___|\__| \_| |_|_| |_|__/  |
                                          |____/

)") << Color::RESET << std::endl;

    printSeparator(color);
}

/**
 *	Titre du menu Players
 */
void				TitleViewer::players()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
             _____  _
            |  __ \| |
            | |__) | | __ _ _   _  ___ _ __ ___
            |  ___/| |/ _` | | | |/ _ \ '__/ __|
            | |    | | (_| | |_| |  __/ |  \__ \
            |_|    |_|\__,_|\__, |\___|_|  |___/
                             __/ |
                            |___/

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu Tournament
 */
void				TitleViewer::tournament()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
 _______                                                _
|__   __|                                              | |
   | | ___  _   _ _ __ _ __   __ _ _ __ ___   ___ _ __ | |_
   | |/ _ \| | | | '__| '_ \ / _` | '_ ` _ \ / _ \ '_ \| __|
   | | (_) | |_| | |  | | | | (_| | | | | | |  __/ | | | |_
   |_|\___/ \__,_|_|  |_| |_|\__,_|_| |_| |_|\___|_| |_|\__|

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu Teams
 */
void				TitleViewer::teams()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
             _______
            |__   __|
               | |  __  __ _ _ __ ___  ___
               | |/ _ \/ _` | '_ ` _ \/ __|
               | |  __/ (_| | | | | | \__ \
               |_|\___|\__,_|_| |_| |_|___/

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu Pools
 */
void				TitleViewer::pools()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
                _____           _
               |  __ \         | |
               | |__) |__  ___ | |___
               |  ___/ _ \/ _ \| / __|
               | |  | (_)| (_) | \__ \
               |_|   \___/\___/|_|___/

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu Matches
 */
void				TitleViewer::matches()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
           __    __        _        _
          |  \  /  |      | |__    | |
          | | \/ | |  __ _|  __|___| |___  ____  ____
          | |\__/| | / _` | | /  _/|  __ \/ __ \/  __|
          | |    | |/ (_| | |_| (__| |  \ \  __/\___ \
          |_|    |_|\___,_|__/\___/|_|  |_|\___/|____/

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu 1/16
 */
void				TitleViewer::sixteenths()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
     _____ _        _                   _    _
    / ____(_)      | |                 | |  | |
   | (___  _ __  __| |_  __  ___  _ __ | |_ | |__  ___
    \___ \| |\ \/ /|  _|/ _\/ _ \| '_ \| __|| '_ \/ __|
    ____) | | >  < | | | __/| __/| | | | |_ | | | \__ \
   |_____/|_|/_/\_\|_|  \__|\___||_| |_|\__||_| |_|___/

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu 1/8
 */
void				TitleViewer::heighths()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
          _    _      _       _     _   _
         | |  | |    (_)     | |   | | | |
         | |__| | ___ _  __ _| |__ | |_| |__  ___
         |  __  |/ _ \ |/ _` | '_ \| __| '_ \/ __|
         | |  | |  __/ | (_| | | | | |_| | | \__ \
         |_|  |_|\___|_|\__, |_| |_|\__|_| |_|___/
                         __/ |
                        |___/

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu 1/4
 */
void				TitleViewer::quarters()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
           ____                   _
          / __ \                 | |
         | |  | |_   _  __ _ _ __| |_ ___ _ __ ___
         | |  | | | | |/ _` | '__| __/ _ \ '__/ __|
         | |__| | |_| | (_| | |  | ||  __/ |  \__ \
          \___\_\\__,_|\__,_|_|   \__\___|_|  |___/

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu 1/2
 */
void				TitleViewer::semis()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
              _____                _
             / ____|              (_)
            | (___   ___ _ __ ___  _ ___
             \___ \ / _ \ '_ ` _ \| / __|
             ____) |  __/ | | | | | \__ \
            |_____/ \___|_| |_| |_|_|___/

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu 3eme place
 */
void				TitleViewer::thirdPlace()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
    _______ _     _          _ _____  _
   |__   __| |   (_)        | |  __ \| |
      | |  | |__  _ _ __   _| | |__) | | __ _  ___ ___
      | |  | '_ \| | '__|/ _' |  ___/| |/ _` |/ __/ _ \
      | |  | | | | | |  | (_) | |    | | (_| | (_|  __/
      |_|  |_| |_|_|_|   \___/\_|    |_|\__,_|\___\___|

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu Finale
 */
void				TitleViewer::final()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
                   ______ _             _
                  |  ____(_)           | |
                  | |__   _ _ __   __ _| |
                  |  __| | | '_ \ / _` | |
                  | |    | | | | | (_| | |
                  |_|    |_|_| |_|\__,_|_|

)" << Color::RESET << std::endl;
	printSeparator(color);
}

/**
 *	Titre du menu Exports
 */
void				TitleViewer::exportMenu()
{
	const char* color = getRandomColor();

	printSeparator(color);
	std::cout << color << R"(
             ______                      _
            |  ____|                    | |
            | |__  __  ___ __   ___  ___| |_ ___
            |  __| \ \/ / '_ \ / _ \/ __| __/ __|
            | |____ >  <| |_) | (_) | | | |_\__ \
            |______/_/\_\ .__/ \___/|_|  \__|___/
                        | |
                        |_|

)" << Color::RESET << std::endl;
	printSeparator(color);
}

void				TitleViewer::printSeparator(const char c)
{
	for (int i = 0; i < 60; ++i)
		std::cout << rainbowColors[i % rainbowColors.size()] << c;

	std::cout << Color::RESET << std::endl;
}

void				TitleViewer::printSeparator(cString color, const char c)
{
	if (color.empty())
	{
		printSeparator(c);
		return;
	}
	std::cout << color;

	for (int i = 0; i < 60; ++i)
		std::cout << c;

	std::cout << Color::RESET << std::endl;
}
