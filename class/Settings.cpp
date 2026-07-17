//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

//	STDLIB
#include <string>

//	INCLUDES
#include "../includes/class/Settings.hpp"
#include "../includes/Constantes.hpp"
#include "../includes/Errors.hpp"

//	TYPEDEF
using				C_STRING	=	const std::string&;
using				V_STRING	=	std::vector<std::string>;

//	STATIC VARIABLES

/****************/
/*	CANONICAL	*/
/****************/

Settings::Settings()
    :	_name(TOURNAMENTNAME), _nbPlayers(NBPLAYER), _nbPlayerByPool(NBPLAYERPERPOOL),
		_nbPools(NBPOOL), _nbBadmintonCourt(NBTERRAIN), _scoreMin(SCOREMIN), _scoreMax(SCOREMAX),
		_diffPointsToWin(ECART), _nbSetPlayedPools(NBPLAYERPERPOOL), _nbSetPlayedSixteenth(NBSETSIXTEENTH),
		_nbSetPlayedHeigth(NBSETHEIGTH), _nbSetPlayedQuarters(NBSETQUARTER), _nbSetPlayedSemis(NBSETSEMI),
    	_nbSetPlayedFinal(NBSETFINAL), _nbSetPlayedThirdPlace(NBSETTHIRD), _isMixed(ISMIXED),
    	_isDouble(ISDOUBLE), _allowMultiTeamPlayers(PLAYERMULTITEAM), _isThirdPlaceMatch(PLAYTHIRDPLACE)
{}

Settings&			Settings::operator=(const Settings& s)
{
	if (this != &s)
	{
		this->_name = s._name;
		this->_nbPlayers = s._nbPlayers;
		this->_nbPlayerByPool = s._nbPlayerByPool;
		this->_nbPools = s._nbPools;
		this->_nbBadmintonCourt = s._nbBadmintonCourt;
		this->_scoreMin = s._scoreMin;
		this->_scoreMax = s._scoreMax;
		this->_diffPointsToWin = s._diffPointsToWin;
		this->_nbSetPlayedPools = s._nbSetPlayedPools;
		this->_nbSetPlayedSixteenth = s._nbSetPlayedSixteenth;
		this->_nbSetPlayedHeigth = s._nbSetPlayedHeigth;
		this->_nbSetPlayedQuarters = s._nbSetPlayedQuarters;
		this->_nbSetPlayedSemis = s._nbSetPlayedSemis;
		this->_nbSetPlayedFinal = s._nbSetPlayedFinal;
		this->_nbSetPlayedThirdPlace = s._nbSetPlayedThirdPlace;
		this->_isMixed = s._isMixed;
		this->_isDouble = s._isDouble;
		this->_allowMultiTeamPlayers = s._allowMultiTeamPlayers;
		this->_isThirdPlaceMatch = s._isThirdPlaceMatch;
	}

	return (*this);
}

/************/
/*	GETTER	*/
/************/

C_STRING			Settings::getName() const
{
	return (this->_name);
}

int					Settings::getNbPlayers() const
{
	return (this->_nbPlayers);
}

int					Settings::getNbPlayerByPool() const
{
	return (this->_nbPlayerByPool);
}

int					Settings::getNbPools() const
{
	return (this->_nbPools);
}

int					Settings::getNbBadmintonCourt() const
{
	return (this->_nbBadmintonCourt);
}

int					Settings::getScoreMin() const
{
	return (this->_scoreMin);
}

int					Settings::getScoreMax() const
{
	return (this->_scoreMax);
}

int					Settings::getDiffPointsToWin() const
{
	return (this->_diffPointsToWin);
}

int					Settings::getNbSetPlayedPools() const
{
	return (this->_nbSetPlayedPools);
}

int					Settings::getNbSetPlayedSixteenth() const
{
	return (this->_nbSetPlayedSixteenth);
}

int					Settings::getNbSetPlayedHeigth() const
{
	return (this->_nbSetPlayedHeigth);
}

int					Settings::getNbSetPlayedQuarters() const
{
	return (this->_nbSetPlayedQuarters);
}

int					Settings::getNbSetPlayedSemis() const
{
	return (this->_nbSetPlayedSemis);
}

int					Settings::getNbSetPlayedFinal() const
{
	return (this->_nbSetPlayedFinal);
}

int					Settings::getNbSetPlayedThirdPlace() const
{
	return (this->_nbSetPlayedThirdPlace);
}

bool				Settings::getIsMixed() const
{
	return (this->_isMixed);
}

bool				Settings::getIsDouble() const
{
	return (this->_isDouble);
}

bool				Settings::getAllowMultiTeamPlayers() const
{
	return (this->_allowMultiTeamPlayers);
}

bool				Settings::getIsThirdPlaceMatch() const
{
	return (this->_isThirdPlaceMatch);
}

/************/
/*	SETTER	*/
/************/

void				Settings::setName(C_STRING value)
{
	this->_name = value;
}

void				Settings::setNbPlayers(const int value)
{
	this->_nbPlayers = value;
}

void				Settings::setNbPlayerByPool(const int value)
{
	this->_nbPlayerByPool = value;
}

void				Settings::setNbPools(const int value)
{
	this->_nbPools = value;
}

void				Settings::setNbBadmintonCourt(const int value)
{
	this->_nbBadmintonCourt = value;
}

void				Settings::setScoreMin(const int value)
{
	this->_scoreMin = value;
}

void				Settings::setScoreMax(const int value)
{
	this->_scoreMax = value;
}

void				Settings::setDiffPointsToWin(const int value)
{
	this->_diffPointsToWin = value;
}

void				Settings::setNbSetPlayedPools(const int value)
{
	this->_nbSetPlayedPools = value;
}

void				Settings::setNbSetPlayedSixteenth(const int value)
{
	this->_nbSetPlayedSixteenth = value;
}

void				Settings::setNbSetPlayedHeigth(const int value)
{
	this->_nbSetPlayedHeigth = value;
}

void				Settings::setNbSetPlayedQuarters(const int value)
{
	this->_nbSetPlayedQuarters = value;
}

void				Settings::setNbSetPlayedSemis(const int value)
{
	this->_nbSetPlayedSemis = value;
}

void				Settings::setNbSetPlayedFinal(const int value)
{
	this->_nbSetPlayedFinal = value;
}

void				Settings::setNbSetPlayedThirdPlace(const int value)
{
	this->_nbSetPlayedThirdPlace = value;
}

void				Settings::setIsMixed(const bool value)
{
	this->_isMixed = value;
}

void				Settings::setIsDouble(const bool value)
{
	this->_isDouble = value;
}

void				Settings::setAllowMultiTeamPlayers(const bool value)
{
	this->_allowMultiTeamPlayers = value;
}

void				Settings::setIsThirdPlaceMatch(const bool value)
{
	this->_isThirdPlaceMatch = value;
}

/********************/
/*	PRIVATE METHOD	*/
/********************/

bool				Settings::addErrorIf(const bool condition, C_STRING message, V_STRING& errors)
{
	if (condition)
	{
		errors.push_back(message);
		
		return (true);
	}

	return (false);
}

/********************/
/*	PUBLIC METHOD	*/
/********************/

bool				Settings::isValid(V_STRING& errors) const
{
	errors.clear();

	addErrorIf(this->_name.empty(), E_NAME, errors);
	addErrorIf(!isInList(this->_nbPlayers, allowedNbPlayers), E_NBPLAYER, errors);
	addErrorIf(!isInList(this->_nbPlayerByPool, allowedNbPlayersOrTeamsPerPools), E_NBPLAYERPERPOOL, errors);
	addErrorIf(!isInList(this->_nbPools, allowedNbPools), E_NBPOOL, errors);
	addErrorIf(this->_nbBadmintonCourt <= 0, E_NBTERRAIN, errors);
	addErrorIf(this->_scoreMin <= 0, E_SCOREMIN, errors);
	addErrorIf(this->_scoreMax <= this->_scoreMin, E_SCOREMAX, errors);
	addErrorIf(this->_diffPointsToWin <= 0, E_DIFFSCORE, errors);
	addErrorIf(!isInList(this->_nbSetPlayedPools, allowedNbSetToPlay), E_NBSETPOOL, errors);
	addErrorIf(!isInList(this->_nbSetPlayedSixteenth, allowedNbSetToPlay), E_NBSETSIXTEENTH, errors);
	addErrorIf(!isInList(this->_nbSetPlayedHeigth, allowedNbSetToPlay), E_NBSETHEIGHT, errors);
	addErrorIf(!isInList(this->_nbSetPlayedQuarters, allowedNbSetToPlay), E_NBSETQUARTER, errors);
	addErrorIf(!isInList(this->_nbSetPlayedSemis, allowedNbSetToPlay), E_NBSETSEMI, errors); 
	addErrorIf(!isInList(this->_nbSetPlayedFinal, allowedNbSetToPlay), E_NBSETFINAL, errors);

	if (this->_isThirdPlaceMatch)
		addErrorIf(!isInList(this->_nbSetPlayedThirdPlace, allowedNbSetToPlay), E_NBSETTHIRD, errors);

	return (errors.empty());
}

bool				Settings::canAccommodate(const int actualParticipants) const
{
	if ((this->getNbPlayers() - actualParticipants) > NBPLAYERINMULTITEAMMAX)
		return (false);
	
	return (true);
}