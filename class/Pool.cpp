//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <random>
#include <memory>
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

using				cBool			=	const bool;

using				cSet			=	const Settings&;

using				vpMatch			=	std::vector<Match*>;
using				uMatch			=	std::unique_ptr<Match>;

using				pTeam			=	Team*;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

int					Pool::_idCounter = 1;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Pool::Pool()
	: _name("Pool " + std::to_string(_idCounter++)), _isFinished(false) {}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

cString				Pool::getName() const			{	return (this->_name);		}
cvpTeam				Pool::getTeams() const			{	return (this->_teams);		}
vpTeam&				Pool::getTeamsMutable()			{	return (this->_teams);		}
cBool				Pool::getIsFinished() const		{	return (this->_isFinished);	}

vpMatch				Pool::getMatches() const
{
	vpMatch matches;

	matches.reserve(this->_matches.size());

	for (const uMatch& match: this->_matches)
		if (match)
			matches.push_back(match.get());
	
	return (matches);
}

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

void				Pool::generateMatches(cInt nbSetsPerEncounter, cSet settings)
{
	this->_matches.clear();
	this->_isFinished = false;

	if (this->_teams.size() < 2)
		return;

	if (nbSetsPerEncounter <= 0)
		return;

	ScoreRules rules{
		settings.getScoreMin(),
		settings.getScoreMax(),
		settings.getDiffPointsToWin()
	};

	for (size_t i = 0; i < this->_teams.size(); ++i)
		for (size_t j = i + 1; j < this->_teams.size(); ++j)
		{
			if (!this->_teams[i] || ! this->_teams[j])
				continue;

			for (int s = 0; s < nbSetsPerEncounter; ++s)
				this->_matches.push_back(std::make_unique<Match>(this->_teams[i], this->_teams[j], rules));
		}
}

void				Pool::sortTeams()
{
	std::random_device rd;
	std::mt19937 g(rd());
	std::ranges::shuffle(this->_teams.begin(), this->_teams.end(), g);

	std::ranges::stable_sort(this->_teams.begin(), this->_teams.end(), [](const Team* a, const Team* b)
	{
		if (!a || !b)
			return (a != nullptr);

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
	this->_isFinished = this->allMatchesFinished();
}

bool				Pool::allMatchesFinished() const
{
	if (this->_matches.empty())
		return(false);

	for (const uMatch& m: this->_matches)
		if (!m || !m->isFinished())
			return (false);

	return (true);
}

bool				Pool::containsTeam(cpTeam team) const
{
	if (!team)
		return (false);

	return (std::ranges::find(this->_teams.begin(), this->_teams.end(), team) != this->_teams.end());
}

vpTeam				Pool::getQualifiers() const
{
	vpTeam qualifiers;
	const size_t nbQualifiers = std::min(this->_teams.size(), static_cast<size_t>(2));
	
	for (size_t i = 0; i < nbQualifiers; ++i)
		qualifiers.push_back(this->_teams[i]);

	return (qualifiers);
}