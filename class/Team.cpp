//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB

//	INCLUDES
#include "../includes/Team.hpp"

//	TYPEDEF
typedef const std::string&			C_STRING;
typedef std::vector<Participant*>	V_PART;

//	STATIC VARIABLES
int									Team::_idCounter = 0;

/****************/
/*	CANONICAL	*/
/****************/

Team::Team() :	_id(++_idCounter), _point(0), _scoreMarked(0), _scoreAgainst(0),
				_isEliminated(false), _hasMultiTeamPlayer(false),
				_name("Team " + std::to_string(_idCounter)) {}

Team::~Team() {}

/************/
/*	GETTER	*/
/************/

size_t								Team::getSize() const
{
	return (this->_members.size());
}

bool								Team::getIsEliminated() const
{
	return (this->_isEliminated);
}

bool								Team::getHasMultiTeamPlayer() const
{
	return (this->_hasMultiTeamPlayer);
}

int									Team::getId() const
{
	return (this->_id);			
}

int									Team::getPoint() const
{
	return (this->_point);
}

int									Team::getScoreMarked() const
{
	return (this->_scoreMarked);
}

int									Team::getScoreAgainst() const
{
	return (this->_scoreAgainst);
}

int									Team::getScoreDiff() const
{
	return (this->_scoreMarked - this->_scoreAgainst);
}

C_STRING							Team::getName() const
{
	return (this->_name);		
}

const V_PART&						Team::getMembers() const
{
	return (this->_members);	
}

/************/
/*	SETTER	*/
/************/

void								Team::setIsEliminated(bool isEliminated)
{
	this->_isEliminated = isEliminated;
}

void								Team::setHasMultiTeamPlayer(bool hasMultiTeamPlayer)
{
	this->_hasMultiTeamPlayer = hasMultiTeamPlayer;
}

void								Team::setName(C_STRING name)
{
	this->_name = name;
}

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

bool								Team::isComplete(int requiredSize) const
{
	return (this->_members.size() == static_cast<size_t>(requiredSize));
}

void								Team::addMember(Participant* member)
{
	if (member)
		this->_members.push_back(member);
}

void								Team::addPoint(int point)
{
	this->_point += point;
}

void								Team::addScoreMarked(int score)
{
	this->_scoreMarked += score;
}

void								Team::addScoreAgainst(int score)
{
	this->_scoreAgainst += score;
}