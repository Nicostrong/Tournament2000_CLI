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
using				C_STRING	=	const std::string&;

//	GLOBAL VARIABLES

/**
 *	utilitaire pour le formatage des strings et l harmonisation
 */
class FormatUtils
{
	public:
		static STRING				trim(C_STRING s);
		static STRING				capitalize(STRING s);
		static STRING				toUpper(STRING s);
};

#endif