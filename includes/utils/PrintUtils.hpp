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

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;

using				cBool			=	const bool;

using				vtupleMsg		=	std::vector<std::tuple<std::string, bool>>;

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

};