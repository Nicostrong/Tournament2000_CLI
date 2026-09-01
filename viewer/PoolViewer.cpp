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

#include "../includes/cli/CLIUtils.hpp"

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

/**
 * Ecrit le tableau de classement SIMPLIFIÉ de la poule dans le flux out.
 * Colonnes : Rang | Equipe | Pts
 * Les 2 premières équipes sont mises en évidence.
 */
void				PoolViewer::writeSimpleTable(std::ostream& out, cPool pool, cBool toFile)
{
	cvpTeam teams = pool.getTeams();

	if (teams.empty())
		return (PrintUtils::addError("Aucune equipe dans cette poule."));

	constexpr int TABLE_WIDTH = 45;
	constexpr int W_RANK  = 3;
	constexpr int W_PTS   = 4;
	
	constexpr int FIXED_CHARS = 1 + W_RANK + W_PTS + (2 * 3) + 1;
	constexpr int W_TEAM = TABLE_WIDTH - FIXED_CHARS;

	std::string separator = "+" + std::string(TABLE_WIDTH, '-') + "+";

	out << separator << "\n";
	out << std::format("| {:^{}} | {:<{}} | {:^{}} |\n",
		"#", W_RANK,
		"Equipe", W_TEAM,
		"Pts", W_PTS);
	out << separator << "\n";

	for (size_t i = 0; i < teams.size(); ++i)
	{
		cpTeam t = teams[i];

		if (!t)
			continue;

		cBool isTop2 = (i < 2);
		std::string teamName = t->getName();

		if (teamName.length() > static_cast<size_t>(W_TEAM))
			teamName = teamName.substr(0, W_TEAM - 3) + "...";

		if (!toFile && isTop2)
			out << "\033[1;32m";

		out << std::format("| {:^{}} | {:<{}} | {:^{}} |\n",
			i + 1, W_RANK,
			teamName, W_TEAM,
			t->getPoint(), W_PTS);

		if (!toFile && isTop2)
			out << "\033[0m";
	}

	out << separator << "\n";
}

/**
 * Ecrit le tableau de classement COMPLET de la poule dans le flux out.
 * Colonnes : Rang | Equipe | Pts | P+ | P- | Diff
 * Les 2 premieres equipes sont mises en evidence.
 */
void				PoolViewer::writeCompleteTable(std::ostream& out, cPool pool, cBool toFile)
{
	cvpTeam teams = pool.getTeams();

	if (teams.empty())
		return (PrintUtils::addError("Aucune equipe dans cette poule."));

	constexpr int TABLE_WIDTH = 80;
	constexpr int W_RANK  = 3;
	constexpr int W_PTS   = 4;
	constexpr int W_PLUS  = 5;
	constexpr int W_MINUS = 5;
	constexpr int W_DIFF  = 6;

	constexpr int FIXED_CHARS = 1 + W_RANK + W_PTS + W_PLUS + W_MINUS + W_DIFF + (5 * 3) + 1;
	constexpr int W_TEAM = TABLE_WIDTH - FIXED_CHARS;

	std::string separator = "+" + std::string(TABLE_WIDTH, '-') + "+";

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
 * Affiche le classement SIMPLIFIE dans le terminal
 */
void				PoolViewer::displayTable(cPool pool)
{
	CLIUtils::displayTitle(std::format("CLASSEMENT {}", pool.getName()));
	writeSimpleTable(std::cout, pool, false);
}

/**
 * Affiche le classement COMPLET enrichi avec points et difference de score dans le terminal.
 */
void				PoolViewer::displayFullTable(cPool pool)
{
	CLIUtils::displayTitle(std::format("CLASSEMENT DETAILLE {}", pool.getName()));
	writeCompleteTable(std::cout, pool, false);
}

/**
 * Affiche liste des matchs
 */
void				PoolViewer::displayMatches(cPool pool)
{
	CLIUtils::displayTitle(std::format("MATCHS {}", pool.getName()));
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

/**
 * Affiche la liste des poules du tournoi
 */
void				PoolViewer::displayPoolsList(cTour tournament)
{
	std::cout << std::string(44, '-') << '\n';
	std::cout << std::format("| {:<4} | {:<20} | {:<10} |\n", "ID", "Nom", "Terminee");
	std::cout << std::string(44, '-') << '\n';

	int i = 1;

	for (cpPool pool : tournament.getPools())
		std::cout << std::format("| {:<4} | {:<20} | {:<10} |\n",
			i++,
			pool->getName(),
			pool->getIsFinished() ? "Oui" : "Non");
	
	std::cout << std::string(44, '-') << "\n\n";
}

/**
 * Affiche les equipes d'une poule
 */
void				PoolViewer::displayTeamsInPool(cPool pool)
{
	CLIUtils::displayTitle(std::format("EQUIPES DE LA POOL : {}", pool.getName()));

	for (const auto& team : pool.getTeams())
		std::cout << std::format("- {}\n", team->getName());
}

/**
 * Affiche l'etat d'avancement des rencontres dans une poule
 */
void				PoolViewer::displayMatchesInPool(cPool pool)
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
 *	TO DO
 *	
 *	liste des methodes:
 *	
 *	displayTable			=>	affiiche tableau simple (POOL)
 *	displayFullTable		=>	affiiche tableau complet (POOL)
 *	displayMatches			=>	affiiche synthese des matches [a jouer || resultat] (MATCH)
 *	displayPoolDetails		=>	affiiche la composition des teams (TEAM)
 *	displayPoolsList		=>	affiiche la liste des pool + etat (POOL)
 *	displayTeamsInPool		=>	affiiche liste des teams d une pool (TEAM)
 *	displayMatchesInPool	=>	affiiche tablean du 3 (MATCH)
 *
 *	faire le dispatching dans les differentes classes
 */