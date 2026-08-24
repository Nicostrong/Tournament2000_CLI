//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <iomanip>
#include <iostream>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"

#include "../includes/viewer/PoolViewer.hpp"
#include "../includes/viewer/TeamViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"

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
void				PoolViewer::writeMatches(std::ostream& out, cPool pool, cBool toFile)
{
	cvpMatch matches = pool.getMatches();

	if (matches.empty())
	{
		PrintUtils::addError("Aucun match enregistre.");
		return;
	}

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

			out << "  [ À jouer ]";

			if (!toFile)
				out << "\033[0m";
		}

		out << "\n";
	}
}

/**
 * Ecrit le tableau de classement de la poule dans le flux out.
 * Colonnes : Rang | Equipe | Pts | Diff
 * Si toFile = true : pas de codes couleur ANSI.
 */
void				PoolViewer::writeTable(std::ostream& out, cPool pool, cBool toFile)
{
	cvpTeam teams = pool.getTeams();

	if (teams.empty())
	{
		PrintUtils::addError("Aucune equipe dans cette poule.");
		return;
	}

	size_t maxLen = 6;

	for (const Team* t : teams)
		if (t->getName().size() > maxLen)
			maxLen = t->getName().size();

	cInt w = static_cast<int>(maxLen) + 2;

	out << "  " << std::left << std::setw(4)  << "#"
		<< std::setw(w)   << "Equipe"
		<< std::setw(6)   << "Pts"
		<< std::setw(8)   << "Diff"
		<< "\n";

	out << "  " << std::string(4 + w + 6 + 8, '-') << "\n";

	for (size_t i = 0; i < teams.size(); ++i)
	{
		cpTeam t = teams[i];
		cInt diff = t->getScoreDiff();
		cBool isTop2 = (i < 2);

		if (!toFile && isTop2)
			out << "\033[1;32m";

		out << "  " << std::left  << std::setw(4) << (i + 1)
			<< std::setw(w)  << t->getName()
			<< std::setw(6)  << t->getPoint()
			<< (diff >= 0 ? "+" : "") << diff
			<< "\n";

		if (!toFile && isTop2)
			out << "\033[0m";
	}
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 * Affiche la classement dans le terminal
 */
void				PoolViewer::displayTable(cPool pool)
{
	std::cout << "\n=== CLASSEMENT " << pool.getName() << " ===" << std::endl;
	std::cout << std::left << std::setw(15) << "Equipe" << " | " << "Points" << std::endl;
	std::cout << "--------------------------" << std::endl;

	for (const auto& team : pool.getTeams())
		std::cout << std::left << std::setw(15) << team->getName() << " | " << team->getPoint() << " pts" << std::endl;
}

/**
 * Affiche le classement enrichi avec points et difference de score.
 * Les 2 premiers qualifies sont mis en vert.
 */
void				PoolViewer::displayFullTable(cPool pool)
{
	std::cout << "\n=== CLASSEMENT " << pool.getName() << " ===\n";
	writeTable(std::cout, pool, false);
}

/**
 * Affiche liste des matchs
 */
void				PoolViewer::displayMatches(cPool pool)
{
	std::cout << "\n=== MATCHS " << pool.getName() << " ===" << std::endl;

	writeMatches(std::cout, pool, false);
}

/**
 * Affiche la composition detaillee des equipes
 */
void				PoolViewer::displayPoolDetails(cPool pool)
{
	std::cout << "\n============================================" << std::endl;
	std::cout << "   COMPOSITION DES EQUIPES - " << pool.getName() << std::endl;
	std::cout << "============================================" << std::endl;

	for (cpTeam t : pool.getTeams())
		TeamViewer::showTeamDescription(*t);
}
