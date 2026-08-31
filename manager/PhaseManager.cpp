//
// Created by Nicolas Fordoxcel on 28/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Settings.hpp"

#include "../includes/manager/PhaseManager.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cInt			=	const int;

using				cBool			=	const bool;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

PhaseManager::PhaseManager(cSet settings) :
	_settings(settings), _hasSixteenth(settings.getNbPools() == 16),
	_hasEighth(settings.getNbPools() >= 8), _hasThirdMatch(settings.getIsThirdPlaceMatch()) {}

PhaseManager::~PhaseManager() = default;

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

pPhase				PhaseManager::getSixteenth() const			{	return (this->_sixteenths.get());		}
pPhase				PhaseManager::getEighth() const				{	return (this->_eighth.get());			}
pPhase				PhaseManager::getQuarters() const			{	return (this->_quarters.get());			}
pPhase				PhaseManager::getSemis() const				{	return (this->_semis.get());			}
pPhase				PhaseManager::getFinal() const				{	return (this->_final.get());			}
pPhase				PhaseManager::getThirdPlace() const			{	return (this->_thirdPlace.get());		}
bool				PhaseManager::getHasSixteenth() const		{	return (this->_hasSixteenth);			}
bool				PhaseManager::getHasEighth() const			{	return (this->_hasEighth);				}
bool				PhaseManager::getHasThirdMatch() const		{	return (this->_hasThirdMatch);			}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				PhaseManager::setHasSixteenth(bool value)	{	this->_hasSixteenth = value;	}
void				PhaseManager::setHasEighth(bool value)		{	this->_hasEighth = value;		}
void				PhaseManager::setHasThirdMatch(bool value)	{	this->_hasThirdMatch = value;	}

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

void				PhaseManager::generateSymmetricPoolEncounters(pPhase targetPhase, cvpPool pools, size_t nbPools) const
{
	if (!targetPhase || pools.size() < nbPools)
		return;

	for (size_t i = 0; i < nbPools / 2; ++i)
	{
		pPool firstPool = pools[i];
		pPool oppositePool = pools[nbPools - 1 - i];

		if (!firstPool || !oppositePool)
			continue;

		vpTeam firstQualifiers = firstPool->getQualifiers();
		vpTeam oppositeQualifiers = oppositePool->getQualifiers();

		if (firstQualifiers.size() < 2 || oppositeQualifiers.size() < 2)
			continue;

		targetPhase->addEncounter(firstQualifiers[0], oppositeQualifiers[1], this->_settings);
		targetPhase->addEncounter(oppositeQualifiers[0], firstQualifiers[1], this->_settings);
	}
}

bool				PhaseManager::addEncountersFromPreviousPhase(pPhase currentPhase, cpPhase previousPhase)
{
	if (!currentPhase || !previousPhase || !previousPhase->isFinished())
		return (false);

	vpTeam winners = previousPhase->getWinners();

	if (winners.size() < 2)
		return (false);

	for (size_t i = 0; i + 1 < winners.size(); i += 2)
		currentPhase->addEncounter(winners[i], winners[i + 1], this->_settings);

	return (!currentPhase->getMatches().empty());
}

pMatch				PhaseManager::findCurrentActiveMatch(pTeam team) const
{
	if (!team)
		return (nullptr);

	std::vector<cpPhase> bracketPhases
	{
	this->_final.get(),
	this->_thirdPlace.get(),
	this->_semis.get(),
	this->_quarters.get(),
	this->_eighth.get(),
	this->_sixteenths.get()
	};

	for (cpPhase phase : bracketPhases)
	{
		if (!phase)
			continue;

		for (pMatch match : phase->getMatches())
			if (match && !match->isFinished() && (match->getTeamA() == team || match->getTeamB() == team))
				return (match);
	}

	return (nullptr);
}

pMatch				PhaseManager::findPreviousMatch(pTeam team) const
{
	if (!team)
		return (nullptr);

	std::vector<cpPhase> reversePhases
	{
	this->_final.get(),
	this->_thirdPlace.get(),
	this->_semis.get(),
	this->_quarters.get(),
	this->_eighth.get(),
	this->_sixteenths.get()
	};

	for (cpPhase phase : reversePhases)
	{
		if (!phase)
			continue;

		for (pMatch match : phase->getMatches())
			if (match && match->isFinished() && (match->getTeamA() == team || match->getTeamB() == team))
			return (match);
	}

	return (nullptr);
}

pTeam				PhaseManager::getNextBestTeamFromPool(pTeam disqualifiedTeam, cvpPool pools) const
{
	if (!disqualifiedTeam)
		return (nullptr);

	pPool targetPool = nullptr;

	for (pPool pool : pools)
	{
		if (pool && pool->containsTeam(disqualifiedTeam))
		{
			targetPool = pool;
			break;
		}
	}

	if (!targetPool)
		return (nullptr);

	targetPool->sortTeams();

	vpTeam teams = targetPool->getTeams();

	if (teams.size() <= 2)
		return (nullptr);

	pTeam repechedTeam = teams[2];

	if (!repechedTeam)
		return (nullptr);

	repechedTeam->setIsEliminated(false);

	return (repechedTeam);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				PhaseManager::clean()
{
	this->_sixteenths.reset();
	this->_eighth.reset();
	this->_quarters.reset();
	this->_semis.reset();
	this->_final.reset();
	this->_thirdPlace.reset();

	this->_repechageHistory.clear();
}

void				PhaseManager::generateSixteenths(cvpPool pools)
{
	if (this->_sixteenths || !this->_hasSixteenth || pools.size() < 16)
		return;

	auto phase = std::make_unique<Phase>("1/16 de Finale", this->_settings.getNbSetPlayedSixteenth());

	generateSymmetricPoolEncounters(phase.get(), pools, 16);

	if (!phase->getMatches().empty())
		this->_sixteenths = std::move(phase);
}

void				PhaseManager::generateEighths(cvpPool pools)
{
	if (this->_eighth || !this->_hasEighth)
		return;

	auto phase = std::make_unique<Phase>("1/8 de Finale", this->_settings.getNbSetPlayedHeigth());

	if (this->_hasSixteenth)
	{
		if (!addEncountersFromPreviousPhase(phase.get(), this->_sixteenths.get()))
			return;
	}
	else
	{
		if (pools.size() < 8)
			return;

		generateSymmetricPoolEncounters(phase.get(), pools,8);
	}

	if (!phase->getMatches().empty())
		this->_eighth = std::move(phase);
}

void				PhaseManager::generateQuarters(cvpPool pools)
{
	if (this->_quarters)
		return;

	auto phase = std::make_unique<Phase>("Quarts de Finale", this->_settings.getNbSetPlayedQuarters());

	if (this->_hasEighth)
	{
		if (!addEncountersFromPreviousPhase(phase.get(), this->_eighth.get()))
			return;
	}
	else
	{
		if (pools.size() < 4)
			return;

		for (size_t i = 0; i < 2; ++i)
		{
			pPool firstPool = pools[i];
			pPool oppositePool = pools[i + 2];

			if (!firstPool || !oppositePool)
				continue;

			vpTeam firstQualifiers = firstPool->getQualifiers();
			vpTeam oppositeQualifiers = oppositePool->getQualifiers();

			if (firstQualifiers.size() < 2 || oppositeQualifiers.size() < 2)
				continue;

			phase->addEncounter(firstQualifiers[0], oppositeQualifiers[1], this->_settings);

			phase->addEncounter(oppositeQualifiers[0], firstQualifiers[1], this->_settings);
		}
	}

	if (!phase->getMatches().empty())
		this->_quarters = std::move(phase);
}

void				PhaseManager::generateSemis()
{
	if (this->_semis || !this->_quarters || !this->_quarters->isFinished())
		return;

	vpTeam winners = this->_quarters->getWinners();

	if (winners.size() < 4)
		return;

	auto phase = std::make_unique<Phase>("Demi-Finales", this->_settings.getNbSetPlayedSemis());

	phase->addEncounter(winners[0], winners[2], this->_settings);
	phase->addEncounter(winners[1], winners[3], this->_settings);

	this->_semis = std::move(phase);
}

void				PhaseManager::generateFinal()
{
	if (this->_final || !this->_semis || !this->_semis->isFinished())
		return;

	vpTeam winners = this->_semis->getWinners();

	if (winners.size() < 2)
		return;

	auto phase = std::make_unique<Phase>("Finale", this->_settings.getNbSetPlayedFinal());

	phase->addEncounter(winners[0], winners[1], this->_settings);

	this->_final = std::move(phase);
}

void				PhaseManager::generateThirdPlace()
{
	if (this->_thirdPlace || !this->_semis || !this->_semis->isFinished() || !this->_hasThirdMatch)
		return;

	vpTeam losers = this->_semis->getLosers();

	if (losers.size() < 2)
		return;

	auto phase = std::make_unique<Phase>("Petite Finale", this->_settings.getNbSetPlayedThirdPlace());

	phase->addEncounter(losers[0], losers[1], this->_settings);

	this->_thirdPlace = std::move(phase);
}

void				PhaseManager::applyBracketDisqualification(pTeam team, cvpPool pools)
{
	if (!team)
		return;

	pMatch currentMatch =findCurrentActiveMatch(team	);

	if (!currentMatch)
		return;

	pTeam replacementTeam = nullptr;
	pMatch previousMatch = findPreviousMatch(team);

	if (previousMatch)
		replacementTeam = (previousMatch->getTeamA() == team)
			? previousMatch->getTeamB()
			: previousMatch->getTeamA();
	else
		replacementTeam = getNextBestTeamFromPool(team, pools);

	if (!replacementTeam)
		return;

	this->_repechageHistory[team] = replacementTeam;

	if (currentMatch->getTeamA() == team)
		currentMatch->setTeamA(replacementTeam);
	else
		currentMatch->setTeamB(replacementTeam);

	team->disqualifyTeam(true);
}

void				PhaseManager::unDisqualifyTeam(pTeam team)
{
	if (!team)
		return;

	auto it = this->_repechageHistory.find(team);

	if (it == this->_repechageHistory.end())
		return;

	pTeam replacementTeam = it->second;
	pMatch currentMatch = findCurrentActiveMatch(replacementTeam);

	if (currentMatch)
	{
		if (currentMatch->getTeamA() == replacementTeam)
			currentMatch->setTeamA(team);
		else if (currentMatch->getTeamB() == replacementTeam)
			currentMatch->setTeamB(team);
	}

	team->disqualifyTeam(false);

	this->_repechageHistory.erase(it);
}


/****************************************************************************************************/
/*	UNLOCKS																							*/
/****************************************************************************************************/


bool				PhaseManager::isSixteenthUnlocked(bool poolsFinished) const
{
	return (this->_hasSixteenth && poolsFinished);
}

bool				PhaseManager::isEighthUnlocked(bool poolsFinished) const
{
	if (!this->_hasEighth)
		return (false);

	if (this->_hasSixteenth)
		return (this->_sixteenths && this->_sixteenths->isFinished());

	return (poolsFinished);
}

bool				PhaseManager::isQuartersUnlocked(bool poolsFinished) const
{
	if (this->_hasEighth)
		return (this->_eighth && this->_eighth->isFinished());

	return (poolsFinished);
}

bool				PhaseManager::isSemisUnlocked() const
{
	return (this->_quarters && this->_quarters->isFinished());
}

bool				PhaseManager::isFinalUnlocked() const
{
	return (this->_semis && this->_semis->isFinished());
}

bool				PhaseManager::isThirdUnlocked() const
{
	if (!this->_hasThirdMatch)
		return (false);

	return (this->_semis && this->_semis->isFinished());
}