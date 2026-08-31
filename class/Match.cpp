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

using				pTeam			=	Team*;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Match::Match(pTeam a, pTeam b, ScoreRules rules)
	: _teamA(a), _teamB(b), _scoreA(0), _scoreB(0), _isFinished(false), _rules(rules) {}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

pTeam				Match::getTeamA() const		{	return (this->_teamA);		}
pTeam				Match::getTeamB() const		{	return (this->_teamB);		}
int					Match::getScoreA() const	{	return (this->_scoreA);		}
int					Match::getScoreB() const	{	return (this->_scoreB);		}
bool				Match::isFinished() const	{	return (this->_isFinished);	}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				Match::setScoreA(cInt value)			{	this->_scoreA = value;		}
void				Match::setScoreB(cInt value)			{	this->_scoreB = value;		}
void				Match::setTeamA(pTeam value)		{	this->_teamA = value;		}
void				Match::setTeamB(pTeam value)		{	this->_teamB = value;		}
void				Match::setIsFinished(cBool value)	{	this->_isFinished = value;	}

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

bool				Match::isDraw() const
{
	return (this->_scoreA == this->_scoreB);
}

bool				Match::checkScore(cInt sA, cInt sB) const
{
	if (sA < 0 || sB < 0 || sA == sB)
		return (false);

	cInt winnerScore = std::max(sA, sB);
	cInt loserScore = std::min(sA, sB);
	cInt diff = winnerScore - loserScore;

	if (winnerScore > this->_rules.scoreMaxToWin)
		return (false);

	if (winnerScore == this->_rules.scoreMaxToWin)
		return (diff <= this->_rules.diffPointsToWin);

	if (winnerScore < this->_rules.scoreToWin)
		return (false);

	if (winnerScore == this->_rules.scoreToWin)
		return (loserScore <= (this->_rules.scoreToWin - this->_rules.diffPointsToWin));

	return (diff == this->_rules.diffPointsToWin);
}

void				Match::applyStats(cInt sA, cInt sB, cInt multiplier) const
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

pTeam				Match::getWinner() const
{
	if (!this->_isFinished || this->isDraw())
		return (nullptr);

	return ((this->_scoreA > this->_scoreB) ? this->_teamA : this->_teamB);
}

pTeam				Match::getLoser() const
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

bool				Match::setScore(cInt sA, cInt sB)
{
	if (!checkScore(sA, sB))
		return (false);

	setScoreA(sA);
	setScoreB(sB);

	this->applyStats(sA, sB, 1);
	this->_isFinished = true;
	return (true);
}

void				Match::resetScore()
{
	if (this->_isFinished)
		applyStats(this->_scoreA, this->_scoreB, -1);

	this->_scoreA = 0;
	this->_scoreB = 0;
	this->_isFinished = false;
}