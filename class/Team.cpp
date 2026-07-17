//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB

//	INCLUDES
#include "../includes/class/Team.hpp"

//	TYPEDEF
using				C_STRING	=	const std::string&;
using				VP_PART		=	std::vector<Participant*>;
using				CVP_PART	=	const VP_PART&;

//	STATIC VARIABLES
int									Team::_idCounter = 0;

/****************/
/*	CANONICAL	*/
/****************/

Team::Team() :	_id(++_idCounter), _point(0), _scoreMarked(0), _scoreAgainst(0),
				_isEliminated(false), _hasMultiTeamPlayer(false),
				_name("Team " + std::to_string(_idCounter))
{}

/************/
/*	GETTER	*/
/************/

size_t				Team::getSize() const
{
	return (this->_members.size());
}

bool				Team::getIsEliminated() const
{
	return (this->_isEliminated);
}

bool				Team::getHasMultiTeamPlayer() const
{
	return (this->_hasMultiTeamPlayer);
}

int					Team::getId() const
{
	return (this->_id);			
}

int					Team::getPoint() const
{
	return (this->_point);
}

int					Team::getScoreMarked() const
{
	return (this->_scoreMarked);
}

int					Team::getScoreAgainst() const
{
	return (this->_scoreAgainst);
}

int					Team::getScoreDiff() const
{
	return (this->_scoreMarked - this->_scoreAgainst);
}

C_STRING			Team::getName() const
{
	return (this->_name);		
}

CVP_PART			Team::getMembers() const
{
	return (this->_members);	
}

/************/
/*	SETTER	*/
/************/

void				Team::setIsEliminated(const bool value)
{
	this->_isEliminated = value;
}

void				Team::setHasMultiTeamPlayer(const bool value)
{
	this->_hasMultiTeamPlayer = value;
}

void				Team::setName(C_STRING value)
{
	this->_name = value;
}

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

bool				Team::isComplete(const int requiredSize) const
{
	return (this->_members.size() == static_cast<size_t>(requiredSize));
}

void				Team::addMember(Participant* member)
{
	if (member)
		this->_members.push_back(member);
}

void				Team::addPoint(const int point)
{
	this->_point += point;
}

void				Team::addScoreMarked(const int score)
{
	this->_scoreMarked += score;
}

void				Team::addScoreAgainst(const int score)
{
	this->_scoreAgainst += score;
}