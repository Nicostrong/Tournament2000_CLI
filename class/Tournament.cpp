//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include "../includes/class/Team.hpp"
#include "../includes/class/Pool.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Player.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"

#include "../includes/manager/PlayerManager.hpp"


/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cInt			=	const int;

using				cvpPool			=	const std::vector<Pool*>&;

using				pTeam			=	Team*;
using				vpTeam			=	std::vector<Team*>;

using				pPhase			=	Phase*;

using				cSet			=	const Settings&;

using				vpPlayer		=	std::vector<Player*>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Tournament::Tournament(cSet settings, PlayerManager& playerManager)
	: _settings(settings), _playerManager(playerManager), _teamManager(settings), _poolManager(settings),
	_phaseManager(settings), _isReady(false), _isFinished(false) {}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

vpPlayer			Tournament::getPlayers() const			{	return (this->_playerManager.getPlayers());			}
vpTeam				Tournament::getTeams() const			{	return (this->_teamManager.getTeams());					}
pTeam				Tournament::getTeamById(cInt id) const	{	return (this->_teamManager.getTeamById(id));			}
cSet				Tournament::getSettings() const			{	return (this->_settings);								}
vpPool				Tournament::getPools() const			{	return (this->_poolManager.getPools());					}
pPhase				Tournament::getSixteenth() const		{	return (this->_phaseManager.getSixteenth());			}
pPhase				Tournament::getEighth() const			{	return (this->_phaseManager.getEighth());				}
pPhase				Tournament::getQuarters() const			{	return (this->_phaseManager.getQuarters());				}
pPhase				Tournament::getSemis() const			{	return (this->_phaseManager.getSemis());				}
pPhase				Tournament::getFinal() const			{	return (this->_phaseManager.getFinal());				}
pPhase				Tournament::getThirdPlace() const		{	return (this->_phaseManager.getThirdPlace());			}
bool				Tournament::getHasSixteenth() const		{	return (this->_phaseManager.getHasSixteenth());			}
bool				Tournament::getHasEighth() const		{	return (this->_phaseManager.getHasEighth());			}
bool				Tournament::getHasThirdMatch() const	{	return (this->_phaseManager.getHasThirdMatch());		}
bool				Tournament::getIsReady() const			{	return (this->_isReady);								}
bool				Tournament::getIsFinished() const		{	return (this->_isFinished);								}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				Tournament::setHasSixteenth(cBool value)	{	this->_phaseManager.setHasSixteenth(value);		}
void				Tournament::setHasEighth(cBool value)		{	this->_phaseManager.setHasEighth(value);		}
void				Tournament::setHasThirdMatch(cBool value)	{	this->_phaseManager.setHasThirdMatch(value);	}
void				Tournament::setIsReady(cBool value)			{	this->_isReady = value;							}
void				Tournament::setIsFinished(cBool value)		{	this->_isFinished = value;						}

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				Tournament::clean()
{
	this->_teamManager.clean();
	this->_poolManager.clean();
	this->_phaseManager.clean();

	this->_isReady = false;
	this->_isFinished = false;
}

void				Tournament::generateTeams()
{
	this->_teamManager.generateTeams(this->_playerManager.getPlayers());
}

void				Tournament::generatePools()
{
	this->_poolManager.generatePools(this->_teamManager.getTeams());
}

void				Tournament::generateSixteenths()
{
	if (!this->_poolManager.isPoolsFinished())
		return;

	this->_phaseManager.generateSixteenths(this->_poolManager.getPools());
}

void				Tournament::generateEighths()
{
	if (!this->_poolManager.isPoolsFinished())
		return;

	this->_phaseManager.generateEighths(this->_poolManager.getPools());
}

void				Tournament::generateQuarters()
{
	if (!this->_poolManager.isPoolsFinished())
		return;

	this->_phaseManager.generateQuarters(this->_poolManager.getPools());
}

void				Tournament::generateSemis()
{
	this->_phaseManager.generateSemis();
}

void				Tournament::generateFinal()
{
	this->_phaseManager.generateFinal();
}

void				Tournament::generateThirdPlace()
{
	this->_phaseManager.generateThirdPlace();
}

void				Tournament::disqualifyTeam(pTeam team)
{
	if (!team)
		return;

	if (!this->_poolManager.isPoolsFinished())
		this->_poolManager.applyPoolDisqualification(team);
	else
		this->_phaseManager.applyBracketDisqualification(team, this->_poolManager.getPools());
}

void				Tournament::unDisqualifyTeam(pTeam team)
{
	if (!team || !team->getIsDisqualified())
		return;

	if (!this->_poolManager.isPoolsFinished())
		this->_poolManager.unDisqualifyTeam(team);
	else
		this->_phaseManager.unDisqualifyTeam(team);

	team->disqualifyTeam(false);
}

bool				Tournament::initializeTournament()
{
	cInt required = this->_settings.getNbPlayers();
	cInt actual = static_cast<int>(this->_playerManager.getSize());

	if (actual < required && !this->_settings.getAllowMultiTeamPlayers())
		return (false);

	if (actual == 0)
		return (false);

	generateTeams();
	generatePools();

	this->_isReady = true;

	return (true);
}

bool				Tournament::isPoolsFinished() const
{
	return (this->_poolManager.isPoolsFinished());
}

bool				Tournament::isSixteenthUnlocked() const
{
	return (this->_phaseManager.isSixteenthUnlocked(this->_poolManager.isPoolsFinished()));
}

bool				Tournament::isEighthUnlocked() const
{
	return (this->_phaseManager.isEighthUnlocked(this->_poolManager.isPoolsFinished()));
}

bool				Tournament::isQuartersUnlocked() const
{
	return (this->_phaseManager.isQuartersUnlocked(this->_poolManager.isPoolsFinished()));
}

bool				Tournament::isSemisUnlocked() const
{
	return (this->_phaseManager.isSemisUnlocked());
}

bool				Tournament::isFinalUnlocked() const
{
	return (this->_phaseManager.isFinalUnlocked());
}

bool				Tournament::isThirdUnlocked() const
{
	return (this->_phaseManager.isThirdUnlocked());
}

