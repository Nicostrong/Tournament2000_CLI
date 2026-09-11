//
// Created by Nicolas Fordoxcel on 18/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <string>
#include <vector>
#include <format>
#include <iomanip>

#include "../includes/class/Team.hpp"
#include "../includes/class/Pool.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Player.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/cli/PhaseCLI.hpp"

#include "../includes/utils/Exporter.hpp"
#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/TablePrinter.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cSet			=	const Settings&;

using				pPlayer			=	Player*;
using				vpPlayer		=	std::vector<Player*>;
using				cvpPlayer		=	const std::vector<Player*>&;

using				pPool			=	Pool*;
using				cPool			=	const Pool&;
using				cpPool			=	const Pool*;
using				vpPool			=	std::vector<Pool*>;
using				cvpPool			=	const std::vector<Pool*>&;

using				pMatch			=	Match*;
using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

using				pPhase			=	Phase*;
using				cPhase			=	const Phase&;
using				cpPhase			=	const Phase*;
using				vpPhase			=	std::vector<Phase*>;
using				cvpPhase		=	const std::vector<Phase*>&;

using				pPlayer			=	Player*;
using				cPlayer			=	const Player&;
using				cpPlayer			=	const Player*;
using				vpPlayer			=	std::vector<Player*>;
using				cvpPlayer			=	const std::vector<Player*>&;

using				pTour			=	Tournament*;
using				cTour			=	const Tournament&;
using				cpTour			=	const Tournament*;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/************/
/*  HELPERS	*/
/************/

/*void				Exporter::writeHeader(std::ofstream& out, cTour tournament)
{
	cSet s = tournament.getSettings();

	out << "############################################################\n";
	out << "##\t\tTOURNOI :\t" << s.getName() << "\n";
	out << "############################################################\n\n";

	out << "\tType\t\t\t:\t";

	if (s.getIsDouble() && s.getIsMixed())
		out << "Double mixte\n";
	else if (s.getIsDouble())
		out << "Double\n";
	else
		out << "Simple\n";

	out << "\tJoueurs\t\t\t:\t" << s.getNbPlayers() << "\n";
	out << "\tPoules\t\t\t:\t" << s.getNbPools() << " x " << s.getNbPlayerByPool() << " equipes\n";
	out << "\tScore min/max\t:\t" << s.getScoreMin() << " / " << s.getScoreMax()
		<< " (ecart " << s.getDiffPointsToWin() << ")\n";
	out << "\tMulti-team\t\t:\t" << (s.getAllowMultiTeamPlayers() ? "Oui" : "Non") << "\n";
	out << "\tPetite finale\t:\t" << (s.getIsThirdPlaceMatch()    ? "Oui" : "Non") << "\n\n";
}*/

/*void				Exporter::writePools(std::ofstream& out, cTour tournament)
{
	cvpPool pools = tournament.getPools();

	out << "============================================================\n";
	out << "\tPHASE DE POULES\n";
	out << "============================================================\n";

	if (pools.empty())
	{
		PrintUtils::addError("Aucune poule enregistree.");
		return;
	}

	for (cpPool pool : pools)
	{
		if (!pool)
			continue;

		out << "\n------------------------------------------------------------\n";
		out << "\t" << pool->getName() << "\n";
		out << "------------------------------------------------------------\n";

		writePoolMatches(out, *pool);
		writePoolStandings(out, *pool);
	}

	out << "\n";
}*/

/*void				Exporter::writePoolMatches(std::ofstream& out, cPool pool)
{
	out << "\n\t[MATCHS]\n";

	int idx = 1;

	for (cpMatch m : pool.getMatches())
	{
		if (!m)
			continue;

		out << "\t\t" << std::setw(2) << idx++ << ". "
			<< m->getTeamA()->getName() << " vs " << m->getTeamB()->getName();

		if (m->isFinished())
		{
			out << "\t[ " << m->getScoreA() << " - " << m->getScoreB() << " ]";

			if (m->getWinner())
				out << "  ->  " << m->getWinner()->getName();
		}
		else
			out << "\t[ A jouer ]";

		out << "\n";
	}
}*/

/*void				Exporter::writePoolStandings(std::ofstream& out, cPool pool)
{
	cvpTeam teams = pool.getTeams();
	size_t maxLen = 6;

	for (cpTeam t : teams)
		if (t->getName().size() > maxLen)
			maxLen = t->getName().size();

	cInt w = static_cast<int>(maxLen) + 2;

	out << "\n\t[CLASSEMENT]\n";
	out << "\t\t" << std::left << std::setw(4) << "#"
		<< std::setw(w) << "Equipe"
		<< std::setw(6) << "Pts"
		<< "Diff\n";
	out << "\t\t" << String(4 + w + 12, '-') << "\n";

	for (size_t i = 0; i < teams.size(); ++i)
	{
		cInt diff = teams[i]->getScoreDiff();

		out << "\t\t" << std::left << std::setw(4) << (i + 1)
			<< std::setw(w) << teams[i]->getName()
			<< std::setw(6) << teams[i]->getPoint()
			<< (diff >= 0 ? "+" : "") << diff << "\n";
	}
}*/

/*void				Exporter::writeEncounterBlock(std::ofstream& out, cvpMatch matches,
						const size_t startIdx, cInt nbSets, cInt encounterNum)
{
	if (startIdx >= matches.size() || !matches[startIdx])
		return;

	cpMatch first = matches[startIdx];
	cString nameA = first->getTeamA() ? first->getTeamA()->getName() : "?";
	cString nameB = first->getTeamB() ? first->getTeamB()->getName() : "?";

	out << "\n\tRencontre " << encounterNum << " :  " << nameA << "  vs  " << nameB << "\n";
	out << "\t" << String(nameA.size() + nameB.size() + 14, '-') << "\n";

	int winsA = 0;
	int winsB = 0;

	for (int s = 0; s < nbSets; ++s)
	{
		const size_t idx = startIdx + static_cast<size_t>(s);

		if (idx >= matches.size() || !matches[idx])
			break;

		cpMatch m = matches[idx];

		out << "\tSet " << (s + 1) << " : ";

		if (!m->isFinished())
		{
			out << "[ a jouer ]\n";
			continue;
		}

		out << std::setw(3) << m->getScoreA() << " - " << std::setw(3) << m->getScoreB();

		if (m->getWinner())
		{
			out << "  ->  " << m->getWinner()->getName();

			if (m->getWinner() == first->getTeamA())
				winsA++;
			else
				winsB++;
		}

		out << "\n";
	}

	out << "\tVainqueur : ";

	if (winsA > winsB)
		out << nameA;
	else if (winsB > winsA)
		out << nameB;
	else
		out << "Non determine";

	out << "\n  " << String(50, '-') << "\n";
}*/

/*void				Exporter::writePhaseBlock(std::ofstream& out, cpPhase phase)
{
	if (!phase)
		return;

	cvpMatch matches = phase->getMatches();
	cInt nbSets = phase->getNbSetToPlay();

	out << "============================================================\n";
	out << "\t" << phase->getName() << "  (" << nbSets << " set(s) par rencontre)\n";
	out << "============================================================\n";

	if (matches.empty())
	{
		PrintUtils::addError("Aucun match enregistre.");
		return;
	}

	int encounterNum = 1;

	for (size_t i = 0; i < matches.size(); i += static_cast<size_t>(nbSets))
		writeEncounterBlock(out, matches, i, nbSets, encounterNum++);

	if (phase->isFinished())
		writePhaseResults(out, *phase);

	out << "\n";
}*/

/*void				Exporter::writePhaseResults(std::ofstream& out, cPhase phase)
{
	cvpTeam winners = phase.getWinners();
	cvpTeam losers = phase.getLosers();

	out << "\n\tQualifies :\n";

	for (size_t i = 0; i < winners.size(); ++i)
		if (winners[i])
			out << "\t\t" << (i + 1) << ". " << winners[i]->getName() << "\n";

	if (!losers.empty())
	{
		out << "\tElimines :\n";

		for (cpTeam t : losers)
			if (t)
				out << "\t\t- " << t->getName() << "\n";
	}
}*/

/*void				Exporter::writePalmares(std::ofstream& out, cTour tournament)
{
	cpPhase final = tournament.getFinal();
	cpPhase thirdPlace = tournament.getThirdPlace();

	out << "============================================================\n";
	out << "\tPALMARES\n";
	out << "============================================================\n";

	if (!final || !final->isFinished())
	{
		PrintUtils::addError("Finale non terminee — palmares indisponible.");
		return;
	}

	cvpTeam winners = final->getWinners();
	cvpTeam losers  = final->getLosers();

	if (!winners.empty() && winners[0])
		out << "\t1. (Or)\t\t" << winners[0]->getName() << "\n";

	if (!losers.empty()  && losers[0])
		out << "\t2. (Argent)\t" << losers[0]->getName()  << "\n";

	if (thirdPlace && thirdPlace->isFinished())
	{
		cvpTeam third = thirdPlace->getWinners();
		cvpTeam fourth = thirdPlace->getLosers();

		if (!third.empty()  && third[0])
			out << "\t3. (Bronze)\t" << third[0]->getName()  << "\n";

		if (!fourth.empty() && fourth[0])
			out << "\t4.\t\t\t" << fourth[0]->getName() << "\n";
	}

	out << "\n";
}*/

/**
 * Ecrit le tableau de classement de la poule dans le flux out.
 * Colonnes : Rang | Equipe | Pts | Diff
 * Si toFile = true : pas de codes couleur ANSI.
 */
/*void				Exporter::writeTable(std::ostream& out, cPool pool, cBool toFile)
{
	cvpTeam teams = pool.getTeams();

	if (teams.empty())
		return (PrintUtils::addError("Aucune equipe dans cette poule."));

	size_t maxLen = 6;

	for (cpTeam t : teams)
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
}*/

/**************************************************************************************************/
/*	PUBLIC METHOD																				  */
/**************************************************************************************************/

/****************/
/*	EXPORTS TXT	*/
/****************/

/*bool				Exporter::exportTournamentToTxt(cTour tournament, cString filename)
{
	std::ofstream out(filename);

	if (!out.is_open())
		return (false);

	writeHeader(out, tournament);
	writePools(out, tournament);
	writePhaseBlock(out, tournament.getSixteenth());
	writePhaseBlock(out, tournament.getEighth());
	writePhaseBlock(out, tournament.getQuarters());
	writePhaseBlock(out, tournament.getSemis());
	writePhaseBlock(out, tournament.getThirdPlace());
	writePhaseBlock(out, tournament.getFinal());
	writePalmares(out, tournament);

	out.close();

	return (true);
}*/

/*bool				Exporter::exportPhaseToTxt(pPhase phase, cString filename)
{
	if (!phase)
	{
		PrintUtils::addError("Phase inexistante ou non generee — export annule.");
		return (false);
	}

	return (PhaseCLI::exportToTxt(*phase, filename));
}*/

/*bool				Exporter::exportPoolsToTxt(cTour tournament, cString filename)
{
	std::ofstream out(filename);

	if (!out.is_open())
	{
		PrintUtils::addError(std::format("Impossible de créer le fichier : {}", filename));
		return (false);
	}

	writePools(out, tournament);
	out.close();

	return (true);
}*/

/**
 * Exporte l historique complet de la poule (matchs + classement) dans un .txt.
 */
/*bool				Exporter::exportToTxt(cPool pool, cString filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
	{
		PrintUtils::addError(std::format("Impossible de créer : {}", filename));
		return (false);
	}

	file << "============================================================\n";
	file << "  POULE : " << pool.getName() << "\n";
	file << "============================================================\n";

	file << "\n[EQUIPES]\n";

	for (cpTeam t : pool.getTeams())
	{
		file << "  - " << t->getName();

		if (t->getHasMultiTeamPlayer())
			file << " [Multi-joueur]";

		file << " : ";

		cvpPlayer members = t->getMembers();

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
	PrintUtils::writeMatchesList(file, pool.getMatches(), true);

	file << "\n[CLASSEMENT FINAL]\n";
	writeTable(file, pool, true);

	file << "\n============================================================\n";
	file.close();

	return (true);
}*/

/****************/
/*	EXPORTS CSV	*/
/****************/

/*bool				Exporter::exportPlayersToCSV(cvpPlayer participants, cString filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
		return (false);

	file << "pseudo,nom,prenom,genre\n";

	for (cpPlayer p : participants)
		if (p)
			file << p->getPseudo() << ","
				<< p->getLastName() << ","
				<< p->getFirstName() << ","
				<< (p->getGenderInt() == Gender::MALE ? "0" : "1") << "\n";

	file.close();

	return (true);
}
*/
/********************/
/*	EXPORTS JSON	*/
/********************/

/**
 *	Export les Players au format csv
 */
bool				Exporter::exportPlayersToCSV(cvpPlayer players, cString filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
		return (false);

	file << "Pseudo,Nom,Prenom,Genre\n";

	for (const Player* p : players)
	{
		if (!p)
			continue;

		file << p->getPseudo() << ","
			 << p->getLastName() << ","
			 << p->getFirstName() << ","
			 << (p->getGenderInt() == Gender::MALE ? "0" : "1") << "\n";
	}

	file.close();
	return (true);
}

bool				Exporter::exportPlayersToTxt(cvpPlayer players, cString filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
		return (false);

	file << "============================================================\n";
	file << "                  LISTE DES PARTICIPANTS                    \n";
	file << "============================================================\n\n";

	TablePrinter table;
	table.setHeaders({"ID", "Pseudo", "Nom", "Prenom", "Genre"});

	for (const Player* p : players)
	{
		if (!p)
			continue;

		table.addRow({
			std::to_string(p->getId()),
			p->getPseudo(),
			p->getLastName(),
			p->getFirstName(),
			p->getGenderInt() == Gender::MALE ? "0" : "1"
		});
	}

	table.printTable(file, true);
	file.close();
	return (true);
}

/****************************************************************************************************/
/*	TEAMS EXPORT																					*/
/****************************************************************************************************/

bool				Exporter::exportTeamsToTxt(const std::vector<Team*>& teams, cString filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
		return (false);

	file << "============================================================\n";
	file << "                     LISTE DES EQUIPES                      \n";
	file << "============================================================\n\n";

	TablePrinter table;
	table.setHeaders({"ID", "Equipe", "Membres", "Pts", "Marques", "Encaisses", "Diff"});

	for (const Team* t : teams)
	{
		if (!t)
			continue;

		String membersStr;
		const auto& members = t->getMembers();

		for (size_t i = 0; i < members.size(); ++i)
		{
			if (members[i])
			{
				if (i > 0)
					membersStr += " & ";
				membersStr += members[i]->getPseudo();
			}
		}

		table.addRow({
			std::to_string(t->getId()),
			t->getName(),
			membersStr,
			std::to_string(t->getPoint()),
			std::to_string(t->getScoreMarked()),
			std::to_string(t->getScoreAgainst()),
			std::to_string(t->getScoreDiff())
		});
	}

	table.printTable(file, true);
	file.close();
	return (true);
}

bool				Exporter::exportTeamsToTxt(const Tournament& tournament, cString filename)
{
	return (exportTeamsToTxt(tournament.getTeams(), filename));
}

/****************************************************************************************************/
/*	MATCHES EXPORT																					*/
/****************************************************************************************************/

bool				Exporter::exportMatchesToTxt(const std::vector<Match*>& matches, cString filename, cString title)
{
	std::ofstream file(filename);

	if (!file.is_open())
		return (false);

	file << "============================================================\n";
	file << "  " << title << "\n";
	file << "============================================================\n\n";

	TablePrinter table;
	table.setHeaders({"Equipe A", "Score A", "Score B", "Equipe B", "Statut", "Vainqueur"});

	for (const Match* m : matches)
	{
		if (!m)
			continue;

		String teamA = m->getTeamA() ? m->getTeamA()->getName() : "Inconnu";
		String teamB = m->getTeamB() ? m->getTeamB()->getName() : "Inconnu";
		String status = m->isFinished() ? "Termine" : "En cours";
		String winner = "N/A";

		if (m->isFinished())
			winner = m->getWinner()->getName();

		table.addRow({
			teamA,
			std::to_string(m->getScoreA()),
			std::to_string(m->getScoreB()),
			teamB,
			status,
			winner
		});
	}

	table.printTable(file, true);
	file.close();
	return (true);
}

/****************************************************************************************************/
/*	PHASE EXPORT																					*/
/****************************************************************************************************/

bool				Exporter::exportPhaseToTxt(const Phase* phase, cString filename)
{
	if (!phase)
		return (false);

	std::ofstream file(filename);

	if (!file.is_open())
		return (false);

	file << "============================================================\n";
	file << "  PHASE : " << phase->getName() << "\n";
	file << "  Sets a jouer par rencontre : " << phase->getNbSetToPlay() << "\n";
	file << "  Statut : " << (phase->isFinished() ? "Terminee" : "En cours") << "\n";
	file << "============================================================\n\n";

	file << "--- MATCHS DE LA PHASE ---\n\n";
	TablePrinter table;
	table.setHeaders({"Equipe A", "Score A", "Score B", "Equipe B", "Statut"});

	for (const Match* m : phase->getMatches())
	{
		if (!m)
			continue;

		table.addRow({
			m->getTeamA()->getName(),
			std::to_string(m->getScoreA()),
			std::to_string(m->getScoreB()),
			m->getTeamB()->getName(),
			m->isFinished() ? "Termine" : "En cours"
		});
	}

	table.printTable(file, true);

	if (phase->isFinished())
	{
		file << "\n--- RESULTATS ---\n\n";
		file << "Qualifies / Vainqueurs :\n";

		for (const Team* w : phase->getWinners())
			if (w)
				file << "  - " << w->getName() << "\n";

		file << "\nElimines :\n";

		for (const Team* l : phase->getLosers())
			if (l)
				file << "  - " << l->getName() << "\n";
	}

	file.close();
	return (true);
}

/****************************************************************************************************/
/*	POOL EXPORT																						*/
/****************************************************************************************************/

bool				Exporter::exportPoolToTxt(const Pool* pool, cString filename)
{
	if (!pool)
		return (false);

	std::ofstream file(filename);

	if (!file.is_open())
		return (false);

	file << "============================================================\n";
	file << "  POULE : " << pool->getName() << "\n";
	file << "============================================================\n\n";

	file << "--- CLASSEMENT DES EQUIPES ---\n\n";
	TablePrinter table;
	table.setHeaders({"Rang", "Nom Equipe", "Pts", "Marques", "Encaisses", "Diff"});

	size_t rank = 1;
	for (const Team* t : pool->getTeams())
	{
		if (!t)
			continue;

		table.addRow({
			std::to_string(rank++),
			t->getName(),
			std::to_string(t->getPoint()),
			std::to_string(t->getScoreMarked()),
			std::to_string(t->getScoreAgainst()),
			std::to_string(t->getScoreDiff())
		});
	}

	table.printTable(file, true);

	file << "\n--- MATCHS DE LA POULE ---\n\n";
	TablePrinter matchPrinter;
	matchPrinter.setHeaders({"Equipe A", "Score A", "Score B", "Equipe B", "Statut"});

	for (const Match* m : pool->getMatches())
	{
		if (!m)
			continue;

		matchPrinter.addRow({
			m->getTeamA()->getName(),
			std::to_string(m->getScoreA()),
			std::to_string(m->getScoreB()),
			 m->getTeamB()->getName(),
			m->isFinished() ? "Termine" : "En cours"
		});
	}

	matchPrinter.printTable(file, true);

	if (pool->allMatchesFinished())
	{
		file << "\n--- QUALIFIER(S) ---\n";
		for (const Team* q : pool->getQualifiers())
			if (q)
				file << "  - " << q->getName() << "\n";
	}

	file.close();
	return (true);
}

bool				Exporter::exportPoolsToTxt(const std::vector<Pool*>& pools, cString filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
		return (false);

	file << "============================================================\n";
	file << "                  ENSEMBLE DES POULES                       \n";
	file << "============================================================\n\n";

	for (const Pool* pool : pools)
	{
		if (!pool)
			continue;

		file << ">>> " << pool->getName() << " <<<\n\n";

		TablePrinter teamPrinter;
		teamPrinter.setHeaders({"Rang", "Nom Equipe", "Pts", "Marques", "Encaisses", "Diff"});

		size_t rank = 1;
		for (const Team* t : pool->getTeams())
		{
			if (!t)
				continue;

			teamPrinter.addRow({
				std::to_string(rank++),
				t->getName(),
				std::to_string(t->getPoint()),
				std::to_string(t->getScoreMarked()),
				std::to_string(t->getScoreAgainst()),
				std::to_string(t->getScoreDiff())
			});
		}

		teamPrinter.printTable(file, true);
		file << "\n\n";
	}

	file.close();
	return (true);
}

bool				Exporter::exportPoolsToTxt(const Tournament& tournament, cString filename)
{
	return (exportPoolsToTxt(tournament.getPools(), filename));
}

/****************************************************************************************************/
/*	TOURNAMENT EXPORT																				*/
/****************************************************************************************************/

bool				Exporter::exportTournamentToTxt(const Tournament& tournament, cString filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
		return (false);

	const Settings s = tournament.getSettings();

	file << "============================================================\n";
	file << "  RECAPITULATIF DU TOURNOI : " << s.getName() << "\n";
	file << "============================================================\n\n";

	file << "--- PARAMETRES DU TOURNOI ---\n";
	file << "Nombre de participants : " << s.getNbPlayers() << "\n";
	file << "Nombre de poules       : " << s.getNbPools() << "\n";
	file << "Terrains disponibles   : " << s.getNbBadmintonCourt() << "\n";
	file << "Format                 : " << (s.getIsDouble() ? "Double" : "Simple")
		 << " | " << (s.getIsMixed() ? "Mixte" : "Non-mixte") << "\n\n";

	file << "--- PARTICIPANTS ---\n\n";
	TablePrinter playerPrinter;
	playerPrinter.setHeaders({"ID", "Pseudo", "Nom", "Prenom", "Genre"});

	for (const Player* p : tournament.getPlayers())
	{
		if (!p)
			continue;

		playerPrinter.addRow({
			std::to_string(p->getId()),
			p->getPseudo(),
			p->getLastName(),
			p->getFirstName(),
			p->getGenderStr()
		});
	}

	playerPrinter.printTable(file, true);
	file << "\n\n";

	file << "--- CLASSEMENT DES POULES ---\n\n";

	for (const Pool* pool : tournament.getPools())
	{
		if (!pool)
			continue;

		file << "[" << pool->getName() << "]\n";

		TablePrinter poolPrinter;
		poolPrinter.setHeaders({"Nom Equipe", "Pts", "Diff"});

		for (const Team* t : pool->getTeams())
		{
			if (!t)
				continue;

			poolPrinter.addRow({
				t->getName(),
				std::to_string(t->getPoint()),
				std::to_string(t->getScoreDiff())
			});
		}

		poolPrinter.printTable(file, true);
		file << "\n";
	}

	auto exportPhaseSection = [&file](const Phase* phase, cString title)
	{
		if (!phase)
			return;

		file << "--- PHASE : " << title << " ---\n\n";

		TablePrinter phasePrinter;
		phasePrinter.setHeaders({"Equipe A", "Score A", "Score B", "Equipe B"});

		for (const Match* m : phase->getMatches())
		{
			if (!m)
				continue;

			phasePrinter.addRow({
				m->getTeamA()->getName(),
				std::to_string(m->getScoreA()),
				std::to_string(m->getScoreB()),
				m->getTeamB()->getName()
			});
		}

		phasePrinter.printTable(file, true);
		file << "\n\n";
	};

	exportPhaseSection(tournament.getSixteenth(), "1/16 DE FINALE");
	exportPhaseSection(tournament.getEighth(), "1/8 DE FINALE");
	exportPhaseSection(tournament.getQuarters(), "QUARTS DE FINALE");
	exportPhaseSection(tournament.getSemis(), "DEMI-FINALES");
	exportPhaseSection(tournament.getThirdPlace(), "PETITE FINALE");
	exportPhaseSection(tournament.getFinal(), "FINALE");

	file.close();
	return (true);
}
