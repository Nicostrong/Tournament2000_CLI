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

#include "../includes/class/Tournament.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Participant.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Pool.hpp"
#include "../includes/class/Phase.hpp"

#include "../includes/utils/PrintUtils.hpp"

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

Tournament::Tournament(pSet settings):	_settings(settings), _sixteenths(nullptr), _heighths(nullptr),
	_quarters(nullptr), _semis(nullptr), _final(nullptr), _thirdPlace(nullptr), _isReady(false),
	_isFinished(false), _hasSixteenth(settings->getNbPools() == 16),
	_hasHeighth(settings->getNbPools() >= 8),_hasThirdMatch(settings->getIsThirdPlaceMatch())
{}

Tournament::Tournament(pSet settings, vpPart participants): Tournament(settings)
{
	for (Participant* p : participants)
		this->_participants.push_back(p);

	this->initializeTournament();
}

Tournament::~Tournament()
{
	this->clean();
}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

cpSet				Tournament::getSettings() const			{	return (this->_settings);		}
vpPart				Tournament::getParticipants() const		{	return (this->_participants);	}
vpTeam				Tournament::getTeams() const			{	return (this->_teams);			}
cvpPool				Tournament::getPools() const			{	return (this->_pools);			}
Phase*				Tournament::getSixteenth() const		{	return (this->_sixteenths);		}
Phase*				Tournament::getHeighth() const			{	return (this->_heighths);		}
Phase*				Tournament::getQuarters() const			{	return (this->_quarters);		}
Phase*				Tournament::getSemis() const			{	return (this->_semis);			}
Phase*				Tournament::getFinal() const			{	return (this->_final);			}
Phase*				Tournament::getThirdPlace() const		{	return (this->_thirdPlace);		}
bool				Tournament::getHasSixteenth() const		{	return (this->_hasSixteenth);	}
bool				Tournament::getHasHeighth() const		{	return (this->_hasHeighth);		}
bool				Tournament::getHasThirdMatch() const	{	return (this->_hasHeighth);		}
bool				Tournament::getIsReady() const			{	return (this->_isReady);		}
bool				Tournament::getIsFinished() const		{	return (this->_isFinished);		}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				Tournament::setIsReady(cBool value)			{	this->_isReady = value;			}
void				Tournament::setIsFinished(cBool value)		{	this->_isFinished = value;		}
void				Tournament::setHasSixteenth(cBool value)	{	this->_hasSixteenth = value;	}
void				Tournament::setHasHeighth(cBool value)		{	this->_hasHeighth = value;		}
void				Tournament::setHasThirdMatch(cBool value)	{	this->_hasThirdMatch = value;	}

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/**
 * Retourne true si deux equipes partagent au moins un Participant*.
 */
bool				Tournament::teamsShareMember(cpTeam a, cpTeam b)
{
	for (cpPart pa : a->getMembers())
		for (cpPart pb : b->getMembers())
			if (pa == pb)
				return (true);

	return (false);
}

/**
 * Retourne true si `incoming` partage un membre avec une equipe deja dans `pool`.
 */
bool				Tournament::poolHasConflict(cpPool pool, cpTeam incoming)
{
	for (cpTeam t : pool->getTeams())
		if (teamsShareMember(t, incoming))
			return (true);

	return (false);
}

/**
 *	retourne un vecteur avec tout les participants Homme
 */
vpPart				Tournament::getAllMales() const
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
vpPart				Tournament::getAllFemales() const
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
vpPart				Tournament::getMultiTeamsPlayers(vpPart participants) const
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

/**
 *	creation des equipes d un player (tournois simple non mixte)
 */
void				Tournament::createTeamsUniplayer()
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
void				Tournament::createDoubleTeams()
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
void				Tournament::createMixedTeams()
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
 *	verifie le nombre de participants qui devront etre dans deux teams
 */
bool				Tournament::checkMissingPlayers(int& missing) const
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
 *	generation de la pool des participants qui seront dans deux teams
 */
void				Tournament::generateMissingPool(const TeamCreationCtx& ctx)
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
void				Tournament::cloneForEqualGenders(const TeamCreationCtx& ctx)
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
void				Tournament::cloneForUnequalGenders(const TeamCreationCtx& ctx)
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
void				Tournament::createStandardMixedTeams(TeamCreationCtx& ctx)
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
void				Tournament::createMissingMixedTeams(TeamCreationCtx& ctx)
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
void				Tournament::createUnigenreTeams(TeamCreationCtx& ctx)
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
 *	Genere les pools vide
 */
void				Tournament::createEmptyPools(int nbPools)
{
	for (int i = 0; i < nbPools; ++i)
		this->_pools.push_back(new Pool());
}

/**
 *	Distribu les equipes selon le genre
 */
void				Tournament::distributeTeamsToPools(int nbPools)
{
	vpTeam mixedTeams;
	vpTeam otherTeams;

	for (Team* t : this->_teams)
	{
		cvpPart members = t->getMembers();
		bool hasMale = false;
		bool hasFemale = false;

		if (members.size() == 2)
			for (cpPart p : members)
			{
				if (p->getGenderInt() == Gender::MALE)
					hasMale = true;

				if (p->getGenderInt() == Gender::FEMALE)
					hasFemale = true;
			}

		if (hasMale && hasFemale)
			mixedTeams.push_back(t);
		else
			otherTeams.push_back(t);
	}

	int poolIdx = 0;

	for (Team* t : mixedTeams)
		this->_pools[poolIdx++ % nbPools]->addTeam(t);

	for (Team* t : otherTeams)
		this->_pools[poolIdx++ % nbPools]->addTeam(t);
}

/**
 *	Verifie les conflis des equipes a joueurs multi-teams et les corrige
 */
void				Tournament::resolvePoolConflicts(int nbPools)
{
	for (int i = 0; i < nbPools; ++i)
	{
		vpTeam& teamsI = const_cast<vpTeam&>(this->_pools[i]->getTeams());

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

					vpTeam& teamsJ = const_cast<vpTeam&>(this->_pools[j]->getTeams());

					for (size_t c = 0; c < teamsJ.size() && !swapped; ++c)
					{
						Team* candidate = teamsJ[c];
						bool candidateHasConflit = false;

						for (size_t k = 0; k < teamsI.size(); ++k)
							if (k != b && teamsShareMember(teamsI[k], candidate))
							{
								candidateHasConflit = true;
								break;
							}

						bool otherCandidateHasConflit = false;

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

				if (!swapped)
					PrintUtils::addError(std::format("generatePools : conflit non resolu dans {}, un multi-team player jouera contre lui-meme.", 
						this->_pools[i]->getName()));
			}
		}
	}
}

/**
 *	Genere les matchs de pool
 */
void				Tournament::generatePoolMatches()
{
	cInt nbSetPlayed = this->_settings->getNbSetPlayedPools();
	
	for (Pool* p : this->_pools)
		p->generateMatches(nbSetPlayed);
}

/**
 * Trie l ensemble des equipes dans toutes les poules
 */
void				Tournament::sortAllPools() const
{
	for (Pool* p : this->_pools)
		p->sortTeams();
}

/**
 * Genere les affrontements entre le 1er d une poule (i) et le 2e de la poule opposée (N - 1 - i)
 */
void				Tournament::generateSymmetricPoolEncounters(Phase* targetPhase, const size_t nbPools) const
{
	this->sortAllPools();

	for (size_t i = 0; i < nbPools / 2; ++i)
	{
		targetPhase->addEncounter(this->_pools[i]->getQualifiers()[0], this->_pools[nbPools - 1 - i]->getQualifiers()[1]);
		targetPhase->addEncounter(this->_pools[nbPools - 1 - i]->getQualifiers()[0], this->_pools[i]->getQualifiers()[1]);
	}
}

/**
 * Recupere les vainqueurs de la phase precedente et les affronte 2 à 2
 */
bool				Tournament::addEncountersFromPreviousPhase(Phase* currentPhase, cpPhase previousPhase)
{
	if (!previousPhase || !previousPhase->isFinished())
		return (false);

	cvpTeam winners = previousPhase->getWinners();

	for (size_t i = 0; i + 1 < winners.size(); i += 2)
		currentPhase->addEncounter(winners[i], winners[i + 1]);

	return (true);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 *	Libere toute la memoire utilise par l objet Tournament
 */
void				Tournament::clean()
{
	for (cpPart p : this->_participants)
		delete p;

	for (cpTeam t : this->_teams)
		delete t;

	for (cpPool p: this->_pools)
		delete p;
	
	this->_participants.clear();
	this->_teams.clear();
	this->_pools.clear();

	delete this->_sixteenths;
	delete this->_heighths;
	delete this->_quarters;
	delete this->_semis;
	delete this->_final;
	delete this->_thirdPlace;
}

/**
 *	Ajoute un participant dans la liste
 */
void				Tournament::addParticipant(cPart p)
{
	this->_participants.push_back(new Participant(p.getPseudo(), p.getLastName(), p.getFirstName(), p.getGenderInt()));
}

/**
 *	Initialise le debut du tournois en creant les teams et les pools
 */
bool				Tournament::initializeTournament()
{
	cInt required = this->_settings->getNbPlayers();
	cInt actual = static_cast<int>(this->_participants.size());

	if (actual < required && !this->_settings->getAllowMultiTeamPlayers())
	{
		PrintUtils::addError(std::format("Pas assez de joueurs ({}/{}).", actual, required));
		return (false);
	}

	if (actual == 0)
	{
		PrintUtils::addError("Aucun joueur inscrit.");
		return (false);
	}

	generateTeams();
	generatePools();

	cInt nbPools = this->_settings->getNbPools();

	this->_hasSixteenth = (nbPools == 16);
	this->_hasHeighth = (nbPools >= 8);
	this->_isReady = true;
	
	return (true);
}

pTeam				Tournament::getTeamById(int id) const
{
	if (this->_teams.size() < (static_cast<size_t>(id) - 1))
		return (nullptr);
	
	return (this->_teams[id - 1]);
}

/**
*	Genere les equipes selon les settings MIXED/DOUBLE/ALLOWMULTITEAMPLAYER
*/
void				Tournament::generateTeams()
{
	if (!this->_teams.empty())
		return ;

	if (!this->_settings->getIsDouble())
		createTeamsUniplayer();
	else if (this->_settings->getIsMixed())
		createMixedTeams();
	else
		createDoubleTeams();
}

/**
 * Genere les pool en respectant les donnees des settings
 */
void				Tournament::generatePools()
{
	cInt nbPools = this->_settings->getNbPools();

	if (!this->_pools.empty() || nbPools <= 0)
		return;

	this->createEmptyPools(nbPools);
	this->distributeTeamsToPools(nbPools);
	this->resolvePoolConflicts(nbPools);
	this->generatePoolMatches();
}

/**
 * Genere les 1/16 en selectionnant les 1 et 2 de chaque pool
 */
void				Tournament::generateSixteenths()
{
	for (cpPool p: this->_pools)
		if (!p->allMatchesFinished())
			return;

	if (this->_sixteenths || !this->_hasSixteenth || this->_pools.size() < 16)
		return;

	this->_sixteenths = new Phase("1/16 de Finale", this->_settings->getNbSetPlayedSixteenth());
	
	this->generateSymmetricPoolEncounters(this->_sixteenths, 16);
}

/**
 * Genere les 1/8 soit en selectionnant les vainqueurs des 1/16 soit les 1 et 2 de chaque pool
 */
void				Tournament::generateHeighths()
{
	for (cpPool p: this->_pools)
		if (!p->allMatchesFinished())
			return;

	if (this->_heighths || !this->_hasHeighth)
		return;

	this->_heighths = new Phase("1/8 de Finale", this->_settings->getNbSetPlayedHeigth());

	if (this->_hasSixteenth)
	{
		if (!this->addEncountersFromPreviousPhase(this->_heighths, this->_sixteenths))
		{
			delete this->_heighths;
			this->_heighths = nullptr;
		}
	}
	else if (this->_pools.size() >= 8)
		this->generateSymmetricPoolEncounters(this->_heighths, 8);
}

/**
 * Genere les 1/4 soit en selectionnant les vainqueurs des 1/8 soit les 1 et 2 de chaque pool
 */
void				Tournament::generateQuarters()
{
	for (cpPool p: this->_pools)
		if (!p->allMatchesFinished())
			return;

	if (this->_quarters)
		return;

	this->_quarters = new Phase("Quarts de Finale", this->_settings->getNbSetPlayedQuarters());

	if (this->_hasHeighth)
	{
		if (!this->addEncountersFromPreviousPhase(this->_quarters, this->_heighths))
		{
			delete this->_quarters;
			this->_quarters = nullptr;
		}
	}
	else if (this->_pools.size() >= 4)
	{
		this->sortAllPools();

		for (size_t i = 0; i < 2; ++i)
		{
			this->_quarters->addEncounter(this->_pools[i]->getQualifiers()[0], this->_pools[i + 2]->getQualifiers()[1]);
			this->_quarters->addEncounter(this->_pools[i + 2]->getQualifiers()[0], this->_pools[i]->getQualifiers()[1]);
		}
	}
}

/**
 * Genere les 1/2 en recuperant les vainqueurs des 1/4
 */
void				Tournament::generateSemis()
{
	if (this->_semis || !this->_quarters || !this->_quarters->isFinished())
		return;

	cvpTeam winners = this->_quarters->getWinners();

	if (winners.size() < 4)
		return;

	this->_semis = new Phase("Demi-Finales", this->_settings->getNbSetPlayedSemis());

	this->_semis->addEncounter(winners[0], winners[2]);
	this->_semis->addEncounter(winners[1], winners[3]);
}

/**
 * Genere la final en recuperant les vainqueurs des 1/2
 */
void				Tournament::generateFinal()
{
	if (this->_final || !this->_semis || !this->_semis->isFinished())
		return;

	cvpTeam winners = this->_semis->getWinners();

	if (winners.size() < 2)
		return;

	this->_final = new Phase("Finale", this->_settings->getNbSetPlayedFinal());
	
	this->_final->addEncounter(winners[0], winners[1]);
}

/**
 * Genere la phase pour la 3eme place en recuperant les perdants des 1/2
 */
void				Tournament::generateThirdPlace()
{
	if (this->_thirdPlace || !this->_semis || !this->_semis->isFinished() || !this->_settings->getIsThirdPlaceMatch())
		return;

	cvpTeam losers = this->_semis->getLosers();

	if (losers.size() < 2)
		return;

	this->_thirdPlace = new Phase("Petite Finale", this->_settings->getNbSetPlayedThirdPlace());
	this->_thirdPlace->addEncounter(losers[0], losers[1]);
}
