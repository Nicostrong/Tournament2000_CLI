//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

//	STDLIB
#include <iostream>
#include <fstream>

//	INCLUDES
#include "../includes/TournamentHistory.hpp"

//	TYPEDEF
typedef const std::string&			C_STRING;

//	STATIC VARIABLES

/****************/
/*	CANONICAL	*/
/****************/

TournamentHistory::TournamentHistory() {}

TournamentHistory::~TournamentHistory() {}

/************/
/*	GETTER	*/
/************/

/************/
/*	SETTER	*/
/************/

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

void								TournamentHistory::logEvent(C_STRING event)
{
	this->_globalEvents.push_back(event);
}

void								TournamentHistory::recordMatch(const Match* match)
{
	if (!match)
		return ;

	for (const Participant* p : match->getTeamA()->getMembers())
		this->_participantMatches[p].push_back(match);

	for (const Participant* p : match->getTeamB()->getMembers())
		this->_participantMatches[p].push_back(match);
}

void								TournamentHistory::exportParticipantSummary(const Participant* p, C_STRING filename) const
{
	std::ofstream file(filename);

	if (!file.is_open())
	{
		std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << std::endl;
		return ;
	}

	file << "=== RESUME DU TOURNOI POUR " << p->getPseudo() << " ===\n\n";

	auto it = this->_participantMatches.find(p);

	if (it != this->_participantMatches.end() && !it->second.empty())
	{
		for (const Match* m : it->second)
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