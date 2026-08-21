//
// Created by Nicolas Fordoxcel on 24/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <utility>

#include "../includes/class/Phase.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Team.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Phase::Phase(String name, cInt nbSets): _name(std::move(name)), _nbSetsToPlay(nbSets), _isFinished(false)
{
	this->_matches.clear();
}

Phase::~Phase()
{
	if (!this->_matches.empty())
		for (cpMatch m : this->_matches)
			delete m;
	
	this->_matches.clear();
}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

cString				Phase::getName() const			{	return (this->_name);			}
int					Phase::getNbSetToPlay() const	{	return (this->_nbSetsToPlay);	}
bool				Phase::getIsFinished() const	{	return (this->_isFinished);		}
cvpMatch			Phase::getMatches() const		{	return (this->_matches);		}

vpTeam				Phase::getWinners() const
{
	vpTeam winners;	

	for (size_t i = 0; i < this->_matches.size(); i += this->_nbSetsToPlay)
	{
		int winsA = 0;
		int winsB = 0;
		pTeam a = this->_matches[i]->getTeamA();
		pTeam b = this->_matches[i]->getTeamB();

		for (int j = 0; j < this->_nbSetsToPlay; ++j)
		{
			if (this->_matches[i + j]->getWinner() == a)
				winsA++;
			else if (this->_matches[i + j]->getWinner() == b)
				winsB++;
		}
		
		winners.push_back(winsA >= winsB ? a : b);
	}

	return (winners);
}

vpTeam				Phase::getLosers() const
{
	vpTeam losers;
	vpTeam winners = getWinners();

	for (size_t i = 0; i < winners.size(); ++i)
	{
		const size_t matchIdx = i * static_cast<size_t>(this->_nbSetsToPlay);

		if (matchIdx >= this->_matches.size())
			break;

		Team* a = this->_matches[matchIdx]->getTeamA();
		Team* b = this->_matches[matchIdx]->getTeamB();

		losers.push_back((winners[i] == a) ? b : a);
	}

	return (losers);
}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				Phase::setIsFinished(cBool isFinished)	{	this->_isFinished = isFinished;	}

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHODS																					*/
/****************************************************************************************************/

void				Phase::addEncounter(pTeam a, pTeam b)
{
	if (!a || !b)
		return;

	for (int i = 0; i < this->_nbSetsToPlay; ++i)
		this->_matches.push_back(new Match(a, b));
}

bool				Phase::isFinished() const
{
	if (this->_matches.empty())
		return (false);

	for (const auto m : this->_matches)
		if (!m->isFinished())
			return (false);

	return (true);
}