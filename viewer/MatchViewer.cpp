//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iomanip>
#include <iostream>

#include "../includes/viewer/MatchViewer.hpp"

#include "../includes/cli/CLIUtils.hpp"

#include "../includes/utils/PrintUtils.hpp"

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"

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

/**
 * Ecrit la liste des matchs de la poule dans le flux out.
 * Si toFile = true : pas de codes couleur ANSI.
 */
void				MatchViewer::writeMatches(std::ostream& out, cPool pool, cBool toFile)
{
	cvpMatch matches = pool.getMatches();

	if (matches.empty())
		return (PrintUtils::addError("Aucun match enregistre."));
	int i = 1;

	for (cpMatch m : matches)
	{
		if (!m)
			continue;

		out << "  " << std::setw(2) << i++ << ". ";
		out << m->getTeamA()->getName() << " vs " << m->getTeamB()->getName();

		if (m->isFinished())
		{
			out << "  [ " << m->getScoreA() << " - " << m->getScoreB() << " ]";

			if (m->getWinner())
			{
				if (!toFile)
					out << "\033[1;32m";

				out << "  ->  Vainqueur : " << m->getWinner()->getName();

				if (!toFile)
					out << "\033[0m";
			}
		}
		else
		{
			if (!toFile)
				out << "\033[1;33m";

			out << "  [ A jouer ]";

			if (!toFile)
				out << "\033[0m";
		}

		out << "\n";
	}
}

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
	std::cout << "+" << std::string(78, '-') << "+\n";
	std::cout << std::format("| {:>2} | {:<44} | {:>4} | {:>4} | {:<4} |\n",
		"ID",
		"Rencontre",
		"ST 1",
		"ST 2",
		"Fini"
	);
	std::cout << "+" << std::string(78, '-') << "+\n";

	int i = 0;
	for (cpMatch m : matches)
	{
		std::string encounter = std::format("{:<20.20} vs {:>20.20}", m->getTeamA()->getName(), m->getTeamB()->getName());
		std::string scoreA = m->isFinished() ? std::to_string(m->getScoreA()) : "-";
		std::string scoreB = m->isFinished() ? std::to_string(m->getScoreB()) : "-";
		std::string status = m->isFinished() ? "Oui" : "Non";
		
		std::cout << std::format("| {:>2} | {:^44.44} | {:>4} | {:>4} | {:<4} |\n", i++, encounter, scoreA, scoreB, status);
	}
	std::cout << "+" << std::string(72, '-') << "+\n";
}

/**
 * Affiche l'etat d'avancement des rencontres dans une poule
 */
void				MatchViewer::displayMatchesInPool(cPool pool)
{
	CLIUtils::displayTitle(std::format("MATCHS DE LA POOL : {}", pool.getName()));
	std::cout << std::format("| {:<35} | {:<10} | {:<10} |\n", "Rencontre", "Resultat", "Termine");
	std::cout << std::string(65, '-') << '\n';

	for (const auto& match : pool.getMatches())
	{
		std::string encounter = std::format("{} vs {}", match->getTeamA()->getName(), match->getTeamB()->getName());
		std::string score = match->isFinished() ? std::format("{} - {}", match->getScoreA(), match->getScoreB()) : "- : -";
		std::string status = match->isFinished() ? "Oui" : "Non";
		
		std::cout << std::format("| {:<35} | {:<10} | {:<10} |\n", encounter, score, status);
	}
}

/**
 * Affiche liste des matchs
 */
void				MatchViewer::displayMatches(cPool pool)
{
	CLIUtils::displayTitle(std::format("MATCHS {}", pool.getName()));
	writeMatches(std::cout, pool, false);
}
