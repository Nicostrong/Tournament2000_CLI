//
// Created by Nicolas Fordoxcel on 09/07/2026.
//


/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <fstream>

#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Participant.hpp"

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/TournamentHistory.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

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

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				TournamentHistory::logEvent(cString event)
{
	this->_globalEvents.push_back(event);
}

void				TournamentHistory::recordMatch(pMatch match)
{
	if (!match)
		return;

	for (cpPart p : match->getTeamA()->getMembers())
		this->_participantMatches[p].push_back(match);

	for (cpPart p : match->getTeamB()->getMembers())
		this->_participantMatches[p].push_back(match);
}

void				TournamentHistory::exportParticipantSummary(cpPart p, cString filename) const
{
	std::ofstream file(filename);

	if (!file.is_open())
	{
		PrintUtils::addError(std::format("Erreur : Impossible d'ouvrir le fichier {}.", filename));
		return;
	}

	file << "=== RESUME DU TOURNOI POUR " << p->getPseudo() << " ===\n\n";

	auto it = this->_participantMatches.find(p);

	if (it != this->_participantMatches.end() && !it->second.empty())
	{
		for (cpMatch m : it->second)
		{
			file << m->getTeamA()->getName() << " [" << m->getScoreA() << "] - ["
				<< m->getScoreB() << "] " << m->getTeamB()->getName();
				
			if (m->getWinner())
				file << " (Vainqueur : " << m->getWinner()->getName() << ")";
			file << "\n";
		}
	}
	else
		file << "Aucun match enregistre pour ce participant.\n";
	
	file.close();
}