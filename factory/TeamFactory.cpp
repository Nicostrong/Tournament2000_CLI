//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <random>
#include <iostream>
#include <algorithm>

#include "../includes/class/Team.hpp"
#include "../includes/class/Pool.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"
#include "../includes/class/Participant.hpp"

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

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/**
 * Retourne true si deux equipes partagent au moins un Participant*.
 */
bool				TeamFactory::teamsShareMember(cpTeam a, cpTeam b)
{
	for (cpPart pa : a->getMembers())
		for (cpPart pb : b->getMembers())
			if (pa == pb)
				return (true);

	return (false);
}

/**
 *	creation des equipes d un player (tournois simple non mixte)
 */
void				TeamFactory::createTeamsUniplayer()
{
	int missing = 0;

	if (!checkMissingPlayers(missing))
		return;

	cInt required = this->_settings->getNbPlayers();
	cInt actual = static_cast<int>(this->_participants.size());

	std::random_device rd;
	std::mt19937 g(rd());

	vpPart shuffled = this->_participants;

	std::ranges::shuffle(shuffled.begin(), shuffled.end(), g);

	for (int i = 0; i < missing; ++i)
	{
		Participant* p = shuffled[static_cast<size_t>(i)];

		p->setIsMultiTeamPlayer(true);

		Team* clone = new Team();

		clone->addMember(p);
		clone->setHasMultiTeamPlayer(true);
		clone->renameTeam();

		this->_teams.push_back(clone);
	}

	cInt normalLimit = std::min(actual, required - missing);

	for (int i = 0; i < normalLimit; ++i)
	{
		Team* t = new Team();

		t->addMember(shuffled[static_cast<size_t>(i)]);
		t->renameTeam();

		this->_teams.push_back(t);
	}

	if (static_cast<int>(this->_teams.size()) != required)
		PrintUtils::addError(std::format("createTeamsUniplayer : {} equipes creees pour {} attendues.", this->_teams.size(), required ));
}

/**
 *	creations des equipes en double (tournois double non mixte)
 */
void				TeamFactory::createDoubleTeams()
{
	vpPart pool = this->_participants;
	cvpPart multiTeams = getMultiTeamsPlayers(pool);

	std::random_device rd;
	std::mt19937 g(rd());

	std::ranges::shuffle(pool.begin(), pool.end(), g);
	
	size_t pIdx = 0;
	size_t mIdx = 0;

	while (pIdx < pool.size() || mIdx < multiTeams.size())
	{
		Team* t = new Team();
		
		if (pIdx < pool.size())
			t->addMember(pool[pIdx++]);
		else if (mIdx < multiTeams.size())
		{
			t->addMember(multiTeams[mIdx++]);
			t->setHasMultiTeamPlayer(true);
		}

		if (pIdx < pool.size())
			t->addMember(pool[pIdx++]);
		else if (mIdx < multiTeams.size())
		{
			t->addMember(multiTeams[mIdx++]);
			t->setHasMultiTeamPlayer(true);
		}
		
		t->renameTeam();

		if (t->getMembers().size() == 2)
			this->_teams.push_back(t);
		else
			delete t;
	}
}

/**
 *	creation des equipes mixtes (tournois double/mixte)
 */
void				TeamFactory::createMixedTeams()
{
	TeamCreationCtx ctx;

	if (!checkMissingPlayers(ctx.missing))
		return;

	vpPart males = getAllMales();
	vpPart females = getAllFemales();
	vpPart minoritaryPool = (females.size() < males.size()) ? females : males;
	vpPart majoritaryPool = (females.size() < males.size()) ? males : females;
	vpPart missingPool;

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

	createStandardMixedTeams(ctx);
	createMissingMixedTeams(ctx);
	createUnigenreTeams(ctx);
}

/**
 *	generation de la pool des participants qui seront dans deux teams
 */
void				TeamFactory::generateMissingPool(const TeamCreationCtx& ctx)
{
	if (ctx.missing <= 0)
		return;

	if (ctx.minoritary->size() == ctx.majoritary->size())
		cloneForEqualGenders(ctx);
	else
		cloneForUnequalGenders(ctx);
}

/**
 *	creation d une pool de multiteamplayers en cas de parite de genre
 */
void				TeamFactory::cloneForEqualGenders(const TeamCreationCtx& ctx)
{
	std::random_device rd;
	std::mt19937 g(rd());
	
	vpPart sampleA = *(ctx.males);
	vpPart sampleB = *(ctx.females);

	std::ranges::shuffle(sampleA.begin(), sampleA.end(), g);
	std::ranges::shuffle(sampleB.begin(), sampleB.end(), g);

	for (int i = 0; i < ctx.missing; ++i)
	{
		if (i % 2 == 0)
		{
			Participant* p = sampleA[i % sampleA.size()];

			p->setIsMultiTeamPlayer(true);
			ctx.missingPool->push_back(p);
		}
		else
		{
			Participant* p = sampleB[i % sampleB.size()];

			p->setIsMultiTeamPlayer(true);
			ctx.missingPool->push_back(p);
		}
	}
}

/**
 *	creation d une pool de multiteamplayers en utilisant la pool minoritaire de genre
 */
void				TeamFactory::cloneForUnequalGenders(const TeamCreationCtx& ctx)
{
	std::random_device rd;
	std::mt19937 g(rd());

	vpPart candidates = *(ctx.minoritary);

	std::ranges::shuffle(candidates.begin(), candidates.end(), g);

	for (int i = 0; i < ctx.missing; ++i)
	{
		Participant* p = candidates[i % candidates.size()];

		p->setIsMultiTeamPlayer(true);
		ctx.missingPool->push_back(p);
	}
}

/**
 *	creation des equipes mixte
 */
void				TeamFactory::createStandardMixedTeams(TeamCreationCtx& ctx)
{
	while (ctx.minIdx < ctx.minoritary->size() && ctx.majIdx < ctx.majoritary->size())
	{
		Team* t = new Team();

		t->addMember((*ctx.minoritary)[ctx.minIdx++]);
		t->addMember((*ctx.majoritary)[ctx.majIdx++]);
		
		if (t->getMembers()[0]->getIsMultiTeamPlayer() || t->getMembers()[1]->getIsMultiTeamPlayer())
			t->setHasMultiTeamPlayer(true);

		t->setIsMixed(true);
		t->renameTeam();

		this->_teams.push_back(t);
	}
}

/**
 *	creation des equipes mixte en utilisant la pool de multiteamsplayer
 */
void				TeamFactory::createMissingMixedTeams(TeamCreationCtx& ctx)
{
	while (ctx.missIdx < ctx.missingPool->size() && ctx.majIdx < ctx.majoritary->size())
	{
		Team* t = new Team();

		t->addMember((*ctx.missingPool)[ctx.missIdx++]);
		t->addMember((*ctx.majoritary)[ctx.majIdx++]);
		t->setHasMultiTeamPlayer(true);
		
		t->renameTeam();

		this->_teams.push_back(t);
	}
}

/**
 *	creation des equipes unigenre
 */
void				TeamFactory::createUnigenreTeams(TeamCreationCtx& ctx)
{
	vpPart leftovers;

	while (ctx.majIdx < ctx.majoritary->size())
		leftovers.push_back((*ctx.majoritary)[ctx.majIdx++]);

	while (ctx.minIdx < ctx.minoritary->size())
		leftovers.push_back((*ctx.minoritary)[ctx.minIdx++]);

	while (ctx.missIdx < ctx.missingPool->size())
		leftovers.push_back((*ctx.missingPool)[ctx.missIdx++]);

	for (size_t i = 0; i + 1 < leftovers.size(); i += 2)
	{
		Team* t = new Team();

		t->addMember(leftovers[i]);
		t->addMember(leftovers[i + 1]);
		
		if (leftovers[i]->getIsMultiTeamPlayer() || leftovers[i + 1]->getIsMultiTeamPlayer())
			t->setHasMultiTeamPlayer(true);
		
		t->renameTeam();

		this->_teams.push_back(t);
	}
}

/**
 *	verifie le nombre de participants qui devront etre dans deux teams
 */
bool				TeamFactory::checkMissingPlayers(int& missing) const
{
	cInt actual = static_cast<int>(this->_participants.size());
	cInt required = this->_settings->getNbPlayers();

	missing = std::max(0, required - actual);

	if (missing > NBPLAYERINMULTITEAMMAX)
	{
		PrintUtils::addError(std::format("Trop de joueurs manquants ({}). Limite de reutilisation autorisee : {}.", missing , NBPLAYERINMULTITEAMMAX));
		return (false);
	}

	return (true);
}

/**
 *	retourne un vecteur avec tout les participants Homme
 */
vpPart				TeamFactory::getAllMales() const
{
	vpPart males;

	for (Participant* p : this->_participants)
		if (p->getGenderInt() == Gender::MALE)
			males.push_back(p);

	return (males);
}

/**
 *	retourne un vecteur avec tout les participants Femme
 */
vpPart				TeamFactory::getAllFemales() const
{
	vpPart females;

	for (Participant* p : this->_participants)
		if (p->getGenderInt() == Gender::FEMALE)
			females.push_back(p);

	return (females);
}

/**
 *	retourne un vecteur avec tout les participants qui sont dans plusieurs teams dans une pool donnee
 */
vpPart				TeamFactory::getMultiTeamsPlayers(vpPart participants) const
{
	vpPart result;

	cInt actual = static_cast<int>(this->_participants.size());
	cInt required = this->_settings->getNbPlayers();
	cInt missing = std::max(0, required - actual);
	cInt maxRecyclable = std::min(missing, NBPLAYERINMULTITEAMMAX);

	if (maxRecyclable <= 0 || participants.empty())
		return (result);

	std::random_device rd;
	std::mt19937 g(rd());

	std::ranges::shuffle(participants.begin(), participants.end(), g);

	for (int i = 0; i < maxRecyclable && i < static_cast<int>(participants.size()); ++i)
	{
		participants[i]->setIsMultiTeamPlayer(true);
		result.push_back(participants[i]);
	}

	return (result);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

vpTeam				TeamFactory::createTeams(const vpPart& participants,cpSet settings)
{

}