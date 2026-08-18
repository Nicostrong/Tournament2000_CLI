//
// Created by Nicolas Fordoxcel on 24/06/2026.
//

//	STDLIB
#include <string>
#include <vector>
#include <utility>

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

Phase::Phase(string name, const int nbSets): _name(std::move(name)), _nbSetsToPlay(nbSets), _isFinished(false)
{
	this->_matches.clear();
}

Phase::~Phase()
{
	if (!this->_matches.empty())
		for (const Match* m : this->_matches)
			delete m;
	
	this->_matches.clear();
}

/************************************************************************************************/
/*	GETTER																						*/
/************************************************************************************************/

cString				Phase::getName() const			{	return (this->_name);		}
int					Phase::getNbSetToPlay() const	{	return (this->_nbSetsToPlay);	}
bool				Phase::getIsFinished() const	{	return (this->_isFinished);		}
cvpMatch			Phase::getMatches() const		{	return (this->_matches);		}

vpTeam				Phase::getWinners() const
{
	vpTeam winners;	

	for (size_t i = 0; i < this->_matches.size(); i += this->_nbSetsToPlay)
	{
		int winsA = 0;
		int winsB = 0;
		Team* a = this->_matches[i]->getTeamA();
		Team* b = this->_matches[i]->getTeamB();

		for (int j = 0; j < this->_nbSetsToPlay; ++j)
		{
			if (this->_matches[i + j]->getWinner() == a)
				winsA++;
			else if (this->_matches[i + j]->getWinner() == b)
				winsB++;
		}
		
		winners.push_back(winsA >= winsB ? a : b);
	}

	return (winners);
}

vpTeam				Phase::getLosers() const
{
	vpTeam losers;
	vpTeam winners = getWinners();

	for (size_t i = 0; i < winners.size(); ++i)
	{
		const size_t matchIdx = i * static_cast<size_t>(this->_nbSetsToPlay);

		if (matchIdx >= this->_matches.size())
			break ;

		Team* a = this->_matches[matchIdx]->getTeamA();
		Team* b = this->_matches[matchIdx]->getTeamB();

		losers.push_back((winners[i] == a) ? b : a);
	}

	return (losers);
}

/************************************************************************************************/
/*	SETTER																						*/
/************************************************************************************************/

void				Phase::setIsFinished(const bool isFinished)	{	this->_isFinished = isFinished;	}

/************************************************************************************************/
/*	PRIVATE METHODS																				*/
/************************************************************************************************/

/************************************************************************************************/
/*	PUBLIC METHODS																				*/
/************************************************************************************************/

void				Phase::addEncounter(Team* a, Team* b)
{
	if (!a || !b)
		return ;

	for (int i = 0; i < this->_nbSetsToPlay; ++i)
		this->_matches.push_back(new Match(a, b));
}

bool				Phase::isFinished() const
{
	if (this->_matches.empty())
		return (false);

	for (const auto m : this->_matches)
		if (!m->isFinished())
			return (false);

	return (true);
}