//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB

//	INCLUDES
#include "../includes/class/Team.hpp"

//	TYPEDEF
using				C_STRING	=	const std::string&;
using				CVP_PART	=	const std::vector<Participant*>&;

//	STATIC VARIABLES
int									Team::_idCounter = 0;

/****************/
/*	CANONICAL	*/
/****************/

Team::Team() :	_id(++_idCounter), _point(0), _scoreMarked(0), _scoreAgainst(0),
				_isMixed(false), _isEliminated(false), _hasMultiTeamPlayer(false),
				_name("Team " + std::to_string(_idCounter))
{}

/************/
/*	GETTER	*/
/************/

/**
 * Indique le nombre de membres dans la team
 */
size_t				Team::getSize() const
{
	return (this->_members.size());
}

/**
 * Indique si une equipe est mixte ou unigenre
 */
bool				Team::getIsMixed() const
{
	return (this->_isMixed);
}

/**
 * Indique si une equipe est eliminee ou pas
 */
bool				Team::getIsEliminated() const
{
	return (this->_isEliminated);
}

/**
 * Indique si une equipe est constitue d un joueur qui joue dans une autre equipe
 */
bool				Team::getHasMultiTeamPlayer() const
{
	return (this->_hasMultiTeamPlayer);
}

/**
 * Indique l id de la team
 */
int					Team::getId() const
{
	return (this->_id);			
}

/**
 * Indique le nombre de points de la team
 */
int					Team::getPoint() const
{
	return (this->_point);
}

/**
 * indique le nombre de point total marque pendant tous les matchs
 */
int					Team::getScoreMarked() const
{
	return (this->_scoreMarked);
}

/**
 * Indique le nombre de point concede pendant tous les matchs
 */
int					Team::getScoreAgainst() const
{
	return (this->_scoreAgainst);
}

/**
 * Indique la difference entre le nombre de point marque et le nombre de point concede
 * pendant tous les matchs
 */
int					Team::getScoreDiff() const
{
	return (this->_scoreMarked - this->_scoreAgainst);
}

/**
 * Indique le nom de l equipe
 */
C_STRING			Team::getName() const
{
	return (this->_name);		
}

/**
 * Indique la liste des participants qui constitue cette equipe
 */
CVP_PART			Team::getMembers() const
{
	return (this->_members);	
}

/************/
/*	SETTER	*/
/************/

/**
 * Set l equipe comme mixte
 */
void				Team::setIsMixed(const bool value)
{
	this->_isMixed = value;
}

/**
 * Set l equipe comme eliminee
 */
void				Team::setIsEliminated(const bool value)
{
	this->_isEliminated = value;
}

/**
 * Set l equipe comme constitue d un joueur qui joue dans plusieurs equipes
 */
void				Team::setHasMultiTeamPlayer(const bool value)
{
	this->_hasMultiTeamPlayer = value;
}

/**
 * Set le nom d une equipe
 */
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

	std::string newName = "";

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