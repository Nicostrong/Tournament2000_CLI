//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>

#include "../includes/class/Settings.hpp"

#include "../includes/Errors.hpp"
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

/**
 *	Ajoute une erreur sous condition
 */
bool				Settings::addErrorIf(cBool condition, cString message, vString errors)
{
	if (condition)
	{
		errors.push_back(message);
		
		return (true);
	}

	return (false);
}

/**
 *	Verifie la logique des settings
 */
void				Settings::checkLogicalTournament(vString errors) const
{
	cInt totalTeamsRequired = this->_nbPools * this->_nbPlayerByPool;
	cInt playersPerTeam = this->_isDouble ? 2 : 1;
	cInt totalPlayersRequired = totalTeamsRequired * playersPerTeam;

	if (this->_nbPlayers > totalPlayersRequired)
		addErrorIf(true, std::format("Nombre de joueurs trop eleve ({}) pour la structure actuelle ({} poules de {} {} = {} joueurs max).",
				this->_nbPlayers, this->_nbPools, this->_nbPlayerByPool, (this->_isDouble ? "equipes" : "joueurs"),
				totalPlayersRequired), errors);
	else if (this->_nbPlayers < totalPlayersRequired && !this->_allowMultiTeamPlayers)
		addErrorIf(true, std::format("Pas assez de joueurs ({}). La structure necessite exactement {} joueurs (ou activez le mode multi-equipes).",
				this->_nbPlayers, totalPlayersRequired), errors);
	else if (this->_nbPlayers < totalPlayersRequired && this->_allowMultiTeamPlayers)
	{
		cInt missingPlayers = totalPlayersRequired - this->_nbPlayers;

		if (missingPlayers > NBPLAYERINMULTITEAMMAX)
			addErrorIf(true, std::format("Ecart de joueurs trop important ({} manquants). Le mode multi-equipes tolere au maximum {} joueurs manquants.",
					missingPlayers, NBPLAYERINMULTITEAMMAX), errors);
	}
}

/****************************************************************************************************/
/*	PUBLIC METHODS																					*/
/****************************************************************************************************/

/**
 *	Verifie si les settings sont valids
 */
bool				Settings::isValid(vString errors)
{
	errors.clear();

	cvInt allowedPlayers = this->_isDouble
		? vInt(allowedNbPlayersDouble.begin(), allowedNbPlayersDouble.end())
		: vInt(allowedNbPlayersSimple.begin(), allowedNbPlayersSimple.end());

	addErrorIf(this->_name.empty(), E_NAME, errors);
	addErrorIf(!isInList(this->_nbPlayers, allowedPlayers), E_NBPLAYER, errors);
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

	checkLogicalTournament(errors);

	if (errors.empty())
		this->_isValid = true;

	return (errors.empty());
}

/**
 *	Verifie le nombre de participants qui pourront jouer dasn plusieurs equipes
 */
bool				Settings::canAccommodate(cInt actualParticipants) const
{
	if ((this->getNbPlayers() - actualParticipants) > NBPLAYERINMULTITEAMMAX)
		return (false);
	
	return (true);
}