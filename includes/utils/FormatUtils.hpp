//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef FORMAT_UTILS_HPP
# define FORMAT_UTILS_HPP

//	STDLIB
# include <string>

//	INCLUDES

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;

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