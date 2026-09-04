//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <tuple>
# include <string>
# include <vector>
# include <fstream>
# include <string_view>

#include "../Constantes.hpp"

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Match;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;
using				StringV			=	std::string_view;

using				cBool			=	const bool;

using				vtupleMsg		=	std::vector<std::tuple<std::string, bool>>;

using				pMatch			=	Match*;
using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 * Contient les methodes pour efface l ecran ou afficher des titres
 */
class				PrintUtils
{
	private:

		static vtupleMsg			_messages;

		static void					printHeader();
		static void					printMessageLines(cString msg, cBool isError);
		static void					printMessage(const std::tuple<cString, cBool>& msgTuple);
	
		public:
	
		static void					clear();
		static void					addSuccess(cString msg);
		static void					addError(cString msg);
		static void					handleMessages();

		//	PRINT
		static void					printTitle(StringV title,  int len = LENTITLE);
		static void					printSeparator(char c = '=', int len = LENSEPARATOR);
		static void					printSeparator(cString color, char c = '=');

		//	HELPER
		static void					writeMatchesList(std::ostream& out, cvpMatch matches, bool toFile);

};