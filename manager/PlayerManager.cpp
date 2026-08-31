//
// Created by Nicolas Fordoxcel on 28/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <memory>
#include <fstream>
#include <sstream>


#include "../includes/class/Player.hpp"
#include "../includes/class/Settings.hpp"

#include "../includes/manager/PlayerManager.hpp"

#include "../includes/utils/CheckerCSV.hpp"
#include "../includes/utils/FormatUtils.hpp"

#include "../includes/Constantes.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cBool			=	const bool;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

PlayerManager::PlayerManager(cSet settings) : _settings(settings) {}

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

bool				PlayerManager::isPseudoAvailable(cString pseudo, pPlayer ignored) const
{
	for (const uPlayer& player : this->_players)
	{
		if (!player || player.get() == ignored)
			continue;

		if (player->getPseudo() == pseudo)
			return (false);
	}

	return (true);
}

uPlayer				PlayerManager::createPlayer(cString pseudo, cString lastName, cString firstName, Gender gender) const
{
	return (std::make_unique<Player>(pseudo, lastName, firstName, gender));
}


/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

vpPlayer				PlayerManager::getPlayers() const
{
	vpPlayer players;

	players.reserve(this->_players.size());

	for (const uPlayer& player : this->_players)
	{
		if (player)
			players.push_back(player.get());
	}

	return (players);
}

pPlayer				PlayerManager::getPlayerById(size_t id) const
{
	for (const uPlayer& player : this->_players)
		if (player && player->getId() == id)
			return (player.get());

	return (nullptr);
}

pPlayer				PlayerManager::getPlayerByPseudo(cString pseudo) const
{
	for (const uPlayer& player : this->_players)
		if (player && player->getPseudo() == pseudo)
			return (player.get());

	return (nullptr);
}

size_t				PlayerManager::getSize() const
{
	return (this->_players.size());
}

bool				PlayerManager::isEmpty() const
{
	return (this->_players.empty());
}

/****************************************************************************************************/
/*	MANAGEMENT																						*/
/****************************************************************************************************/

bool				PlayerManager::addPlayer(cString pseudo, cString lastName, cString firstName, Gender gender)
{
	if (this->getSize() >= static_cast<size_t>(this->_settings.getNbPlayers()))
		return (false);

	if (!this->_settings.getIsMixed() && gender != this->_settings.getTournamentGender())
		return (false);

	if (!this->isPseudoAvailable(pseudo))
		return (false);

	uPlayer player = this->createPlayer(pseudo, lastName, firstName, gender);

	if (!player)
		return (false);

	this->_players.push_back(std::move(player));

	return (true);
}

bool				PlayerManager::addPlayer(uPlayer player)
{
	if (!player)
		return (false);

	if (this->getSize() >= static_cast<size_t>(this->_settings.getNbPlayers()))
		return (false);
	
	if (!this->_settings.getIsMixed() && player->getGenderInt() != this->_settings.getTournamentGender())
		return (false);

	if (!this->isPseudoAvailable( player->getPseudo() ))
		return (false);
	
	this->_players.push_back(std::move(player));

	return (true);
}

bool				PlayerManager::removePlayer(size_t id)
{
	for (auto it = this->_players.begin(); it != this->_players.end(); ++it)
	{
		if (*it && (*it)->getId() == id)
		{
			this->_players.erase(it);
			return (true);
		}
	}
	
	return (false);
}

bool				PlayerManager::modifyPlayer(size_t id, cString pseudo, cString lastName, cString firstName, Gender gender)
{
	pPlayer player = this->getPlayerById(id);

	if (!player)
		return (false);

	if (!this->_settings.getIsMixed() && gender != this->_settings.getTournamentGender())
		return (false);

	if (!this->isPseudoAvailable(pseudo, player))
		return (false);

	player->setPseudo(pseudo);
	player->setLastName(lastName);
	player->setFirstName(firstName);
	player->setGender(gender);

	return (true);
}

void				PlayerManager::clear()
{
	this->_players.clear();
}
