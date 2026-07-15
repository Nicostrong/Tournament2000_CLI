//
// Created by Nicolas Fordoxcel on 24/06/2026.
//

//	STDLIB

//	INCLUDES
#include "../includes/Phase.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef std::vector<Match*>			V_MATCH;
typedef std::vector<Team*>			V_TEAM;

//	STATIC VARIABLES

/****************/
/*	CANONICAL	*/
/****************/

Phase::Phase(C_STRING name, int nbSets): _name(name), _nbSetsToPlay(nbSets), _isFinished(false)
{
	this->_matches.clear();
}

Phase::~Phase()
{
	if (!this->_matches.empty())
		for (Match* m : this->_matches)
			delete m;
	
	this->_matches.clear();
}


/************/
/*	GETTER	*/
/************/

C_STRING							Phase::getName() const
{
	return (this->_name);
}

const V_MATCH&						Phase::getMatches() const
{
	return (this->_matches);
}


V_TEAM								Phase::getWinners() const
{
	V_TEAM winners;
	
	for (size_t i = 0; i < this->_matches.size(); i += this->_nbSetsToPlay)
	{
		int winsA = 0;
		int winsB = 0;
		
		Team* a = this->_matches[i]->getTeamA();
		Team* b = this->_matches[i]->getTeamB();

		for (int j = 0; j < this->_nbSetsToPlay; ++j)
		{
			if (this->_matches[i + j]->getWinner() == a)
				winsA++;
			else if (this->_matches[i + j]->getWinner() == b)
				winsB++;
		}
		
		if (winsA > winsB)
			winners.push_back(a);
		else
			winners.push_back(b);
	}

	return (winners);
}

/************/
/*	SETTER	*/
/************/

/********************/
/*	PRIVATE METHODS	*/
/********************/

/********************/
/*	PUBLIC METHODS	*/
/********************/

void								Phase::addEncounter(Team* a, Team* b)
{
	if (!a || !b)
		return ;

	for (int i = 0; i < this->_nbSetsToPlay; ++i)
		this->_matches.push_back(new Match(a, b));
}

bool								Phase::isFinished() const
{
	if (this->_matches.empty())
		return (false);

	for (auto m : this->_matches)
		if (!m->isFinished())
			return (false);

	return (true);
}