//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef CHECKERCSV_HPP
# define CHECKERCSV_HPP

//	STDLIB
# include <string>

//	INCLUDES

//	TYPEDEF
using				C_STRING	=	const std::string&;

//	GLOBAL VARIABLES

/**
 *	utilitaire pour le formatage des strings et l harmonisation
 */
class CheckerCSV
{
	public:

		static bool					validateParticipantCSV(C_STRING filename);
};

#endif