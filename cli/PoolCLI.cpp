//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

//	STDLIB
#include <iostream>
#include <iomanip>
#include <fstream>

//	INCLUDE
#include "../includes/cli/PoolCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"
#include "../includes/cli/TeamCLI.hpp"

//	TYPEDEF
using				C_STRING	=	const std::string&;
using				CVP_PART	=	const std::vector<Participant*>&;
using               CVP_TEAM	=	const std::vector<Team*>&;
using				CVP_MATCH	=	const std::vector<Match*>&;

//	STATIC VARIABLES

/********************/
/*	PRIVATE METHOD	*/
/********************/

/**
 * Ecrit la liste des matchs de la poule dans le flux out.
 * Si toFile = true : pas de codes couleur ANSI.
 */
void				PoolCLI::writeMatches(std::ostream& out, const Pool& pool, const bool toFile)
{
	CVP_MATCH		matches = pool.getMatches();

	if (matches.empty())
	{
		out << "  Aucun match enregistré.\n";
		return ;
	}

	int				i = 1;

	for (const Match* m : matches)
	{
		if (!m)
			continue ;

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
void				PoolCLI::writeTable(std::ostream& out, const Pool& pool, const bool toFile)
{
	CVP_TEAM		teams = pool.getTeams();

	if (teams.empty())
	{
		out << "  Aucune équipe dans cette poule.\n";
		return ;
	}

	size_t			maxLen = 6;

	for (const Team* t : teams)
		if (t->getName().size() > maxLen)
			maxLen = t->getName().size();

	const int		w = static_cast<int>(maxLen) + 2;

	out << "  " << std::left << std::setw(4)  << "#"
		<< std::setw(w)   << "Equipe"
		<< std::setw(6)   << "Pts"
		<< std::setw(8)   << "Diff"
		<< "\n";

	out << "  " << std::string(4 + w + 6 + 8, '-') << "\n";

	for (size_t i = 0; i < teams.size(); ++i)
	{
		const Team*	t = teams[i];
		const int	diff = t->getScoreDiff();
		const bool	isTop2 = (i < 2);

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

/********************/
/*	PUBLIC METHOD	*/
/********************/

/**
 * Affiche la classement dans le terminal
 */
void				PoolCLI::displayTable(const Pool& pool)
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
void				PoolCLI::displayFullTable(const Pool& pool)
{
	std::cout << "\n=== CLASSEMENT " << pool.getName() << " ===\n";
	writeTable(std::cout, pool, false);
}

/**
 * Affiche liste des matchs
 */
void				PoolCLI::displayMatches(const Pool& pool)
{
	std::cout << "\n=== MATCHS " << pool.getName() << " ===" << std::endl;

	writeMatches(std::cout, pool, false);
}

/**
 * Affiche la composition detaillee des equipes
 */
void				PoolCLI::displayPoolDetails(const Pool& pool)
{
	std::cout << "\n============================================" << std::endl;
	std::cout << "   COMPOSITION DES EQUIPES - " << pool.getName() << std::endl;
	std::cout << "============================================" << std::endl;

	for (const Team* t : pool.getTeams())
		TeamCLI::print(*t);
}

/**
 * Exporte l historique complet de la poule (matchs + classement) dans un .txt.
 */
bool				PoolCLI::exportToTxt(const Pool& pool, C_STRING filename)
{
	std::ofstream	file(filename);

	if (!file.is_open())
	{
		std::cerr << "\033[1;31m[!] Impossible de créer : " << filename << "\033[0m\n";
		return (false);
	}

	file << "============================================================\n";
	file << "  POULE : " << pool.getName() << "\n";
	file << "============================================================\n";

	file << "\n[EQUIPES]\n";

	for (const Team* t : pool.getTeams())
	{
		file << "  - " << t->getName();

		if (t->getHasMultiTeamPlayer())
			file << " [Multi-joueur]";

		file << " : ";

		CVP_PART	members = t->getMembers();

		for (size_t i = 0; i < members.size(); ++i)
		{
			file << members[i]->getPseudo();

			if (members[i]->getIsMultiTeamPlayer())
				file << " (Multi)";

			if (i + 1 < members.size())
				file << " & ";
		}

		file << "\n";
	}

	file << "\n[MATCHS]\n";
	writeMatches(file, pool, true);

	file << "\n[CLASSEMENT FINAL]\n";
	writeTable(file, pool, true);

	file << "\n============================================================\n";
	file.close();

	return (true);
}