//
// Created by Nicolas Fordoxcel on 24/06/2026.
//

//	STDLIB
# include <utility>

//	INCLUDES
#include "../includes/class/Phase.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VP_MATCH	=	std::vector<Match*>;
using				VP_TEAM		=	std::vector<Team*>;

//	STATIC VARIABLES

/****************/
/*	CANONICAL	*/
/****************/

Phase::Phase(STRING name, const int nbSets): _name(std::move(name)), _nbSetsToPlay(nbSets), _isFinished(false)
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

/************/
/*	GETTER	*/
/************/

C_STRING			Phase::getName() const
{
	return (this->_name);
}

int					Phase::getNbSetToPlay() const
{
	return (this->_nbSetsToPlay);
}

bool				Phase::getIsFinished() const
{
	return (this->_isFinished);
}

const VP_MATCH&		Phase::getMatches() const
{
	return (this->_matches);
}


VP_TEAM				Phase::getWinners() const
{
	VP_TEAM			winners;
	
	for (size_t i = 0; i < this->_matches.size(); i += this->_nbSetsToPlay)
	{
		int			winsA = 0;
		int			winsB = 0;
		
		Team*		a = this->_matches[i]->getTeamA();
		Team*		b = this->_matches[i]->getTeamB();

		for (int j = 0; j < this->_nbSetsToPlay; ++j)
		{
			if (this->_matches[i + j]->getWinner() == a)
				winsA++;
			else if (this->_matches[i + j]->getWinner() == b)
				winsB++;
		}
		
		if (winsA > winsB)
			winners.push_back(a);
		else
			winners.push_back(b);
	}

	return (winners);
}

/************/
/*	SETTER	*/
/************/

void				Phase::setIsFinished(const bool isFinished)
{
	this->_isFinished = isFinished;
}

/********************/
/*	PRIVATE METHODS	*/
/********************/

/********************/
/*	PUBLIC METHODS	*/
/********************/

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