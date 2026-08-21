//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>

#include "../includes/class/Settings.hpp"

#include "../includes/Constantes.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cBool			=	const bool;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Settings::Settings(): _name(TOURNAMENTNAME), _nbPlayers(NBPLAYER), _nbPlayerByPool(NBPLAYERPERPOOL),
	_nbPools(NBPOOL), _nbBadmintonCourt(NBTERRAIN), _scoreMin(SCOREMIN), _scoreMax(SCOREMAX),
	_diffPointsToWin(ECART), _nbSetPlayedPools(NBPLAYERPERPOOL), _nbSetPlayedSixteenth(NBSETSIXTEENTH),
	_nbSetPlayedHeigth(NBSETHEIGTH), _nbSetPlayedQuarters(NBSETQUARTER), _nbSetPlayedSemis(NBSETSEMI),
    _nbSetPlayedFinal(NBSETFINAL), _nbSetPlayedThirdPlace(NBSETTHIRD), _isMixed(ISMIXED),
    _isDouble(ISDOUBLE), _allowMultiTeamPlayers(PLAYERMULTITEAM), _isThirdPlaceMatch(PLAYTHIRDPLACE),
	_isValid(ISVALIDE), _gender(GENDER)
{}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

cString				Settings::getName() const					{	return (this->_name);					}
int					Settings::getNbPlayers() const				{	return (this->_nbPlayers);				}
int					Settings::getNbPlayerByPool() const			{	return (this->_nbPlayerByPool);			}
int					Settings::getNbPools() const				{	return (this->_nbPools);				}
int					Settings::getNbBadmintonCourt() const		{	return (this->_nbBadmintonCourt);		}
int					Settings::getScoreMin() const				{	return (this->_scoreMin);				}
int					Settings::getScoreMax() const				{	return (this->_scoreMax);				}
int					Settings::getDiffPointsToWin() const		{	return (this->_diffPointsToWin);		}
int					Settings::getNbSetPlayedPools() const		{	return (this->_nbSetPlayedPools);		}
int					Settings::getNbSetPlayedSixteenth() const	{	return (this->_nbSetPlayedSixteenth);	}
int					Settings::getNbSetPlayedHeigth() const		{	return (this->_nbSetPlayedHeigth);		}
int					Settings::getNbSetPlayedQuarters() const	{	return (this->_nbSetPlayedQuarters);	}
int					Settings::getNbSetPlayedSemis() const		{	return (this->_nbSetPlayedSemis);		}
int					Settings::getNbSetPlayedFinal() const		{	return (this->_nbSetPlayedFinal);		}
int					Settings::getNbSetPlayedThirdPlace() const	{	return (this->_nbSetPlayedThirdPlace);	}
bool				Settings::getIsMixed() const				{	return (this->_isMixed);				}
bool				Settings::getIsDouble() const				{	return (this->_isDouble);				}
bool				Settings::getAllowMultiTeamPlayers() const	{	return (this->_allowMultiTeamPlayers);	}
bool				Settings::getIsThirdPlaceMatch() const		{	return (this->_isThirdPlaceMatch);		}
bool				Settings::getIsValid() const				{	return (this->_isValid);				}
Gender				Settings::getTournamentGender() const		{	return (this->_gender);					}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				Settings::setName(cString value)				{	this->_name = value;					}
void				Settings::setNbPlayers(cInt value)				{	this->_nbPlayers = value;				}
void				Settings::setNbPlayerByPool(cInt value)			{	this->_nbPlayerByPool = value;			}
void				Settings::setNbPools(cInt value)				{	this->_nbPools = value;					}
void				Settings::setNbBadmintonCourt(cInt value)		{	this->_nbBadmintonCourt = value;		}
void				Settings::setScoreMin(cInt value)				{	this->_scoreMin = value;				}
void				Settings::setScoreMax(cInt value)				{	this->_scoreMax = value;				}
void				Settings::setDiffPointsToWin(cInt value)		{	this->_diffPointsToWin = value;			}
void				Settings::setNbSetPlayedPools(cInt value)		{	this->_nbSetPlayedPools = value;		}
void				Settings::setNbSetPlayedSixteenth(cInt value)	{	this->_nbSetPlayedSixteenth = value;	}
void				Settings::setNbSetPlayedHeigth(cInt value)		{	this->_nbSetPlayedHeigth = value;		}
void				Settings::setNbSetPlayedQuarters(cInt value)	{	this->_nbSetPlayedQuarters = value;		}
void				Settings::setNbSetPlayedSemis(cInt value)		{	this->_nbSetPlayedSemis = value;		}
void				Settings::setNbSetPlayedFinal(cInt value)		{	this->_nbSetPlayedFinal = value;		}
void				Settings::setNbSetPlayedThirdPlace(cInt value)	{	this->_nbSetPlayedThirdPlace = value;	}
void				Settings::setIsMixed(cBool value)				{	this->_isMixed = value;					}
void				Settings::setIsDouble(cBool value)				{	this->_isDouble = value;				}
void				Settings::setAllowMultiTeamPlayers(cBool value)	{	this->_allowMultiTeamPlayers = value;	}
void				Settings::setIsThirdPlaceMatch(cBool value)		{	this->_isThirdPlaceMatch = value;		}
void				Settings::setIsValid(cBool value)				{	this->_isValid = value;					}
void				Settings::setTournamentGender(cGender value)	{	this->_gender = value;					}

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHODS																					*/
/****************************************************************************************************/
