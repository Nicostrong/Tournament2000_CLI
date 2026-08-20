//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <algorithm>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Team.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;
using				vtupleMsg		=	std::vector<std::tuple<std::string, bool>>;

using				cInt			=	const int;
using				vInt			=	std::vector<int>;
using				cvInt			=	const std::vector<int>;
template<std::size_t N>
using				aInt			=	std::array<int, N>;

using				cBool			=	const bool;

using				pPool			=	Pool*;
using				cPool			=	const Pool&;
using				cpPool			=	const Pool*;
using				vpPool			=	std::vector<Pool*>;
using				cvpPool			=	const std::vector<Pool*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

int					Pool::_idCounter = 1;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Pool::Pool(): _name("Pool " + std::to_string(_idCounter++))
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

void				Pool::generateMatches(cInt nbSetsPerEncounter)
{
	for (cpMatch m : this->_matches)
		delete m;
	
	this->_matches.clear();

	if (this->_teams.size() < 2)
		return;

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
	for (cpMatch m: this->_matches)
		if (!m->isFinished())
			return (false);

	return (true);
}

vpTeam				Pool::getQualifiers() const
{
	vpTeam qualifiers;

	if (!this->_teams.empty())
		qualifiers.push_back(this->_teams[0]);

	if (this->_teams.size() >= 2)
		qualifiers.push_back(this->_teams[1]);

	return (qualifiers);
}