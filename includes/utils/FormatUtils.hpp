//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef FORMAT_UTILS_HPP
# define FORMAT_UTILS_HPP

//	STDLIB
# include <string>

//	INCLUDES

//	TYPEDEF
using				STRING		=	std::string;

//	GLOBAL VARIABLES

/**
 * Contient plusieurs methodes de modification de strings.
 */
class FormatUtils
{
	public:
		static void					trim(STRING& s);
		static void					capitalize(STRING& s);
		static void					toUpper(STRING& s);
		static void					toLower(STRING& s);
		static void					trimAndCapitalize(STRING& s);
		static void					trimAndPutToUpper(STRING& s);
		static void					trimAndPutToLower(STRING& s);
};

#endif