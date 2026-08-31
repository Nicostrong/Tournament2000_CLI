//
// Created by Nicolas Fordoxcel on 28/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include "../includes/class/Team.hpp"
#include "../includes/class/Player.hpp"
#include "../includes/class/Settings.hpp"

#include "../includes/manager/TeamManager.hpp"

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

TeamManager::TeamManager(cSet settings) : _teamFactory(settings) {}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

vpTeam				TeamManager::getTeams() const
{
	vpTeam teams;

	teams.reserve(this->_teams.size());

	for (const uTeam& team: this->_teams)
		if (team)
			teams.push_back(team.get());

	return (teams);
}

pTeam				TeamManager::getTeamById(int id) const
{
	if (id < 0)
		return (nullptr);

	for (const uTeam& team: this->_teams)
	{
		if (!team)
			continue;

		if (static_cast<int>(team->getId()) == id)
			return(team.get());
	}

	return (nullptr);
}

size_t				TeamManager::getSize() const
{
	return (this->_teams.size());
}

bool				TeamManager::isEmpty() const
{
	return (this->_teams.empty());
}

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				TeamManager::clean()
{
	this->_teams.clear();
}

void				TeamManager::generateTeams(cvpPlayer players)
{
	if (!this->_teams.empty())
		return;

	this->_teams = this->_teamFactory.generateTeams(players);
}