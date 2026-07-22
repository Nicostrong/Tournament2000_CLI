//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB
#include <algorithm>

//	INCLUDES
#include "../includes/class/Pool.hpp"

//	TYPEDEF
using				C_STRING	=	const std::string&;
using				VP_TEAM		=	std::vector<Team*>;
using				CVP_MATCH	=	const std::vector<Match*>&;
using               CVP_TEAM	=	const std::vector<Team*>&;

//	STATIC VARIABLES
int					Pool::_idCounter = 1;

/****************/
/*	CANONICAL	*/
/****************/

Pool::Pool() : _name("Pool " + std::to_string(_idCounter++))
{}

Pool::~Pool()
{
	for (const Match* m : this->_matches)
		delete m;
	
	this->_matches.clear();
}

/************/
/*	GETTER	*/
/************/

C_STRING			Pool::getName() const
{
	return (this->_name);
}

CVP_TEAM			Pool::getTeams() const
{
	return (this->_teams);
}

CVP_MATCH			Pool::getMatches() const
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

void				Pool::addTeam(Team* team)
{
	if (team)
		this->_teams.push_back(team);
}

void				Pool::generateMatches(const int nbSetsPerEncounter)
{
	for (const Match* m : this->_matches)
		delete m;
	
	this->_matches.clear();

	if (this->_teams.size() < 2)
		return ;

	for (size_t i = 0; i < this->_teams.size(); ++i)
		for (size_t j = i + 1; j < this->_teams.size(); ++j)
			for (int s = 0; s < nbSetsPerEncounter; ++s)
				this->_matches.push_back(new Match(this->_teams[i], this->_teams[j]));
}

void				Pool::sortTeams()
{
	std::sort(this->_teams.begin(), this->_teams.end(), [](const Team* a, const Team* b)
	{
		if (a->getPoint() != b->getPoint())
			return (a->getPoint() > b->getPoint());

		return (a->getScoreDiff() > b->getScoreDiff());
	});
}

bool				Pool::allMatchesFinished() const
{
	for (const Match* m: this->_matches)
		if (!m->isFinished())
			return (false);

	return (true);
}

VP_TEAM				Pool::getQualifiers() const
{
	VP_TEAM			qualifiers;

	if (!this->_teams.empty())
		qualifiers.push_back(this->_teams[0]);

	if (this->_teams.size() >= 2)
		qualifiers.push_back(this->_teams[1]);

	return (qualifiers);
}