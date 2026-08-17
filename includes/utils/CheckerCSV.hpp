//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef CHECKERCSV_HPP
# define CHECKERCSV_HPP

//	STDLIB
# include <tuple>
# include <string>
# include <vector>

//	INCLUDES

//	TYPEDEF
using				C_STRING	=	const std::string&;
using				V_STRING	=	std::vector<std::string>;
using				VTUPLE_MSG	=	std::vector<std::tuple<std::string, bool>>;

//	GLOBAL VARIABLES

/**
 *	utilitaire pour le formatage des strings et l harmonisation
 */
class CheckerCSV
{
	public:

		static bool					validateParticipantCSV(C_STRING filename, VTUPLE_MSG& messages);
};

#endif