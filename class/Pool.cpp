//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB

//	INCLUDES
#include "../includes/class/Pool.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef std::vector<Match*>			VP_MATCH;
typedef std::vector<Team*>			VP_TEAM;

//	STATIC VARIABLES
int									Pool::_idCounter = 0;

/****************/
/*	CANONICAL	*/
/****************/

Pool::Pool() : _name("Pool_" + std::to_string(_idCounter++))
{}

Pool::~Pool()
{
	for (Match* m : this->_matches)
		delete m;
	
	this->_matches.clear();
}

/************/
/*	GETTER	*/
/************/

C_STRING							Pool::getName() const
{
	return (this->_name);
}

const VP_TEAM&						Pool::getTeams() const
{
	return (this->_teams);
}

const VP_MATCH&						Pool::getMatches() const
{
	return (this->_matches);
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

void								Pool::addTeam(Team* team)
{
	if (team)
		this->_teams.push_back(team);
}

void								Pool::generateMatches(int nbSetsPerEncounter)
{
	for (Match* m : this->_matches)
		delete m;
	
	this->_matches.clear();

	if (this->_teams.size() < 2)
		return ;

	for (size_t i = 0; i < this->_teams.size(); ++i)
		for (size_t j = i + 1; j < this->_teams.size(); ++j)
			for (int s = 0; s < nbSetsPerEncounter; ++s)
				this->_matches.push_back(new Match(this->_teams[i], this->_teams[j]));
}

void								Pool::sortTeams()
{
	std::sort(this->_teams.begin(), this->_teams.end(), [](Team* a, Team* b)
	{
		if (a->getPoint() != b->getPoint())
			return (a->getPoint() > b->getPoint());

		return (a->getScoreDiff() > b->getScoreDiff());
	});
}

VP_TEAM								Pool::getQualifiers() const
{
	VP_TEAM			qualifiers;

	if (this->_teams.size() >= 1)
		qualifiers.push_back(this->_teams[0]);
	if (this->_teams.size() >= 2)
		qualifiers.push_back(this->_teams[1]);

	return (qualifiers);
}
