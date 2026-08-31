//
// Created by Nicolas Fordoxcel on 24/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <utility>
#include <memory>

#include "../includes/class/Team.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Settings.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

using				cSet			=	const Settings&;

using				vpMatch			=	std::vector<Match*>;
using				uMatch			=	std::unique_ptr<Match>;

using				pTeam			=	Team*;
using				vpTeam			=	std::vector<Team*>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Phase::Phase(String name, cInt nbSets)
	: _name(std::move(name)), _nbSetsToPlay(nbSets), _isFinished(false) {}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

cString				Phase::getName() const			{	return (this->_name);			}
int					Phase::getNbSetToPlay() const	{	return (this->_nbSetsToPlay);	}
bool				Phase::getIsFinished() const	{	return (this->_isFinished);		}
vpMatch				Phase::getMatches() const
{
	vpMatch matches;

	matches.reserve(this->_matches.size());

	for (const uMatch& match : this->_matches)
		if (match)
			matches.push_back(match.get());

	return (matches);
}

vpTeam				Phase::getWinners() const
{
	vpTeam winners;

	if (this->_nbSetsToPlay <= 0)
		return (winners);

	for (size_t i = 0; i < this->_matches.size(); i += static_cast<size_t>(this->_nbSetsToPlay))
	{
		pTeam winner = this->getEncounterWinner(i);

		if (winner)
			winners.push_back(winner);
	}

	return (winners);
}

vpTeam				Phase::getLosers() const
{
	vpTeam losers;

	if (this->_nbSetsToPlay <= 0)
		return (losers);

	vpTeam winners = this->getWinners();

	for (size_t i = 0; i < winners.size(); ++i)
	{
		const size_t matchIdx = i * static_cast<size_t>(this->_nbSetsToPlay);

		if (matchIdx >= this->_matches.size())
			break;

		const uMatch& match = this->_matches[matchIdx];

		if (!match)
			continue;

		pTeam a = match->getTeamA();
		pTeam b = match->getTeamB();

		if (winners[i] == a)
		{
			if (b)
				losers.push_back(b);
		}
		else if (winners[i] == b)
		{
			if (a)
				losers.push_back(a);
		}
	}

	return (losers);
}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				Phase::setIsFinished(cBool isFinished)	{	this->_isFinished = isFinished;	}

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

Team*				Phase::getEncounterWinner(size_t index) const
{
	if (this->_nbSetsToPlay <= 0 || index >= this->_matches.size())
		return (nullptr);

	const uMatch& firstMatch = this->_matches[index];

	if (!firstMatch)
		return (nullptr);

	pTeam a = firstMatch->getTeamA();
	pTeam b = firstMatch->getTeamB();

	if (!a || !b)
		return (nullptr);

	int winsA = 0;
	int winsB = 0;

	const auto nbSets = static_cast<size_t>(this->_nbSetsToPlay);

	if (index + nbSets > this->_matches.size())
		return (nullptr);

	for (size_t j = 0; j < nbSets; ++j)
	{
		const uMatch& match = this->_matches[index + j];

		if (!match)
			return (nullptr);

		pTeam winner = match->getWinner();

		if (winner == a)
			++winsA;
		else if (winner == b)
			++winsB;
	}

	return (winsA >= winsB ? a : b);
}

/****************************************************************************************************/
/*	PUBLIC METHODS																					*/
/****************************************************************************************************/

void				Phase::addEncounter(pTeam a, pTeam b, cSet settings)
{
	if (!a || !b)
		return;

	if (this->_nbSetsToPlay <= 0)
		return;

	ScoreRules rules
	{
		settings.getScoreMin(),
		settings.getScoreMax(),
		settings.getDiffPointsToWin()
	};

	for (int i = 0; i < this->_nbSetsToPlay; ++i)
		this->_matches.push_back(std::make_unique<Match>(a, b, rules));
}

bool				Phase::isFinished() const
{
	if (this->_matches.empty())
		return (false);

	for (const uMatch& m : this->_matches)
		if (!m || !m->isFinished())
			return (false);

	return (true);
}