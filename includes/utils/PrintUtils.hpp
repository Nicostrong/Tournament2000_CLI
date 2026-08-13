//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef PRINT_UTILS_HPP
# define PRINT_UTILS_HPP

//	STDLIB

//	INCLUDES

//	TYPEDEF

//	GLOBAL VARIABLES

/**
 * Contient les methodes pour efface l ecran ou afficher des titres
 */
class PrintUtils
{
	public:
		static void					clear();
		static void					banner();
		static void					setting();
		static void					players();
		static void					tournament();
		static void					teams();
		static void					pools();
		static void					sixteenths();
		static void					heighths();
		static void					quarters();
		static void					semis();
		static void					thirdPlace();
		static void					final();
		static void					exportMenu();
};

#endif