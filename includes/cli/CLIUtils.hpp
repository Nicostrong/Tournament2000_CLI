//
// Created by Nicolas Fordoxcel on 26/08/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <span>
# include <string>
# include <vector>
# include <csignal>
# include <optional>
# include <exception>
# include <string_view>

#include "../Constantes.hpp"

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				StringV			=	std::string_view;

using				cvInt			=	const std::vector<int>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

extern volatile std::sig_atomic_t	g_running;

/****************************************************************************************************/
/*	STRUCT																							*/
/****************************************************************************************************/

struct				MenuItem
{
	char			key;
	String			label;
};

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

class				CLIInterrupted : public std::exception
{
	public:

		[[nodiscard]]
		const char*					what() const noexcept override
		{
			return ("CLI Interrompue par l'utilisateur.");
		}
};

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				CLIUtils
{

	private:

		
	public:

		// Input
		static void					clearInput();
		static void					checkInterruption();

		static String				input();
		static std::optional<int>	parseInt(StringV input);

		// Typed input
		static bool					askBool(StringV prompt, bool defaultValue);
		static String				askString(StringV prompt, StringV defaultValue);

		static int					askInt(StringV prompt, int min, int max, int defaultValue);
		static int					askIntList(StringV prompt, cvInt allowedValues, int defaultValue);

		// Menu
		static void					displayMenu(StringV title, std::span<const MenuItem> items);
		static char					askMenuChoice(std::span<const MenuItem> items);

		// UI
		static void					handleTitle(void (*function)());
		static void					waitForEnter();
		
};
