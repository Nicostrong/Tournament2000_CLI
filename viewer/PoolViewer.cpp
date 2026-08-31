//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iomanip>
#include <iostream>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

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

	constexpr int TABLE_WIDTH = 80;
	constexpr int W_RANK  = 4;
	constexpr int W_PTS   = 5;
	constexpr int W_PLUS  = 5;
	constexpr int W_MINUS = 5;
	constexpr int W_DIFF  = 6;

	constexpr int FIXED_CHARS = 1 + W_RANK + W_PTS + W_PLUS + W_MINUS + W_DIFF + (5 * 3) + 1;
	constexpr int W_TEAM = TABLE_WIDTH - FIXED_CHARS;

	std::string separator = "+" + std::string(TABLE_WIDTH - 2, '-') + "+";

	out << separator << "\n";
	out << std::format("| {:^{}} | {:<{}} | {:^{}} | {:^{}} | {:^{}} | {:^{}} |\n",
		"#", W_RANK,
		"Equipe", W_TEAM,
		"Pts", W_PTS,
		"P+", W_PLUS,
		"P-", W_MINUS,
		"Diff", W_DIFF);
	out << separator << "\n";

	for (size_t i = 0; i < teams.size(); ++i)
	{
		cpTeam t = teams[i];

		if (!t)
			continue;

		cInt diff = t->getScoreDiff();
		cBool isTop2 = (i < 2);
		std::string teamName = t->getName();

		if (teamName.length() > static_cast<size_t>(W_TEAM))
			teamName = teamName.substr(0, W_TEAM - 3) + "...";

		std::string diffStr = std::format("{}{}", (diff > 0 ? "+" : ""), diff);

		if (!toFile && isTop2)
			out << "\033[1;32m";

		out << std::format("| {:^{}} | {:<{}} | {:^{}} | {:^{}} | {:^{}} | {:^{}} |\n",
			i + 1, W_RANK,
			teamName, W_TEAM,
			t->getPoint(), W_PTS,
			t->getScoreMarked(), W_PLUS,
			t->getScoreAgainst(), W_MINUS,
			diffStr, W_DIFF);

		if (!toFile && isTop2)
			out << "\033[0m";
	}

	out << separator << "\n";
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

void				PoolViewer::displayPoolsList(cTour tournament)
{
	std::cout << std::string(44, '-') << '\n';
	std::cout << std::format("| {:<4} | {:<20} | {:<10} |\n", "ID", "Nom", "Terminée");
	std::cout << std::string(44, '-') << '\n';

	int i = 1;

	for (cpPool pool : tournament.getPools())
		std::cout << std::format("| {:<4} | {:<20} | {:<10} |\n",
			i++,
			pool->getName(),
			pool->getIsFinished() ? "Oui" : "Non");
	
	std::cout << std::string(44, '-') << "\n\n";
}

void				PoolViewer::displayTeamsInPool(cPool pool)
{
	std::cout << std::format("\n=== EQUIPES DE LA POOL : {} ===\n", pool.getName());

	for (const auto& team : pool.getTeams())
		std::cout << std::format("- {}\n", team->getName());
}

void				PoolViewer::displayMatchesInPool(cPool pool)
{
	std::cout << std::format("\n=== MATCHS DE LA POOL : {} ===\n", pool.getName());
	std::cout << std::format("| {:<35} | {:<10} | {:<10} |\n", "Rencontre", "Résultat", "Terminé");
	std::cout << std::string(65, '-') << '\n';

	for (const auto& match : pool.getMatches())
	{
		std::string encounter = std::format("{} vs {}", match->getTeamA()->getName(), match->getTeamB()->getName());
		std::string score = match->isFinished() ? std::format("{} - {}", match->getScoreA(), match->getScoreB()) : "- : -";
		std::string status = match->isFinished() ? "Oui" : "Non";
		
		std::cout << std::format("| {:<35} | {:<10} | {:<10} |\n", encounter, score, status);
	}
}

void				PoolViewer::displayPoolStandings(cPool pool)
{
	std::cout << std::format("\n=== CLASSEMENT DE LA POOL : {} ===\n", pool.getName());
	std::cout << std::format("| {:<3} | {:<20} | {:<3} | {:<3} | {:<3} | {:<5} | {:<10} |\n",
	"Pos", "Equipe", "Pts", "BP", "BC", "Diff", "Qualifié");
	std::cout << std::string(72, '-') << '\n';

	/*int position = 1;

	for (const auto& stat : poolgetStandings())
	{
		std::string qualified = stat.isQualified() ? "Oui" : "Non";
		
		std::cout << std::format("| {:<3} | {:<20} | {:<3} | {:<3} | {:<3} | {:<5} | {:<10} |\n", 
								position++, 
								stat.getTeam().getName(), 
								stat.getPoints(), 
								stat.getGoalsScored(), 
								stat.getGoalsConceded(), 
								stat.getGoalDifference(), 
								qualified);
	}*/
}