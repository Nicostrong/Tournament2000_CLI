//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

//	STDLIB
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

//	INCLUDES
#include "../includes/utils/CheckerCSV.hpp"
#include "../includes/utils/FormatUtils.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef std::vector<std::string>	V_STRING;

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

bool								CheckerCSV::validateParticipantCSV(C_STRING filename, V_STRING& errorMessages)
{
	std::ifstream					file(filename);

	if (!file.is_open())
	{
		errorMessages.push_back("Impossible d'ouvrir le fichier : " + filename);
		
		return (false);
	}

	STRING							line;
	int								lineNumber = 0;
	bool							isValid = true;

	while (std::getline(file, line))
	{
		lineNumber++;
		
		STRING						trimmed = FormatUtils::trim(line);
		
		if (trimmed.empty())
			continue ;

		size_t						commaCount = std::count(trimmed.begin(), trimmed.end(), ',');

		if (commaCount != 3)
		{
			std::stringstream		ss;

			ss << "Ligne " << lineNumber << " : Structure incorrecte. Attendu : 4 colonnes, Trouve : " 
			<< (commaCount + 1) << " colonnes.";
			
			errorMessages.push_back(ss.str());
			isValid = false;

			continue;
		}

		std::stringstream			ssLine(trimmed);
		STRING						cell;
		bool						hasEmptyField = false;

		while (std::getline(ssLine, cell, ','))
			if (FormatUtils::trim(cell).empty())
				hasEmptyField = true;
		
		if (trimmed.back() == ',')
			hasEmptyField = true;

		if (hasEmptyField)
		{
			std::stringstream		ss;

			ss << "Ligne " << lineNumber << " : Un des champs obligatoires est vide.";
			
			errorMessages.push_back(ss.str());
			isValid = false;
		}
	}

	file.close();
	
	return (isValid);
}