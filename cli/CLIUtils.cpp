//
// Created by Nicolas Fordoxcel on 26/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <limits>
#include <iostream>
#include <exception>
#include <algorithm>

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/cli/CLIUtils.hpp"

#include "../includes/Color.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

void				CLIUtils::displayTitle(StringV title)
{
	size_t lenTitle = title.size();
	int nbChar = (60 - static_cast<int>(lenTitle) - 2) / 2;

	std::cout << std::endl;

	for (int i = 0; i < nbChar; ++i)
		std::cout << "=";

	std::cout << " " << title << " ";

	for (int i = 0; i < nbChar; ++i)
		std::cout << "=";
	
	std::cout << std::endl;
}

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
		const int value = std::stoi(String(input), &pos);

		if (pos != input.size())
			return (std::nullopt);

		return (value);
	} 
	catch (const std::exception&) 
	{
		return (std::nullopt);
	}
}

bool				CLIUtils::askBool(StringV prompt, bool defaultValue)
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
			
		PrintUtils::addError("Veuillez repondre par O(ui) ou N(on).");
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
				
			PrintUtils::addError("La saisie ne peut pas etre vide.");
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
		
		PrintUtils::addError(std::format("Veuillez entrer un nombre entier compris entre {} et {}.", min, max));
	}
}

int					CLIUtils::askIntList(StringV prompt, cvInt allowedValues, int defaultValue)
{
	while (true)
	{
		checkInterruption();

		std::cout << std::format("{} (defaut: {}) : ", prompt, defaultValue);
		String res = input();
		
		if (res.empty())
			return (defaultValue);
			
		auto parsed = parseInt(res);

		if (parsed.has_value())
			if (std::find(allowedValues.begin(), allowedValues.end(), parsed.value()) != allowedValues.end())
				return (parsed.value());
		
		PrintUtils::addError("Cette valeur n'est pas dans la liste autorisee.");
	}
}

void				CLIUtils::displayMenu(StringV title, std::span<const MenuItem> items)
{
	displayTitle(title);

	for (const auto& item : items)
		std::cout << Color::BYELLOW << "\t" << item.key << "\t" << Color::RESET << item.label << std::endl;
	
	std::cout << "Entrer le numero ou la lettre correspondant a ce que vous voulez: ";
}

char				CLIUtils::askMenuChoice(std::span<const MenuItem> items)
{
	while (true)
	{
		std::cout << "Votre choix : ";
		String res = input();
		
		if (res.length() == 1)
		{
			char choice = static_cast<char>(std::toupper(res[0]));
			
			for (const auto& item : items)
				if (std::toupper(item.key) == choice)
					return (item.key);
		}
		
		PrintUtils::addError("Choix invalide. Veuillez selectionner une option du menu.");
	}
}

void				CLIUtils::waitForEnter()
{
	std::cout << "\nAppuyez sur Entree pour continuer...";
	input();
}
