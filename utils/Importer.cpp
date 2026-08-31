//
// Created by Nicolas Fordoxcel on 18/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <memory>
#include <fstream>
#include <sstream>

#include "../includes/class/Player.hpp"

#include "../includes/manager/PlayerManager.hpp"

#include "../includes/utils/Importer.hpp"
#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/CheckerCSV.hpp"
#include "../includes/utils/FormatUtils.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

uPlayer				Importer::extractPlayerFromLine(cString line, cBool isFirstLine)
{
	std::stringstream ss(line);
	String pseudo;
	String firstName;
	String lastName;
	String genderStr;

	std::getline(ss, pseudo, ',');
	std::getline(ss, lastName, ',');
	std::getline(ss, firstName, ',');
	std::getline(ss, genderStr, ',');

	FormatUtils::trim(pseudo);
	FormatUtils::trim(lastName);
	FormatUtils::trim(firstName);
	FormatUtils::trim(genderStr);

	if (isFirstLine && pseudo == "pseudo")
		return (nullptr);

	if (pseudo.empty() || lastName.empty() || firstName.empty() || genderStr.empty())
		return (nullptr);

	Gender gender = Gender::MALE;

	if (genderStr == "0")
		gender = Gender::MALE;
	else if (genderStr == "1")
		gender = Gender::FEMALE;
	else
		return (nullptr);

	return (std::make_unique<Player>(pseudo, lastName, firstName, gender));
}

/**************************************************************************************************/
/*	PUBLIC METHOD																				  */
/**************************************************************************************************/

int					Importer::importPlayers(cString path, PlayerManager& playerManager)
{
	if (!CheckerCSV::validatePlayerCSV(path))
	{
		PrintUtils::addError("Le fichier CSV des participants est invalide.");
		return (0);
	}

	std::ifstream file(path);

	if (!file.is_open())
	{
		PrintUtils::addError("Impossible d'ouvrir le fichier CSV.");
		return (0);
	}

	String line;
	bool isFirstLine = true;
	int imported = 0;
	int skipped = 0;

	while (std::getline(file, line))
	{
		FormatUtils::trim(line);

		if (line.empty())
			continue;

		uPlayer player = extractPlayerFromLine(line, isFirstLine);

		isFirstLine = false;

		if (!player)
		{
			++skipped;
			continue;
		}

		if (playerManager.addPlayer(std::move(player)))
			++imported;
		else
			++skipped;
	}

	file.close();

	if (imported > 0)
		PrintUtils::addSuccess(std::format("{} participant(s) importe(s) et {} participant(s) ignore(s).", imported, skipped));
	else
		PrintUtils::addError("Aucun participant importe. Verifiez le fichier.");

	return (imported);
}