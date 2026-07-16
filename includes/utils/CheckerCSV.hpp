//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef CHECKERCSV_HPP
# define CHECKERCSV_HPP

//	STDLIB
# include <string>
# include <vector>

//	INCLUDES

//	TYPEDEF
typedef const std::string&			C_STRING;
typedef std::vector<std::string>	V_STRING;

//	GLOBAL VARIABLES

/**
 *	utilitaire pour le formatage des strings et l harmonisation
 */
class CheckerCSV
{
	public:

		static bool					validateParticipantCSV(C_STRING filename, V_STRING& errorMessages);
};

#endif