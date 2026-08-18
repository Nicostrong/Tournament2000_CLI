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
int									Team::_idCounter = 0;

/************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																	*/
/************************************************************************************************/

Team::Team():	_id(++_idCounter), _point(0), _scoreMarked(0), _scoreAgainst(0), _isMixed(false),
	_isEliminated(false), _hasMultiTeamPlayer(false), _name("Team " + std::to_string(_idCounter))
{}

/************************************************************************************************/
/*	GETTER																						*/
/************************************************************************************************/

size_t				Team::getSize() const				{	return (this->_members.size());						}
bool				Team::getIsMixed() const			{	return (this->_isMixed);							}
bool				Team::getIsEliminated() const		{	return (this->_isEliminated);						}
bool				Team::getHasMultiTeamPlayer() const	{	return (this->_hasMultiTeamPlayer);					}
int					Team::getId() const					{	return (this->_id);									}
int					Team::getPoint() const				{	return (this->_point);								}
int					Team::getScoreMarked() const		{	return (this->_scoreMarked);						}
int					Team::getScoreAgainst() const		{	return (this->_scoreAgainst);						}
int					Team::getScoreDiff() const			{	return (this->_scoreMarked - this->_scoreAgainst);	}
cString				Team::getName() const				{	return (this->_name);								}
cvpPart				Team::getMembers() const			{	return (this->_members);							}

/************************************************************************************************/
/*	SETTER																						*/
/************************************************************************************************/

void				Team::setIsMixed(const bool value)				{	this->_isMixed = value;				}
void				Team::setIsEliminated(const bool value)			{	this->_isEliminated = value;		}
void				Team::setHasMultiTeamPlayer(const bool value)	{	this->_hasMultiTeamPlayer = value;	}
void				Team::setName(cString value)					{	this->_name = value;				}

/************************************************************************************************/
/*	PRIVATE METHODS																				*/
/************************************************************************************************/

/************************************************************************************************/
/*	PUBLIC METHODS																				*/
/************************************************************************************************/

/**
 * Indique si une equipe est complete ou pas
 */
bool				Team::isComplete(const int requiredSize) const
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
		return ;

	string newName = "";

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
void				Team::addPoint(const int point)
{
	this->_point += point;
}

/**
 * Cumule les points marque contre une autre equipe
 */
void				Team::addScoreMarked(const int score)
{
	this->_scoreMarked += score;
}

/**
 * Cumule les points encaisse contre une autre equipe
 */
void				Team::addScoreAgainst(const int score)
{
	this->_scoreAgainst += score;
}