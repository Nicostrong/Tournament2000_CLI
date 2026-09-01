//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iostream>
#include <algorithm>

#include "../includes/utils/PrintUtils.hpp"

#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/Color.hpp"
#include "../includes/Constantes.hpp"

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

vtupleMsg			PrintUtils::_messages;

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

void				PrintUtils::printHeader()
{
	cString title = "MESSAGES | MESSAGES | MESSAGES | MESSAGES | MESSAGES";
	
	TitleViewer::printSeparator(Color::BBLUE, '=');
	std::cout << Color::BBLUE << std::format("{:^{}}", title, LENSEPARATOR) << '\n' << Color::RESET;
	TitleViewer::printSeparator(Color::BBLUE, '=');
}

void				PrintUtils::printMessageLines(cString msg, cBool isError)
{
	const size_t MAX_TEXT_LEN = LENSEPARATOR - 5;
	size_t start = 0;
	bool firstLine = true;

	while (start < msg.length())
	{
		size_t currentLen = std::min(MAX_TEXT_LEN, msg.length() - start);

		if (start + currentLen < msg.length())
		{
			size_t lastSpace = msg.rfind(' ', start + currentLen);

			if (lastSpace != std::string::npos && lastSpace > start)
				currentLen = lastSpace - start;
		}

		cString prefix = firstLine ? (isError ? " [!] " : " [v] ") : "     ";
		std::cout << prefix << msg.substr(start, currentLen) << '\n';
		
		firstLine = false;
		start += currentLen;

		while (start < msg.length() && msg[start] == ' ')
			start++;
	}
}

void				PrintUtils::printMessage(const std::tuple<cString, cBool>& msgTuple)
{
	const auto& [msg, isError] = msgTuple;

	std::cout << (isError ? Color::RED : Color::GREEN);
	printMessageLines(msg, isError);
	std::cout << Color::RESET;
}

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
	if (_messages.empty())
		return;

	printHeader();

	for (const auto& msgTuple : _messages)
	{
		printMessage(msgTuple);
	}
	
	TitleViewer::printSeparator(Color::BBLUE, '=');
	_messages.clear();
}

/*void				PrintUtils::handleMessages()
{
	if (!_messages.empty())
	{
		cString title = "MESSAGES | MESSAGES | MESSAGES | MESSAGES | MESSAGES";

		TitleViewer::printSeparator(Color::BBLUE, '=');
		std::cout << Color::BBLUE << std::format("{:^{}}", title, LENSEPARATOR) << std::endl << Color::RESET;
		TitleViewer::printSeparator(Color::BBLUE, '=');

		const size_t MAX_TEXT_LEN = LENSEPARATOR - 5;

		for (const auto& msgTuple : _messages)
		{
			cString msg = std::get<0>(msgTuple);
			cBool isError = std::get<1>(msgTuple);

			std::cout << (isError ? Color::RED : Color::GREEN);

			size_t start = 0;
			bool firstLine = true;

			while (start < msg.length())
			{
				size_t currentLen = std::min(MAX_TEXT_LEN, msg.length() - start);

				if (start + currentLen < msg.length())
				{
					size_t lastSpace = msg.rfind(' ', start + currentLen);

					if (lastSpace != std::string::npos && lastSpace > start)
						currentLen = lastSpace - start;
				}

				if (firstLine)
				{
					std::cout << (isError ? " [!] " : " [v] ") << msg.substr(start, currentLen) << '\n';
					firstLine = false;
				}
				else
					std::cout << "     " << msg.substr(start, currentLen) << '\n';

				start += currentLen;

				while (start < msg.length() && msg[start] == ' ')
					start++;
			}
			std::cout << Color::RESET;
		}
		
		TitleViewer::printSeparator(Color::BBLUE, '=');
		
		_messages.clear();
	}
}*/