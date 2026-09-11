//
// Created by Nicolas Fordoxcel on 26/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <limits>
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <algorithm>

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/FormatUtils.hpp"

#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/cli/CLIUtils.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				StringV			=	std::string_view;

using				cInt			=	const int;
using				cvInt			=	const std::vector<int>&;

using				cBool			=	const bool;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				CLIUtils::clearInput()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void				CLIUtils::checkInterruption()
{
	if (!g_running || std::cin.eof())
		throw CLIInterrupted();
}

String				CLIUtils::input()
{
	String input;
	
	checkInterruption();

	if (!std::getline(std::cin, input))
	{
		checkInterruption();
		clearInput();
		PrintUtils::addError("Saisie invalide.");
		return ("");
	}
	
	return (input);
}

std::optional<int>	CLIUtils::parseInt(StringV input)
{
	try
	{
		std::size_t pos = 0;
		cInt value = std::stoi(String(input), &pos);

		if (pos != input.size())
			return (std::nullopt);

		return (value);
	} 
	catch (const std::exception&) 
	{
		return (std::nullopt);
	}
}

bool				CLIUtils::askBool(StringV prompt, cBool defaultValue)
{
	String defaultStr = defaultValue ? "o/O" : "n/N";
	
	while (true)
	{
		checkInterruption();

		std::cout << std::format("{} [{}] : ", prompt, defaultStr);
		String res = input();
		
		if (res.empty())
			return (defaultValue);
			
		if (res == "O" || res == "o" || res == "Y" || res == "y")
			return (true);
			
		if (res == "N" || res == "n")
			return (false);
			
		std::cout << Color::RED << "Veuillez repondre par O(ui) ou N(on).\n" << Color::RESET;
	}
}

String				CLIUtils::askString(StringV prompt, StringV defaultValue)
{
	while (true)
	{
		checkInterruption();

		if (defaultValue.empty())
			std::cout << std::format("{} : ", prompt);
		else
			std::cout << std::format("{} [{}] : ", prompt, defaultValue);
			
		String res = input();
		
		if (res.empty())
		{
			if (!defaultValue.empty())
				return String(defaultValue);
				
			std::cout << Color::RED << "La saisie ne peut pas etre vide.\n" << Color::RESET;
		}
		else
			return (res);
	}
}

int					CLIUtils::askInt(StringV prompt, int min, int max, int defaultValue)
{
	while (true)
	{
		checkInterruption();

		std::cout << std::format("{} [{}-{}] (defaut: {}) : ", prompt, min, max, defaultValue);
		String res = input();
		
		if (res.empty())
			return (defaultValue);
			
		auto parsed = parseInt(res);

		if (parsed.has_value() && parsed.value() >= min && parsed.value() <= max)
			return (parsed.value());
		

		std::cout << Color::RED << std::format("Veuillez entrer un nombre entier compris entre {} et {}.\n", min, max) << Color::RESET;
	}
}

int					CLIUtils::askIntList(StringV prompt, cvInt allowedValues, cInt defaultValue)
{
	String optionsStr;

	for (size_t i = 0; i < allowedValues.size(); ++i)
	{
		optionsStr += std::to_string(allowedValues[i]);

		if (i < allowedValues.size() - 1)
			optionsStr += ", ";
	}

	while (true)
	{
		checkInterruption();

		std::cout << std::format("{} [{}] (defaut: {}) : ", prompt, optionsStr, defaultValue);
		String res = input();
		
		if (res.empty())
			return (defaultValue);
			
		auto parsed = parseInt(res);

		if (parsed.has_value())
			if (std::ranges::find(allowedValues.begin(), allowedValues.end(), parsed.value()) != allowedValues.end())
				return (parsed.value());
		
		std::cout << Color::RED << std::format("Cette valeur n'est pas dans la liste autorisee. Options : {}\n", optionsStr) << Color::RESET;
	}
}

void				CLIUtils::displayMenu(std::span<const MenuItem> menus, StringV title)
{
	if (!title.empty())
		PrintUtils::printTitle(title);

	for (const auto& item : menus)
		std::cout << Color::BYELLOW << "\t" << item.key << "\t" << Color::RESET << item.label << std::endl;
	
	std::cout << Color::GREEN << "Entrer le numero ou la lettre correspondant a ce que vous voulez: " << Color::RESET;
}

String				CLIUtils::askMenuChoice(std::span<const MenuItem> menus)
{
	String choice = input();
	FormatUtils::toLower(choice);

	if (choice.empty())
		return ("");

	for (const auto& item : menus)
	{
		String key = item.key;
		FormatUtils::toLower(key);

		if (key == choice)
			return (item.key);
	}

	std::vector<MenuItem> matches;

	for (const auto& item : menus)
	{
		std::string lowerLabel = item.label;
		FormatUtils::toLower(lowerLabel);

		if (lowerLabel.find(choice) != std::string::npos)
			matches.push_back(item);
	}

	if (matches.size() == 1)
		return (matches[0].key);
	else if (matches.size() > 1)
	{
		PrintUtils::addError("Saisie ambigue, plusieurs menus correspondent.");

		for (const auto& match : matches)
			PrintUtils::addError(std::format(" - {} (Taper {})", match.label, match.key));
	}
	else
		PrintUtils::addError("Saisie invalide, veuillez réessayer.");
	return ("");
}

void				CLIUtils::handleTitle(void (*function)())
{
	PrintUtils::clear();
	TitleViewer::banner();
	
	if (function)
		function();
}

void				CLIUtils::waitForEnter()
{
	std::cout << "\nAppuyez sur Entree pour continuer...";
	input();
}
