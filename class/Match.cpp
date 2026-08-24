//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include "../includes/class/Match.hpp"
#include "../includes/class/Team.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cInt			=	const int;
using				cBool			=	const bool;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Match::Match(pTeam a, pTeam b): _teamA(a), _teamB(b), _scoreA(0), _scoreB(0), _isFinished(false)
{}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

Team*				Match::getTeamA() const		{	return (this->_teamA);		}
Team*				Match::getTeamB() const		{	return (this->_teamB);		}
int					Match::getScoreA() const	{	return (this->_scoreA);		}
int					Match::getScoreB() const	{	return (this->_scoreB);		}
bool				Match::isFinished() const	{	return (this->_isFinished);	}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				Match::setScoreA(int value)			{	this->_scoreA = value;		}
void				Match::setScoreB(int value)			{	this->_scoreB = value;		}
void				Match::setIsFinished(cBool value)	{	this->_isFinished = value;	}
void				Match::setScore(cInt sA, cInt sB)
{
	setScoreA(sA);
	setScoreB(sB);

	this->applyStats(sA, sB, 1);
	this->_isFinished = true;
}

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

bool				Match::isDraw() const
{
	return (this->_scoreA == this->_scoreB);
}

void				Match::applyStats(cInt sA, cInt sB, int multiplier)
{
	this->_teamA->addScoreMarked(sA * multiplier);
	this->_teamA->addScoreAgainst(sB * multiplier);

	this->_teamB->addScoreMarked(sB * multiplier);
	this->_teamB->addScoreAgainst(sA * multiplier);
	
	if (sA > sB)
		this->_teamA->addPoint(3 * multiplier);
	else if (sA < sB)
		this->_teamB->addPoint(3 * multiplier);
	else
	{
		this->_teamA->addPoint(1 * multiplier);
		this->_teamB->addPoint(1 * multiplier);
	}
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

Team*				Match::getWinner() const
{
	if (!this->_isFinished || this->isDraw())
		return (nullptr);

	return ((this->_scoreA > this->_scoreB) ? this->_teamA : this->_teamB);
}

Team*				Match::getLoser() const
{
	if (!this->_isFinished || this->isDraw())
		return (nullptr);

	return ((this->_scoreA < this->_scoreB) ? this->_teamA : this->_teamB);
}

void				Match::modifyScore(cInt sA, cInt sB)
{
	if (!this->_isFinished)
		return;
	
	this->applyStats(this->_scoreA, this->_scoreB, -1);
	
	this->_scoreA = sA;
	this->_scoreB = sB;
	
	this->applyStats(this->_scoreA, this->_scoreB, 1);
}
