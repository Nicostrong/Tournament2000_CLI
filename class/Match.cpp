//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB
#include <string>
#include <vector>

//	INCLUDES
#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"
#include "../includes/class/Participant.hpp"

//	TYPEDEF
using				string		=	std::string;
using				cString		=	const std::string&;

using				cPool		=	const Pool&;
using				cTeam		=	const Team&;
using				cMatch		=	const Match&;
using				cPhase		=	const Phase&;
using				cSet		=	const Settings&;
using				cPart		=	const Participant&;

using				vpPool		=	std::vector<Pool*>;
using				vpTeam		=	std::vector<Team*>;
using				vpMatch		=	std::vector<Match*>;
using				vpPhase		=	std::vector<Phase*>;
using				vpPart		=	std::vector<Participant*>;

using				cvpPool		=	const std::vector<Pool*>&;
using				cvpTeam		=	const std::vector<Team*>&;
using				cvpMatch	=	const std::vector<Match*>&;
using				cvpPhase	=	const std::vector<Phase*>&;
using				cvpPart		=	const std::vector<Participant*>&;

//	STATIC VARIABLES

/************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																	*/
/************************************************************************************************/

Match::Match(Team* a, Team* b): _teamA(a), _teamB(b), _scoreA(0), _scoreB(0), _isFinished(false)
{}

/************************************************************************************************/
/*	GETTER																						*/
/************************************************************************************************/

Team*				Match::getTeamA() const		{	return (this->_teamA);		}
Team*				Match::getTeamB() const		{	return (this->_teamB);		}
int					Match::getScoreA() const	{	return (this->_scoreA);		}
int					Match::getScoreB() const	{	return (this->_scoreB);		}
bool				Match::isFinished() const	{	return (this->_isFinished);	}

/************************************************************************************************/
/*	SETTER																						*/
/************************************************************************************************/

/**
 *	Gere l ajout de score aux equipes lors d un match
 */
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

/************************************************************************************************/
/*	PRIVATE METHOD																				*/
/************************************************************************************************/

/************************************************************************************************/
/*	PUBLIC METHOD																				*/
/************************************************************************************************/

/**
 *	Retourne l equipe vainqueur
 */
Team*				Match::getWinner() const
{
	if (!this->_isFinished || this->_scoreA == this->_scoreB)
		return (nullptr);

	return ((this->_scoreA > this->_scoreB) ? this->_teamA : this->_teamB);
}

/**
 *	Retourne l equipe perdante
 */
Team*				Match::getLoser() const
{
	if (!this->_isFinished || this->_scoreA == this->_scoreB)
		return (nullptr);

	return ((this->_scoreA < this->_scoreB) ? this->_teamA : this->_teamB);
}