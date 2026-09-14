//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iomanip>
#include <iostream>
#include <algorithm>

#include "../includes/class/Match.hpp"

#include "../includes/utils/PrintUtils.hpp"

#include "../includes/Color.hpp"
#include "../includes/class/Team.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cString			=	const std::string&;

using				cInt			=	const int;

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
	
	printSeparator(std::cout, Color::BBLUE, '=');
	std::cout << Color::BBLUE << std::format("{:^{}}", title, LENSEPARATOR) << '\n' << Color::RESET;
	printSeparator(std::cout, Color::BBLUE, '=');
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
	
	printSeparator(std::cout, Color::BBLUE, '=');
	_messages.clear();
}

void				PrintUtils::printTitle(std::ostream& out, StringV title, int len)
{
	cString titleCenter = std::format(" {} ", title);

	out << std::format("{:=^{}}", titleCenter, len) << std::endl;
}

void				PrintUtils::printSeparator(std::ostream& out, const char c, cInt len)
{
	for (int i = 0; i < len; ++i)
		out << c;

	out << std::endl;
}

void				PrintUtils::printSeparator(std::ostream& out, cString color, const char c)
{
	if (color.empty())
	{
		printSeparator(out, c);
		return;
	}
	out << color;

	for (int i = 0; i < LENSEPARATOR; ++i)
		out << c;

	out << Color::RESET << std::endl;
}

void				PrintUtils::writeMatchesList(std::ostream& out, cvpMatch matches, bool toFile)
{
	if (matches.empty())

		return (addError("No matches."));

	int i = 1;

	for (const Match* m : matches)
	{
		if (!m)
			continue;

		out << "  " << std::setw(2) << i++ << ". " << m->getTeamA()->getName() << " vs " << m->getTeamB()->getName();

		if (m->isFinished())
		{
			out << "  [ " << m->getScoreA() << " - " << m->getScoreB() << " ]";

			if (m->getWinner())
			{
				if (!toFile)
					out << Color::BGREEN;

				out << "  ->  " << "Winner : " << m->getWinner()->getName();

				if (!toFile)
					out << Color::RESET;
			}
		}
		else
		{
			if (!toFile)
				out << Color::BYELLOW;

			out << "  [ To play ]";

			if (!toFile)
				out << Color::RESET;
		}
		out << "\n";
	}
}