//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef PRINT_UTILS_HPP
# define PRINT_UTILS_HPP

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

/**
 * Contient les methodes pour efface l ecran ou afficher des titres
 */
class PrintUtils
{
	private:

		static vtupleMsg			_messages;

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
		static void					addSuccess(cString msg);
		static void					addError(cString msg);
		static void					handleMessages();
};

#endif