//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

//	STDLIB
#include <iostream>
#include <limits>

//	INCLUDES
#include "../includes/ParticipantCLI.hpp"

//	TYPEDEF
typedef std::string					STRING;

//	STATIC VARIABLES

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

Participant							ParticipantCLI::create()
{
	STRING	pseudo;
	STRING	lastName;
	STRING	firstName;
	int		gender;

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

	gender = -1;

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

void								ParticipantCLI::print(const Participant& participant)
{
	std::cout << std::endl;
	std::cout << "===== PARTICIPANT =====" << std::endl;
	std::cout << "Pseudo : " << participant.getPseudo() << std::endl;
	std::cout << "Nom : " << participant.getLastName() << std::endl;
	std::cout << "Prenom : " << participant.getFirstName() << std::endl;
	std::cout << "Sexe : " << participant.getGenderStr() << std::endl;
	std::cout << "=======================" << std::endl;
}

STRING								ParticipantCLI::trim(STRING s)
{
    size_t start = s.find_first_not_of(" \t\n\r\f\v");

    if (start == std::string::npos)
        return ("");

    size_t end = s.find_last_not_of(" \t\n\r\f\v");

    return (s.substr(start, end - start + 1));
}