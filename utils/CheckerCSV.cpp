//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "../includes/utils/CheckerCSV.hpp"
#include "../includes/utils/PrintUtils.hpp"
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

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

bool				CheckerCSV::validatePlayerCSV(cString filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		PrintUtils::addError(std::format("Impossible d'ouvrir le fichier : {}", filename));
		return (false);
	}

	String line;
	int lineNumber = 0;
	bool isValid = true;

	while (std::getline(file, line))
	{
		lineNumber++;
		
		FormatUtils::trim(line);
		
		if (line.empty())
			continue;

		size_t commaCount = std::ranges::count(line.begin(), line.end(), ',');

		if (commaCount != 3)
		{
			PrintUtils::addError(std::format("Ligne {} : Structure incorrecte. Attendu : 4 colonnes, Trouve : {} colonnes.", lineNumber, (commaCount + 1)));
			isValid = false;

			continue;
		}

		std::stringstream ssLine(line);
		String cell;
		bool hasEmptyField = false;

		while (std::getline(ssLine, cell, ','))
		{
			FormatUtils::trim(cell);

			if (cell.empty())
				hasEmptyField = true;
		}
		
		if (line.back() == ',')
			hasEmptyField = true;

		if (hasEmptyField)
		{
			PrintUtils::addError(std::format("Ligne {}: Un des champs obligatoires est vide", lineNumber ));
			isValid = false;
		}
	}

	file.close();
	
	return (isValid);
}