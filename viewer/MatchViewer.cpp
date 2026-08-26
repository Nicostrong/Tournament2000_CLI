//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iostream>

#include "../includes/viewer/MatchViewer.hpp"

#include "../includes/class/Match.hpp"
#include "../includes/class/Team.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				MatchViewer::display(cMatch match)
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

void				MatchViewer::displayAllMatches(cvpMatch matches)
{
	std::cout << std::string(108, '-') << '\n';
	std::cout << std::format(" {:>2} | {:<80} | {:>4} | {:>4} | {:<4}\n",
		"ID",
		"Rencontre",
		"ST 1",
		"ST 2",
		"Fini"
	);
	std::cout << std::string(108, '-') << '\n';

	int i = 0;
	for (cpMatch m : matches)
	{
		std::string encounter = std::format("{} vs {}", m->getTeamA()->getName(), m->getTeamB()->getName());
		std::string scoreA = m->isFinished() ? std::to_string(m->getScoreA()) : "-";
		std::string scoreB = m->isFinished() ? std::to_string(m->getScoreB()) : "-";
		std::string status = m->isFinished() ? "Oui" : "Non";
		
		std::cout << std::format(" {:>2} | {:<80} | {:>4} | {:>4} | {:<4}\n", i++, encounter, scoreA, scoreB, status);
	}
	std::cout << std::string(108, '-') << '\n';
}
