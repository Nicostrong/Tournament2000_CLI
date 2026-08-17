//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

//	STDLIB
#include <string>
#include <vector>
#include <format>
#include <fstream>
#include <sstream>
#include <algorithm>

//	INCLUDES
#include "../includes/utils/CheckerCSV.hpp"
#include "../includes/utils/FormatUtils.hpp"
#include "../includes/utils/PrintUtils.hpp"
#include "../includes/cli/ParticipantCLI.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				V_STRING	=	std::vector<std::string>;

//	STATIC VARIABLES

/****************/
/*	CANONICAL	*/
/****************/

/************/
/*	GETTER	*/
/************/

/************/
/*	SETTER	*/
/************/

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

bool								CheckerCSV::validateParticipantCSV(C_STRING filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		PrintUtils::addError(std::format("Impossible d'ouvrir le fichier : {}", filename));
		return (false);
	}

	STRING line;
	int lineNumber = 0;
	bool isValid = true;

	while (std::getline(file, line))
	{
		lineNumber++;
		
		FormatUtils::trim(line);
		
		if (line.empty())
			continue ;

		size_t commaCount = std::count(line.begin(), line.end(), ',');

		if (commaCount != 3)
		{
			std::stringstream ss;

			//ss << "Ligne " << lineNumber << " : Structure incorrecte. Attendu : 4 colonnes, Trouve : " 
			//<< (commaCount + 1) << " colonnes.";
			
			PrintUtils::addError(std::format("Ligne {} : Structure incorrecte. Attendu : 4 colonnes, Trouve : {} colonnes.", lineNumber, (commaCount + 1)));
			isValid = false;

			continue;
		}

		std::stringstream ssLine(line);
		STRING cell;
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
			std::stringstream ss;

			//ss << "Ligne " << lineNumber << " : Un des champs obligatoires est vide.";
			
			PrintUtils::addError(std::format("Ligne {}: Un des champs obligatoires est vide", lineNumber ));
			isValid = false;
		}
	}

	file.close();
	
	return (isValid);
}