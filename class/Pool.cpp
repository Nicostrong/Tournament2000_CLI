//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB
#include <string>
#include <vector>
#include <algorithm>

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
int					Pool::_idCounter = 1;

/************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																	*/
/************************************************************************************************/

Pool::Pool(): _name("Pool " + std::to_string(_idCounter++))
{}

Pool::~Pool()
{
	for (const Match* m : this->_matches)
		delete m;
	
	this->_matches.clear();
}

/************************************************************************************************/
/*	GETTER																						*/
/************************************************************************************************/

cString				Pool::getName() const		{	return (this->_name);		}
cvpTeam				Pool::getTeams() const		{	return (this->_teams);		}
cvpMatch			Pool::getMatches() const	{	return (this->_matches);	}


/************************************************************************************************/
/*	SETTER																						*/
/************************************************************************************************/

/************************************************************************************************/
/*	PRIVATE METHODS																				*/
/************************************************************************************************/

/************************************************************************************************/
/*	PUBLIC METHODS																				*/
/************************************************************************************************/

/**
 *	Ajoute une equipe dans une pool
 */
void				Pool::addTeam(Team* team)
{
	if (team)
		this->_teams.push_back(team);
}

/**
 *	Genere le nombre de match a jouer dasn une phase du tournoi
 */
void				Pool::generateMatches(const int nbSetsPerEncounter)
{
	for (const Match* m : this->_matches)
		delete m;
	
	this->_matches.clear();

	if (this->_teams.size() < 2)
		return ;

	for (size_t i = 0; i < this->_teams.size(); ++i)
		for (size_t j = i + 1; j < this->_teams.size(); ++j)
			for (int s = 0; s < nbSetsPerEncounter; ++s)
				this->_matches.push_back(new Match(this->_teams[i], this->_teams[j]));
}

/**
 *	Trie les equipes pour le classement
 */
void				Pool::sortTeams()
{
	std::sort(this->_teams.begin(), this->_teams.end(), [](const Team* a, const Team* b)
	{
		if (a->getPoint() != b->getPoint())
			return (a->getPoint() > b->getPoint());

		return (a->getScoreDiff() > b->getScoreDiff());
	});
}

/**
 *	Verifie si tous les matchs sont finis
 */
bool				Pool::allMatchesFinished() const
{
	for (const Match* m: this->_matches)
		if (!m->isFinished())
			return (false);

	return (true);
}

/**
 *	Recupere les eauipes qualifiees
 */
vpTeam				Pool::getQualifiers() const
{
	vpTeam qualifiers;

	if (!this->_teams.empty())
		qualifiers.push_back(this->_teams[0]);

	if (this->_teams.size() >= 2)
		qualifiers.push_back(this->_teams[1]);

	return (qualifiers);
}