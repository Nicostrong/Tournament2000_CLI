//
// Created by Nicolas Fordoxcel on 28/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <cstddef>
# include <memory>

#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Player.hpp"

#include "../includes/Constantes.hpp"

#include "../includes/manager/PoolManager.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cInt			=	const int;

using				pPlayer			=	Player*;
using				cpPlayer		=	const Player*;
using				cvpPlayer		=	const std::vector<Player*>&;

using				pMatch			=	Match*;

using				uPool			=	std::unique_ptr<Pool>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

PoolManager::PoolManager(cSet settings) : _settings(settings) {}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

vpPool				PoolManager::getPools() const
{
	vpPool pools;

	pools.reserve(this->_pools.size());

	for (const uPool& p: this->_pools)
		if (p)
			pools.push_back(p.get());

	return (pools);
}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

void				PoolManager::createEmptyPools(cInt nbPools)
{
	if (nbPools <= 0)
		return;

	this->_pools.reserve(this->_pools.size() + static_cast<size_t>(nbPools));

	for (int i = 0; i < nbPools; ++i)
		this->_pools.push_back(std::make_unique<Pool>());
}

void				PoolManager::distributeTeamsToPools(cvpTeam teams, cInt nbPools)
{
	if (nbPools <= 0)
		return;

	if (static_cast<size_t>(nbPools) > this->_pools.size())
		return;
		
	vpTeam mixedTeams;
	vpTeam otherTeams;

	for (pTeam t : teams)
	{
		if (!t)
			continue;

		cvpPlayer members = t->getMembers();

		bool hasMale = false;
		bool hasFemale = false;

		if (members.size() == 2)
		{
			for (cpPlayer p : members)
			{
				if (!p)
					continue;

				if (p->getGenderInt() == Gender::MALE)
					hasMale = true;

				if (p->getGenderInt() == Gender::FEMALE)
					hasFemale = true;
			}
		}

		if (hasMale && hasFemale)
			mixedTeams.push_back(t);
		else
			otherTeams.push_back(t);
	}

	int poolIdx = 0;

	for (pTeam t : mixedTeams)
		this->_pools[static_cast<size_t>(poolIdx++ % nbPools)]->addTeam(t);

	for (pTeam t : otherTeams)
		this->_pools[static_cast<size_t>(poolIdx++ % nbPools)]->addTeam(t);
}

void				PoolManager::resolvePoolConflicts(int nbPools)
{
	if (nbPools <= 1)
		return;

	if (static_cast<size_t>(nbPools) > this->_pools.size())
		return;

	for (int i = 0; i < nbPools; ++i)
	{
		pPool poolI = this->_pools[static_cast<size_t>(i)].get();

		if (!poolI)
			continue;

		vpTeam& teamsI = poolI->getTeamsMutable();

		for (size_t a = 0; a < teamsI.size(); ++a)
		{
			for (size_t b = a + 1; b < teamsI.size(); ++b)
			{
				if (!teamsShareMember(teamsI[a], teamsI[b]))
					continue;

				bool swapped = false;

				for (int j = 0; j < nbPools && !swapped; ++j)
				{
					if (j == i)
						continue;

					pPool poolJ = this->_pools[static_cast<size_t>(i)].get();

					if (!poolJ)
						continue;

					vpTeam& teamsJ = poolJ->getTeamsMutable();

					for (size_t c = 0; c < teamsJ.size() && !swapped; ++c)
					{
						pTeam candidate = teamsJ[c];
						bool candidateHasConflit = false;
						bool otherCandidateHasConflit = false;

						for (size_t k = 0; k < teamsI.size(); ++k)
							if (k != b && teamsShareMember(teamsI[k], candidate))
							{
								candidateHasConflit = true;
								break;
							}

						for (size_t k = 0; k < teamsJ.size(); ++k)
							if (k != c && teamsShareMember(teamsJ[k], teamsI[b]))
							{
								otherCandidateHasConflit = true;
								break;
							}

						if (!candidateHasConflit && !otherCandidateHasConflit)
						{
							std::swap(teamsI[b], teamsJ[c]);
							swapped = true;
						}
					}
				}
			}
		}
	}
}

void				PoolManager::generatePoolMatches() const
{
	cInt nbSetPlayed = this->_settings.getNbSetPlayedPools();

	for (const uPool& p : this->_pools)
	{
		if (!p)
			continue;

		p->generateMatches(nbSetPlayed, this->_settings);
	}
}

bool				PoolManager::teamsShareMember(pTeam a, pTeam b)
{
	if (!a || !b)
		return (false);

	for (pPlayer pA : a->getMembers())
	{
		if (!pA)
			continue;

		for (pPlayer pB : b->getMembers())
			if (pB && pA == pB)
				return (true);
	}

	return (false);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

void				PoolManager::clean()
{
	this->_pools.clear();
}

void				PoolManager::generatePools(cvpTeam teams)
{
	cInt nbPools = this->_settings.getNbPools();

	if (!this->_pools.empty() || nbPools <= 0)
		return;

	createEmptyPools(nbPools);
	distributeTeamsToPools(teams, nbPools);
	resolvePoolConflicts(nbPools);
	generatePoolMatches();
}

void				PoolManager::sortAllPools() const
{
	for (const uPool& p : _pools)
		if (p)
			p->sortTeams();
}

bool				PoolManager::isPoolsFinished() const
{
	if (this->_pools.empty())
		return (false);

	for (const uPool& p : this->_pools)
	{
		if (!p)
			continue;

		for (pMatch m : p->getMatches())
			if (!m || !m->isFinished())
				return (false);
	}
	return (true);
}

void				PoolManager::applyPoolDisqualification(pTeam team)
{
	if (!team)
		return;

	for (const uPool& p : this->_pools)
	{
		if (!p)
			continue;

		bool found = false;

		for (pMatch match : p->getMatches())
		{
			if (!match || (match->getTeamA() != team && match->getTeamB() != team))
				continue;

			if (match->getTeamA() == team && match->isFinished())
				match->modifyScore(0, SCOREMAXTOWIN);
			else if (match->getTeamB() == team && match->isFinished())
				match->modifyScore(SCOREMAXTOWIN, 0);
			else if (match->getTeamA() == team)
				match->setScore(0, SCOREMAXTOWIN);
			else
				match->setScore(SCOREMAXTOWIN, 0);
			
			match->setIsFinished(true);
			found = true;
		}

		if (found)
		{
			team->disqualifyTeam(true);
			p->checkPoolIsFinished();
		}
	}
}

void				PoolManager::unDisqualifyTeam(pTeam team)
{
	if (!team)
		return;

	for (const uPool& pool : this->_pools)
	{
		if (!pool)
			continue;

		bool found = false;

		for (pMatch match : pool->getMatches())
		{
			if (!match || (match->getTeamA() != team && match->getTeamB() != team))
				continue;

			match->modifyScore(0, 0);
			match->setIsFinished(false);
			found = true;
		}

		if (found)
			team->disqualifyTeam(false);
	}
}