//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB

//	INCLUDES
#include "../includes/class/Match.hpp"

//	TYPEDEF

//	STATIC VARIABLES

/****************/
/*	CANONICAL	*/
/****************/

Match::Match(Team* a, Team* b)
	: _teamA(a), _teamB(b), _scoreA(0), _scoreB(0), _isFinished(false)
{}

/************/
/*	GETTER	*/
/************/

Team*				Match::getTeamA() const
{
	return (this->_teamA);
}

Team*				Match::getTeamB() const
{
	return (this->_teamB);
}

int					Match::getScoreA() const
{
	return (this->_scoreA);
}

int					Match::getScoreB() const
{
	return (this->_scoreB);
}

bool				Match::isFinished() const
{
	return (this->_isFinished);
}

/************/
/*	SETTER	*/
/************/

void				Match::setScore(const int sA, const int sB)
{
	this->_scoreA = sA;
	this->_scoreB = sB;

	this->_teamA->addScoreMarked(sA);
	this->_teamA->addScoreAgainst(sB);

	this->_teamB->addScoreMarked(sB);
	this->_teamB->addScoreAgainst(sA);
	
	if (sA > sB)
		this->_teamA->addPoint(3);
	else if (sA < sB)
		this->_teamB->addPoint(3);
	else
	{
		this->_teamA->addPoint(1);
		this->_teamB->addPoint(1);
	}

	this->_isFinished = true;
}

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

Team*				Match::getWinner() const
{
	if (!this->_isFinished || this->_scoreA == this->_scoreB)
		return (nullptr);

	return ((this->_scoreA > this->_scoreB) ? this->_teamA : this->_teamB);
}

Team*				Match::getLoser() const
{
	if (!this->_isFinished || this->_scoreA == this->_scoreB)
		return (nullptr);

	return ((this->_scoreA < this->_scoreB) ? this->_teamA : this->_teamB);
}