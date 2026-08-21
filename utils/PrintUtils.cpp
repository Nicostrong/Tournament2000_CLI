//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <iostream>

#include "../includes/utils/PrintUtils.hpp"

#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cBool			=	const bool;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

vtupleMsg			PrintUtils::_messages;

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				PrintUtils::clear()
{
	std::cout << "\033[2J\033[1;1H";
}

void                PrintUtils::addSuccess(cString msg)
{
    _messages.push_back({msg, false});
}

void                PrintUtils::addError(cString msg)
{
    _messages.push_back({msg, true});
}

void				PrintUtils::handleMessages()
{
	if (!_messages.empty())
	{
		TitleViewer::printSeparator(Color::BBLUE, '=');
		std::cout << Color::BBLUE << "  MESSAGES | MESSAGES | MESSAGES | MESSAGES | MESSAGES\n" << Color::RESET;
		TitleViewer::printSeparator(Color::BBLUE, '=');

		for (const auto& msgTuple : _messages)
		{
			cString msg = std::get<0>(msgTuple);
			cBool isError = std::get<1>(msgTuple);

			if (isError)
				std::cout << Color::RED << "\t[!]\t" << msg;
			else
				std::cout << Color::GREEN << "\t[v]\t" << msg;

			std::cout << Color::RESET << std::endl;
		}
        
		TitleViewer::printSeparator(Color::BBLUE, '=');
        
		_messages.clear();
	}
}
