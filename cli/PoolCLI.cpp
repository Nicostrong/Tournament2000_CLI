//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

//	STDLIB
#include <iostream>
#include <iomanip>

//	INCLUDE
#include "../includes/cli/PoolCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"
#include "../includes/cli/TeamCLI.hpp"

//	TYPEDEF

//	STATIC VARIABLES

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

void								PoolCLI::displayTable(const Pool& pool)
{
	std::cout << "\n=== CLASSEMENT " << pool.getName() << " ===" << std::endl;
	std::cout << std::left << std::setw(15) << "Equipe" << " | " << "Points" << std::endl;
	std::cout << "--------------------------" << std::endl;

	for (const auto& team : pool.getTeams())
		std::cout << std::left << std::setw(15) << team->getName() << " | " << team->getPoint() << " pts" << std::endl;
}

void								PoolCLI::displayMatches(const Pool& pool)
{
	std::cout << "\n=== MATCHS " << pool.getName() << " ===" << std::endl;

	int i = 1;

	for (const auto& match : pool.getMatches())
	{
		std::cout << i++ << ". ";
		MatchCLI::display(*match);
	}
}

void								PoolCLI::displayPoolDetails(const Pool& pool)
{
	std::cout << "\n============================================" << std::endl;
	std::cout << "   COMPOSITION DES EQUIPES - " << pool.getName() << std::endl;
	std::cout << "============================================" << std::endl;

	for (const auto& teamPtr : pool.getTeams())
		TeamCLI::print(*teamPtr);
}