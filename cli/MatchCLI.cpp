//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

//	STDLIB
#include <iostream>
#include <limits>

//	INCLUDES
#include "../includes/MatchCLI.hpp"

//	TYPEDEF

//	STATIC VARIABLES

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

void								MatchCLI::display(const Match& match)
{
	std::cout << match.getTeamA()->getName() << " vs " << match.getTeamB()->getName();

	if (match.isFinished())
	{
		std::cout << " [ SCORE : " << match.getScoreA() << " - " << match.getScoreB() << " ]";

		if (match.getWinner())
			std::cout << " -> Vainqueur : " << match.getWinner()->getName();
	}
	else
		std::cout << " [ MATCH A VENIR ]";

	std::cout << std::endl;
}

void								MatchCLI::inputScore(Match& match)
{
	int sA = -1;
	int sB = -1;

	std::cout << "\n--- Saisie du score ---" << std::endl;

	while (sA < 0)
	{
		std::cout << "Score de " << match.getTeamA()->getName() << " : ";

		if (!(std::cin >> sA))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			sA = -1;
		}
	}

	while (sB < 0)
	{
		std::cout << "Score de " << match.getTeamB()->getName() << " : ";

		if (!(std::cin >> sB))
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			sB = -1;
		}
	}

	match.setScore(sA, sB);
	std::cout << "Score enregistre !" << std::endl;
}