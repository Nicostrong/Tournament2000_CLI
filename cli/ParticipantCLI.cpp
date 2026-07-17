//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

//	STDLIB
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <algorithm>

//	INCLUDES
#include "../includes/cli/ParticipantCLI.hpp"
#include "../includes/utils/FormatUtils.hpp"
#include "../includes/utils/CheckerCSV.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef std::vector<Participant>	V_PART;

//	STATIC VARIABLES

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

Participant							ParticipantCLI::create()
{
	STRING			pseudo;
	STRING			lastName;
	STRING			firstName;
	int				gender = -1;

	while (lastName.empty())
	{
		std::cout << "Nom : ";
		std::getline(std::cin, lastName);
		lastName = trim(lastName);
	}

	while (firstName.empty())
	{
		std::cout << "Prenom : ";
		std::getline(std::cin, firstName);
		firstName = trim(firstName);
	}

	while (pseudo.empty())
	{
		std::cout << "Pseudo : ";
		std::getline(std::cin, pseudo);
		pseudo = trim(pseudo);
	}

	while (gender != 0 && gender != 1)
	{
		std::cout << "Sexe (0 = HOMME, 1 = FEMME) : ";

		std::cin >> gender;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			gender = -1;
		}
	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return (Participant(pseudo, firstName, lastName, static_cast<Participant::Gender>(gender)));
}

STRING								ParticipantCLI::trim(STRING s)
{
    size_t start = s.find_first_not_of(" \t\n\r\f\v");

    if (start == std::string::npos)
        return ("");

    size_t end = s.find_last_not_of(" \t\n\r\f\v");

    return (s.substr(start, end - start + 1));
}

V_PART								ParticipantCLI::importFromCSV(C_STRING filename)
{
	V_PART							list;
	V_STRING						validationErrors;

	if (!CheckerCSV::validateParticipantCSV(filename, validationErrors))
	{
		std::cerr << "\n\033[1;31m[!] ERREUR : Le fichier CSV contient des erreurs de format.\033[0m\n";

		for (const auto& err : validationErrors)
			std::cerr << "  -> " << err << "\n";

		return (list);
	}

	std::ifstream					file(filename);

	if (!file.is_open()) 
		return (list);

	STRING							line;
	bool							isFirstLine = true;

	while (std::getline(file, line))
	{
		if (FormatUtils::trim(line).empty())
			continue ;

		std::stringstream			ss(line);
		STRING						pseudo;
		STRING						firstName;
		STRING						lastName;
		STRING						genderStr;

		std::getline(ss, pseudo, ',');
		std::getline(ss, firstName, ',');
		std::getline(ss, lastName, ',');
		std::getline(ss, genderStr, ',');

		pseudo = FormatUtils::trim(pseudo);
		firstName = FormatUtils::trim(firstName);
		lastName = FormatUtils::trim(lastName);
		genderStr = FormatUtils::trim(genderStr);

		std::string					pLower = pseudo;

		std::transform(pLower.begin(), pLower.end(), pLower.begin(), ::tolower);

		if (isFirstLine && (pLower == "pseudo" || pLower == "nom" || pLower == "prenom"))
		{
			isFirstLine = false;
			continue;
		}
		isFirstLine = false;

		Participant::Gender			gender = Participant::MALE;
		STRING						gLower = genderStr;

		std::transform(gLower.begin(), gLower.end(), gLower.begin(), ::tolower);

		if (gLower == "1" || gLower == "f" || gLower == "femme")
			gender = Participant::FEMALE;

		list.push_back(Participant(pseudo, firstName, lastName, gender));
	}

	file.close();
	
	return (list);
}

bool								ParticipantCLI::exportToCSV(const V_PART& participants, C_STRING filename)
{
	std::ofstream					file(filename);

	if (!file.is_open())
	{
		std::cerr << "Erreur : Impossible de creer le fichier " << filename << std::endl;
		return (false);
	}

	file << "pseudo,prenom,nom,sexe\n";

	for (const auto& p : participants)
		file << p.getPseudo() << "," << p.getFirstName() << "," << p.getLastName() << ","
			 << (p.getGenderInt() == Participant::MALE ? "0" : "1") << "\n";

	file.close();
	
	return (true);
}

std::ostream&		operator<<(std::ostream& os, const Participant& p)
{
	os << std::format(
		"-----------------------------\n"
		"|  Participant\n"
		"-----------------------------\n"
		"|  Pseudo             : {}\n"
		"|  Prenom             : {}\n"
		"|  Nom                : {}\n"
		"|  Genre              : {}\n"
		"|  Elimine            : {}\n"
		"|  Multi-equipe       : {}\n"
		"-----------------------------\n",
		p.getPseudo(),
		p.getFirstName(),
		p.getLastName(),
		p.getGenderStr(),
		p.getIsEliminated() ? "Oui" : "Non",
		p.getIsMultiTeamPlayer() ? "Oui" : "Non"
	);

	return (os);
}