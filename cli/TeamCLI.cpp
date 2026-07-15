//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

//	STDLIB
#include <iostream>

//	INCLUDES
#include "../includes/TeamCLI.hpp"

//	TYPEDEF

//	STATIC VARIABLES

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

void								TeamCLI::print(const Team& team)
{
	std::cout << "__________ TEAM DESCRIPTION __________" << std::endl;
	std::cout << "Team ID: " << team.getId() << std::endl;
	std::cout << "Nom: " << team.getName() << std::endl;
	
	if (team.getHasMultiTeamPlayer())
		std::cout << " [Contient un Multi-Joueur]" << std::endl;

	std::cout << "Membres : ";

	for (size_t i = 0; i < team.getMembers().size(); ++i)
	{
		std::cout << team.getMembers()[i]->getPseudo();

		if (team.getMembers()[i]->getIsMultiTeamPlayer())
			std::cout << " (Multi)";

		if (i < team.getMembers().size() - 1)
			std::cout << " & ";
	}

	std::cout << "\n______________________________________" << std::endl;
}
