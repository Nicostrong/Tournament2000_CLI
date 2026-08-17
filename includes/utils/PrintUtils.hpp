//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef PRINT_UTILS_HPP
# define PRINT_UTILS_HPP

//	STDLIB
# include <tuple>
# include <string>
# include <vector>

//	INCLUDES

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VTUPLE_MSG	=	std::vector<std::tuple<std::string, bool>>;

//	GLOBAL VARIABLES

/**
 * Contient les methodes pour efface l ecran ou afficher des titres
 */
class PrintUtils
{
	private:

		static VTUPLE_MSG			_messages;

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
		static void					addSuccess(C_STRING msg);
		static void					addError(C_STRING msg);
		static void					handleMessages();
};

#endif