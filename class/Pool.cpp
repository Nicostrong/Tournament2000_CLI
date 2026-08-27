//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <random>
#include <algorithm>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Settings.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cString			=	const std::string&;

using				cInt			=	const int;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

int					Pool::_idCounter = 1;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Pool::Pool(): _name("Pool " + std::to_string(_idCounter++)), _isFinished(false)
{}

Pool::~Pool()
{
	for (cpMatch m : this->_matches)
		delete m;
	
	this->_matches.clear();
}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

cString				Pool::getName() const		{	return (this->_name);		}
cvpTeam				Pool::getTeams() const		{	return (this->_teams);		}
cvpMatch			Pool::getMatches() const	{	return (this->_matches);	}
cBool				Pool::getIsFinished() const	{	return (this->_isFinished);	}


/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHODS																					*/
/****************************************************************************************************/

void				Pool::addTeam(pTeam team)
{
	if (team)
		this->_teams.push_back(team);
}

void				Pool::generateMatches(cInt nbSetsPerEncounter, pSet settings)
{
	for (cpMatch m : this->_matches)
		delete m;
	
	this->_matches.clear();

	if (this->_teams.size() < 2)
		return;

	ScoreRules rules{
		settings->getScoreMin(),
		settings->getScoreMax(),
		settings->getDiffPointsToWin()
	};

	for (size_t i = 0; i < this->_teams.size(); ++i)
		for (size_t j = i + 1; j < this->_teams.size(); ++j)
			for (int s = 0; s < nbSetsPerEncounter; ++s)
				this->_matches.push_back(new Match(this->_teams[i], this->_teams[j], rules));
}

void				Pool::sortTeams()
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::ranges::shuffle(this->_teams.begin(), this->_teams.end(), g);

	std::ranges::stable_sort(this->_teams.begin(), this->_teams.end(), [](const Team* a, const Team* b)
	{
		if (a->getPoint() != b->getPoint())
			return (a->getPoint() > b->getPoint());

		if (a->getScoreDiff() != b->getScoreDiff())
			return (a->getScoreDiff() > b->getScoreDiff());

		if (a->getIsMixed() != b->getIsMixed())
			return (a->getIsMixed());

		return (false); 
	});
}

void				Pool::checkPoolIsFinished()
{
	if (allMatchesFinished())
		this->_isFinished = true;
}

bool				Pool::allMatchesFinished() const
{
	for (cpMatch m: this->_matches)
		if (!m->isFinished())
			return (false);

	return (true);
}

bool				Pool::containsTeam(cpTeam team) const
{
	return (std::find(this->_teams.begin(), this->_teams.end(), team) != this->_teams.end());
}

vpTeam				Pool::getQualifiers() const
{
	vpTeam qualifiers;
	size_t nbQualifiers = std::min(this->_teams.size(), static_cast<size_t>(2));
	
	for (size_t i = 0; i < nbQualifiers; ++i)
		qualifiers.push_back(this->_teams[i]);

	return (qualifiers);
}