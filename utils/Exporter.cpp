//
// Created by Nicolas Fordoxcel on 18/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <fstream>
#include <iomanip>

#include "../includes/class/Team.hpp"
#include "../includes/class/Pool.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"
#include "../includes/class/Participant.hpp"

#include "../includes/cli/PhaseCLI.hpp"

#include "../includes/utils/Exporter.hpp"
#include "../includes/utils/PrintUtils.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cpSet			=	const Settings*;

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

void				Exporter::writeHeader(std::ofstream& out, cTour tournament)
{
	cpSet s = tournament.getSettings();

	out << "############################################################\n";
	out << "##\t\tTOURNOI :\t" << s->getName() << "\n";
	out << "############################################################\n\n";

	out << "\tType\t\t\t:\t";

	if (s->getIsDouble() && s->getIsMixed())
		out << "Double mixte\n";
	else if (s->getIsDouble())
		out << "Double\n";
	else
		out << "Simple\n";

	out << "\tJoueurs\t\t\t:\t" << s->getNbPlayers() << "\n";
	out << "\tPoules\t\t\t:\t" << s->getNbPools() << " x " << s->getNbPlayerByPool() << " equipes\n";
	out << "\tScore min/max\t:\t" << s->getScoreMin() << " / " << s->getScoreMax()
		<< " (ecart " << s->getDiffPointsToWin() << ")\n";
	out << "\tMulti-team\t\t:\t" << (s->getAllowMultiTeamPlayers() ? "Oui" : "Non") << "\n";
	out << "\tPetite finale\t:\t" << (s->getIsThirdPlaceMatch()    ? "Oui" : "Non") << "\n\n";
}

void				Exporter::writePools(std::ofstream& out, cTour tournament)
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
}

void				Exporter::writePoolMatches(std::ofstream& out, cPool pool)
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
}

void				Exporter::writePoolStandings(std::ofstream& out, cPool pool)
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
}

void				Exporter::writeEncounterBlock(std::ofstream& out, cvpMatch matches,
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
}

void				Exporter::writePhaseBlock(std::ofstream& out, cpPhase phase)
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
}

void				Exporter::writePhaseResults(std::ofstream& out, cPhase phase)
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
}

void				Exporter::writePalmares(std::ofstream& out, cTour tournament)
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
}

/**
 * Ecrit la liste des matchs de la poule dans le flux out.
 * Si toFile = true : pas de codes couleur ANSI.
 */
void				Exporter::writeMatches(std::ostream& out, cPool pool, cBool toFile)
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
void				Exporter::writeTable(std::ostream& out, cPool pool, cBool toFile)
{
	cvpTeam teams = pool.getTeams();

	if (teams.empty())
	{
		PrintUtils::addError("Aucune equipe dans cette poule.");
		return;
	}

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
}

/**************************************************************************************************/
/*	PUBLIC METHOD																				  */
/**************************************************************************************************/

/****************/
/*	EXPORTS TXT	*/
/****************/

bool				Exporter::exportTournamentToTxt(cTour tournament, cString filename)
{
	std::ofstream out(filename);

	if (!out.is_open())
		return (false);

	writeHeader(out, tournament);
	writePools(out, tournament);
	writePhaseBlock(out, tournament.getSixteenth());
	writePhaseBlock(out, tournament.getHeighth());
	writePhaseBlock(out, tournament.getQuarters());
	writePhaseBlock(out, tournament.getSemis());
	writePhaseBlock(out, tournament.getThirdPlace());
	writePhaseBlock(out, tournament.getFinal());
	writePalmares(out, tournament);

	out.close();

	return (true);
}

bool				Exporter::exportPhaseToTxt(cpPhase phase, cString filename)
{
	if (!phase)
	{
		PrintUtils::addError("Phase inexistante ou non generee — export annule.");
		return (false);
	}

	return (PhaseCLI::exportToTxt(*phase, filename));
}

bool				Exporter::exportPoolsToTxt(cTour tournament, cString filename)
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
}

/**
 * Exporte l historique complet de la poule (matchs + classement) dans un .txt.
 */
bool				Exporter::exportToTxt(cPool pool, cString filename)
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

		cvpPart members = t->getMembers();

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

/****************/
/*	EXPORTS CSV	*/
/****************/

bool				Exporter::exportParticipantsToCSV(cvpPart participants, cString filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
		return (false);

	file << "pseudo,nom,prenom,genre\n";

	for (cpPart p : participants)
		if (p)
			file << p->getPseudo() << ","
				<< p->getLastName() << ","
				<< p->getFirstName() << ","
				<< (p->getGenderInt() == Gender::MALE ? "0" : "1") << "\n";

	file.close();

	return (true);
}

/********************/
/*	EXPORTS JSON	*/
/********************/
