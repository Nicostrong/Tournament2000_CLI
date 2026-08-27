//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <algorithm>

#include "../includes/class/Team.hpp"
#include "../includes/class/Participant.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

using				pTeam			=	Team*;
using				cTeam			=	const Team&;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

int					Team::_idCounter = 0;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Team::Team():	_id(++_idCounter), _point(0), _scoreMarked(0), _scoreAgainst(0), _isMixed(false),
	_isEliminated(false), _isDisqualified(false), _hasMultiTeamPlayer(false),
	_name("Team " + std::to_string(_idCounter))
{}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

size_t				Team::getSize() const				{	return (this->_members.size());						}
bool				Team::getIsMixed() const			{	return (this->_isMixed);							}
bool				Team::getIsEliminated() const		{	return (this->_isEliminated);						}
bool				Team::getIsDisqualified() const		{	return (this->_isDisqualified);						}
bool				Team::getHasMultiTeamPlayer() const	{	return (this->_hasMultiTeamPlayer);					}
int					Team::getId() const					{	return (this->_id);									}
int					Team::getPoint() const				{	return (this->_point);								}
int					Team::getScoreMarked() const		{	return (this->_scoreMarked);						}
int					Team::getScoreAgainst() const		{	return (this->_scoreAgainst);						}
int					Team::getScoreDiff() const			{	return (this->_scoreMarked - this->_scoreAgainst);	}
cString				Team::getName() const				{	return (this->_name);								}
cvpPart				Team::getMembers() const			{	return (this->_members);							}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				Team::setIsMixed(cBool value)				{	this->_isMixed = value;				}
void				Team::setIsEliminated(cBool value)			{	this->_isEliminated = value;		}
void				Team::setIsDisqualified(cBool value)		{	this->_isDisqualified = value;		}
void				Team::setHasMultiTeamPlayer(cBool value)	{	this->_hasMultiTeamPlayer = value;	}
void				Team::setName(cString value)				{	this->_name = value;				}

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHODS																					*/
/****************************************************************************************************/

/**
 * Indique si une equipe est complete ou pas
 */
bool				Team::isComplete(cInt requiredSize) const
{
	return (this->_members.size() == static_cast<size_t>(requiredSize));
}

/**
 * Ajoute un membre a une equipe
 */
void				Team::addMember(Participant* member)
{
	if (member)
		this->_members.push_back(member);
}

/**
 *	Renome le nom de la team en prenant les pseudo des players
 */
void				Team::renameTeam()
{
	if (this->_members.empty())
		return;

	String newName;

	for (size_t i = 0; i < this->_members.size(); ++i)
	{
		if (this->_members[i])
		{
			if (i > 0)
				newName += " & ";
			
			newName += this->_members[i]->getPseudo();
		}
	}

	if (!newName.empty())
		this->_name = newName;
}

/**
 * Cumule les points de la rencontre
 */
void				Team::addPoint(cInt point)
{
	this->_point += point;
}

/**
 * Cumule les points marque contre une autre equipe
 */
void				Team::addScoreMarked(cInt score)
{
	this->_scoreMarked += score;
}

/**
 * Cumule les points encaisse contre une autre equipe
 */
void				Team::addScoreAgainst(cInt score)
{
	this->_scoreAgainst += score;
}

bool				Team::hasMember(cpPart p) const
{
	return (std::ranges::find(this->_members, p) != this->_members.end());
}

bool				Team::sharesMemberWith(cpTeam other) const
{
	if (!other)
		return (false);

	return (std::ranges::any_of(this->_members, [other](const auto* m)
		{
			return (other->hasMember(m));
		}));
}

bool				Team::replaceMember(size_t index, Participant* newMember)
{
	if (index < this->_members.size() && newMember != nullptr && newMember->getIsEliminated())
	{
		this->_members[index]->setIsEliminated(true);
		newMember->setIsEliminated(false);
		this->_members[index] = newMember;
		
		return (true);
	}
	return (false);
}

void				Team::disqualifyTeam(bool action)
{
	size_t nbMembers = this->_members.size();

	for (size_t i = 0; i < nbMembers; ++i)
		if (!this->_members[i]->getIsMultiTeamPlayer())
			this->_members[i]->setIsEliminated(action);
	
	setIsDisqualified(action);
}