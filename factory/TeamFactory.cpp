//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <random>
#include <memory>
#include <algorithm>

#include "../includes/class/Team.hpp"
#include "../includes/class/Player.hpp"
#include "../includes/class/Settings.hpp"

#include "../includes/utils/PrintUtils.hpp"

#include "../includes/factory/TeamFactory.hpp"

#include "../includes/Constantes.hpp"

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

TeamFactory::TeamFactory(cSet settings) : _settings(settings) {}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

vpPlayer			TeamFactory::getAllMales(cvpPlayer players) const
{
	vpPlayer males;

	for (pPlayer p : players)
		if (p && p->getGenderInt() == Gender::MALE)
			males.push_back(p);

	return (males);
}

vpPlayer			TeamFactory::getAllFemales(cvpPlayer players) const
{
	vpPlayer females;

	for (pPlayer p : players)
		if (p && p->getGenderInt() == Gender::FEMALE)
			females.push_back(p);

	return (females);
}

vpPlayer			TeamFactory::getMultiTeamsPlayers(vpPlayer players) const
{
	vpPlayer result;
	cInt actual = static_cast<int>(players.size());
	cInt required = this->_settings.getNbPlayers();
	cInt missing = std::max(0, required - actual);
	cInt maxRecyclable = std::min(missing, NBPLAYERINMULTITEAMMAX);

	if (maxRecyclable <= 0 || players.empty())
		return (result);

	std::random_device rd;
	std::mt19937 g(rd());
	std::ranges::shuffle(players.begin(), players.end(), g);

	for (int i = 0; i < maxRecyclable && i < static_cast<int>(players.size()); ++i)
	{
		players[static_cast<size_t>(i)]->setIsMultiTeamPlayer(true);
		result.push_back(players[static_cast<size_t>(i)]);
	}

	return (result);
}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

bool				TeamFactory::checkMissingPlayers(cvpPlayer players, int& missing) const
{
	cInt actual = static_cast<int>(players.size());
	cInt required = this->_settings.getNbPlayers();
	missing = std::max(0, required - actual);

	return (missing <= NBPLAYERINMULTITEAMMAX);
}

void				TeamFactory::createTeamsUniplayer(cvpPlayer players, vuTeam& teams)
{
	int missing = 0;

	if (!checkMissingPlayers(players, missing))
		return;

	cInt required = this->_settings.getNbPlayers();
	cInt actual = static_cast<int>(players.size());

	std::random_device rd;
	std::mt19937 g(rd());
	vpPlayer shuffled = players;
	std::ranges::shuffle(shuffled.begin(), shuffled.end(), g);

	for (int i = 0; i < missing; ++i)
	{
		pPlayer p = shuffled[static_cast<size_t>(i)];

		if (!p)
			continue;

		p->setIsMultiTeamPlayer(true);

		auto team = std::make_unique<Team>();

		team->addMember(p);
		team->setHasMultiTeamPlayer(true);
		team->renameTeam();

		teams.push_back(std::move(team));
	}

	cInt normalLimit = std::min(actual, required - missing);

	for (int i = 0; i < normalLimit; ++i)
	{
		pPlayer p = shuffled[static_cast<size_t>(i)];

		if (!p)
			continue;

		auto team = std::make_unique<Team>();

		team->addMember(p);
		team->renameTeam();

		teams.push_back(std::move(team));
	}
}

void				TeamFactory::createDoubleTeams(cvpPlayer players, vuTeam& teams)
{
	vpPlayer pool = players;
	vpPlayer multiTeams = getMultiTeamsPlayers(pool);

	std::random_device rd;
	std::mt19937 g(rd());
	std::ranges::shuffle(pool.begin(), pool.end(), g);

	size_t pIdx = 0;
	size_t mIdx = 0;

	while (pIdx < pool.size() || mIdx < multiTeams.size())
	{
		auto team = std::make_unique<Team>();

		if (pIdx < pool.size())
			team->addMember(pool[pIdx++]);
		else if (mIdx < multiTeams.size())
		{
			team->addMember(multiTeams[mIdx++]);
			team->setHasMultiTeamPlayer(true);
		}

		if (pIdx < pool.size())
			team->addMember(pool[pIdx++]);
		else if (mIdx < multiTeams.size())
		{
			team->addMember(multiTeams[mIdx++]);
			team->setHasMultiTeamPlayer(true);
		}
		
		team->renameTeam();

		if (team->getMembers().size() == 2)
			teams.push_back(std::move(team));
	}
}

void				TeamFactory::createMixedTeams(cvpPlayer players, vuTeam& teams)
{
	TeamCreationCtx ctx;

	if (!checkMissingPlayers(players, ctx.missing))
		return;

	vpPlayer males = getAllMales(players);
	vpPlayer females = getAllFemales(players);
	vpPlayer minoritaryPool = (females.size() < males.size()) ? females : males;
	vpPlayer majoritaryPool = (females.size() < males.size()) ? males : females;
	vpPlayer missingPool;

	ctx.males = &males;
	ctx.females = &females;
	ctx.minoritary = &minoritaryPool;
	ctx.majoritary = &majoritaryPool;
	ctx.missingPool = &missingPool;

	generateMissingPool(ctx);

	std::random_device rd;
	std::mt19937 g(rd());
	std::ranges::shuffle(majoritaryPool.begin(), majoritaryPool.end(), g);
	std::ranges::shuffle(minoritaryPool.begin(), minoritaryPool.end(), g);
	std::ranges::shuffle(missingPool.begin(), missingPool.end(), g);

	createStandardMixedTeams(ctx, teams);
	createMissingMixedTeams(ctx, teams);
	createUnigenreTeams(ctx, teams);
}

void				TeamFactory::generateMissingPool(TeamCreationCtx& ctx)
{
	if (ctx.missing <= 0)
		return;

	if (ctx.minoritary->size() == ctx.majoritary->size())
		cloneForEqualGenders(ctx);
	else
		cloneForUnequalGenders(ctx);
}

void				TeamFactory::cloneForEqualGenders(TeamCreationCtx& ctx)
{
	if (!ctx.males || !ctx.females || ctx.males->empty() || ctx.females->empty())
		return;

	std::random_device rd;
	std::mt19937 g(rd());
	vpPlayer sampleA = *(ctx.males);
	vpPlayer sampleB = *(ctx.females);
	std::ranges::shuffle(sampleA.begin(), sampleA.end(), g);
	std::ranges::shuffle(sampleB.begin(), sampleB.end(), g);

	for (int i = 0; i < ctx.missing; ++i)
	{
		if (i % 2 == 0)
		{
			pPlayer p = sampleA[static_cast<size_t>(i) % sampleA.size()];

			p->setIsMultiTeamPlayer(true);
			ctx.missingPool->push_back(p);
		}
		else
		{
			pPlayer p = sampleB[static_cast<size_t>(i) % sampleB.size()];

			p->setIsMultiTeamPlayer(true);
			ctx.missingPool->push_back(p);
		}
	}
}

void				TeamFactory::cloneForUnequalGenders(TeamCreationCtx& ctx)
{
	if (!ctx.minoritary || ctx.minoritary->empty())
		return;

	std::random_device rd;
	std::mt19937 g(rd());
	vpPlayer candidates = *(ctx.minoritary);
	std::ranges::shuffle(candidates.begin(), candidates.end(), g);

	for (int i = 0; i < ctx.missing; ++i)
	{
		pPlayer p = candidates[static_cast<size_t>(i) % candidates.size()];

		p->setIsMultiTeamPlayer(true);
		ctx.missingPool->push_back(p);
	}
}

void				TeamFactory::createStandardMixedTeams(TeamCreationCtx& ctx, vuTeam& teams)
{
	while (ctx.minIdx < ctx.minoritary->size() && ctx.majIdx < ctx.majoritary->size())
	{
		auto t = std::make_unique<Team>();

		t->addMember((*ctx.minoritary)[ctx.minIdx++]);
		t->addMember((*ctx.majoritary)[ctx.majIdx++]);
		
		if (t->getMembers()[0]->getIsMultiTeamPlayer() || t->getMembers()[1]->getIsMultiTeamPlayer())
			t->setHasMultiTeamPlayer(true);

		t->setIsMixed(true);
		t->renameTeam();
		teams.push_back(std::move(t));
	}
}

void				TeamFactory::createMissingMixedTeams(TeamCreationCtx& ctx, vuTeam& teams)
{
	while (ctx.missIdx < ctx.missingPool->size() && ctx.majIdx < ctx.majoritary->size())
	{
		auto t = std::make_unique<Team>();

		t->addMember((*ctx.missingPool)[ctx.missIdx++]);
		t->addMember((*ctx.majoritary)[ctx.majIdx++]);
		t->setHasMultiTeamPlayer(true);
		t->renameTeam();
		teams.push_back(std::move(t));
	}
}

void				TeamFactory::createUnigenreTeams(TeamCreationCtx& ctx, vuTeam& teams)
{
	vpPlayer leftovers;

	while (ctx.majIdx < ctx.majoritary->size())
		leftovers.push_back((*ctx.majoritary)[ctx.majIdx++]);

	while (ctx.minIdx < ctx.minoritary->size())
		leftovers.push_back((*ctx.minoritary)[ctx.minIdx++]);

	while (ctx.missIdx < ctx.missingPool->size())
		leftovers.push_back((*ctx.missingPool)[ctx.missIdx++]);
	
	for (size_t i = 0; i + 1 < leftovers.size(); i += 2)
	{
		auto t = std::make_unique<Team>();

		t->addMember(leftovers[i]);
		t->addMember(leftovers[i + 1]);

		if (leftovers[i]->getIsMultiTeamPlayer() || leftovers[i + 1]->getIsMultiTeamPlayer())
			t->setHasMultiTeamPlayer(true);

		t->renameTeam();
		teams.push_back(std::move(t));
	}
}

vuTeam				TeamFactory::generateTeams(cvpPlayer players)
{
	vuTeam teams;

	if (!this->_settings.getIsDouble())
		createTeamsUniplayer(players, teams);
	else if (this->_settings.getIsMixed())
		createMixedTeams(players, teams);
	else
		createDoubleTeams(players, teams);

	return (teams);
}