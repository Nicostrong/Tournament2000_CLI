//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB
#include <iostream>
#include <algorithm>
#include <random>

//	INCLUDES
#include "../includes/class/Tournament.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/Constantes.hpp"

//	TYPEDEF
using				VP_PART		=	std::vector<Participant*>;
using				VP_TEAM		=	std::vector<Team*>;
using				CVP_PART	=	const std::vector<Participant*>&;
using               CVP_TEAM	=	const std::vector<Team*>&;
using				CVP_POOL	=	const std::vector<Pool*>;

//	STATIC VARIABLES

/****************/
/*	CANONICAL	*/
/****************/

Tournament::Tournament(const Settings& settings)
	:	_settings(settings), _sixteenths(nullptr), _heighths(nullptr),  _quarters(nullptr),
		_semis(nullptr), _final(nullptr), _thirdPlace(nullptr), _isReady(false), _isFinished(false),
		_hasSixteenth(false), _hasHeighth(false)
{}

Tournament::~Tournament()
{
	for (const Participant* p : this->_participants)
		delete p;

	for (const Team* t : this->_teams)
		delete t;

	for (const Pool* p: this->_pools)
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

/************/
/*	GETTER	*/
/************/

const Settings&		Tournament::getSettings() const
{
	return (this->_settings);
}

CVP_POOL			Tournament::getPools() const
{
	return (this->_pools);
}

Phase*				Tournament::getSixteenth() const
{
	return (this->_sixteenths);
}

Phase*				Tournament::getHeighth() const
{
	return (this->_heighths);
}

Phase*				Tournament::getQuarters() const
{
	return (this->_quarters);
}

Phase*				Tournament::getSemis() const
{
	return (this->_semis);
}

Phase*				Tournament::getFinal() const
{
	return (this->_final);
}

Phase*				Tournament::getThirdPlace() const
{
	return (this->_thirdPlace);
}

bool				Tournament::getHasSixteenth() const
{
	return (this->_hasSixteenth);
}

bool				Tournament::getHasHeighth() const
{
	return (this->_hasHeighth);
}

/************/
/*	SETTER	*/
/************/

/********************/
/*	PRIVATE METHOD	*/
/********************/

/**
 *	retourne un vecteur avec tout les participants Homme
 */
VP_PART				Tournament::getAllMales() const
{
	VP_PART			males;

	for (Participant* p : this->_participants)
		if (p->getGenderInt() == Participant::MALE)
			males.push_back(p);

	return (males);
}

/**
 *	retourne un vecteur avec tout les participants Femme
 */
VP_PART				Tournament::getAllFemales() const
{
	VP_PART			females;

	for (Participant* p : this->_participants)
		if (p->getGenderInt() == Participant::FEMALE)
			females.push_back(p);

	return (females);
}

/**
 *	retourne un vecteur avec tout les participants qui sont dans plusieurs teams dans une pool donnee
 */
VP_PART				Tournament::getMultiTeamsPlayers(VP_PART participants) const
{
	VP_PART			multiTeamsPlayers;

	//const int		actual = static_cast<int>(this->_participants.size());
	//const int		required = this->_settings.getNbPlayers();
	//const int		missing = std::max(0, required - actual);
	//const int		maxRecyclable = std::min(missing, NBPLAYERINMULTITEAMMAX);

	if ((std::min(std::max(0, this->_settings.getNbPlayers() - static_cast<int>(this->_participants.size())), NBPLAYERINMULTITEAMMAX)) <= 0 || participants.empty())
		return (multiTeamsPlayers);

	std::random_device				rd;
	std::mt19937					g(rd());

	std::shuffle(participants.begin(), participants.end(), g);

	//for (int i = 0; i < maxRecyclable && i < static_cast<int>(pool.size()); i++)
	for (Participant* p: participants)
	{
		p->setIsMultiTeamPlayer(true);
		multiTeamsPlayers.push_back(p);
	}

	return (multiTeamsPlayers);
}

/**
 *	creation des equipes d un player (tournois simple non mixte)
 */
void				Tournament::createTeamsUniplayer()
{
	for (Participant* p : this->_participants)
	{
		Team*		t = new Team();

		t->addMember(p);
		this->_teams.push_back(t);
	}
}

/**
 *	creations des equipes en double (tournois double non mixte)
 */
void				Tournament::createDoubleTeams()
{
	VP_PART			pool = this->_participants;
	CVP_PART		multiTeams = getMultiTeamsPlayers(pool);

	std::random_device				rd;
	std::mt19937					g(rd());

	std::shuffle(pool.begin(), pool.end(), g);
	
	size_t pIdx = 0;
	size_t mIdx = 0;

	while (pIdx < pool.size() || mIdx < multiTeams.size())
	{
		Team*		t = new Team();
		
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
	TeamCreationCtx	ctx;

	if (!checkMissingPlayers(ctx.missing))
		return ;

	VP_PART			males = getAllMales();
	VP_PART			females = getAllFemales();
	VP_PART			minoritaryPool = (females.size() < males.size()) ? females : males;
	VP_PART			majoritaryPool = (females.size() < males.size()) ? males : females;
	VP_PART			missingPool;

	ctx.males = &males;
	ctx.females = &females;
	ctx.minoritary = &minoritaryPool;
	ctx.majoritary = &majoritaryPool;
	ctx.missingPool = &missingPool;

	generateMissingPool(ctx);

	std::random_device				rd;
	std::mt19937					g(rd());

	std::shuffle(majoritaryPool.begin(), majoritaryPool.end(), g);
	std::shuffle(minoritaryPool.begin(), minoritaryPool.end(), g);
	std::shuffle(missingPool.begin(), missingPool.end(), g);

	createStandardMixedTeams(ctx);
	createMissingMixedTeams(ctx);
	createUnigenreTeams(ctx);
}

/**
 *	verifie le nombre de participants qui devront etre dans deux teams
 */
bool				Tournament::checkMissingPlayers(int& missing) const
{
	const int		actual = static_cast<int>(this->_participants.size());
	const int		required = this->_settings.getNbPlayers();

	missing = std::max(0, required - actual);

	if (missing > NBPLAYERINMULTITEAMMAX)
	{
		std::cerr << "Erreur : Trop de joueurs manquants (" << missing 
				<< "). Limite de reutilisation autorisee : " << NBPLAYERINMULTITEAMMAX << std::endl;
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
		return ;

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
	std::random_device				rd;
	std::mt19937	g(rd());
	
	VP_PART			sampleA = *(ctx.males);
	VP_PART			sampleB = *(ctx.females);

	std::shuffle(sampleA.begin(), sampleA.end(), g);
	std::shuffle(sampleB.begin(), sampleB.end(), g);

	for (int i = 0; i < ctx.missing; ++i)
	{
		if (i % 2 == 0)
		{
			Participant*			p = sampleA[i % sampleA.size()];

			p->setIsMultiTeamPlayer(true);
			ctx.missingPool->push_back(p);
		}
		else
		{
			Participant*			p = sampleB[i % sampleB.size()];

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
	std::random_device				rd;
	std::mt19937					g(rd());

	VP_PART			candidates = *(ctx.minoritary);

	std::shuffle(candidates.begin(), candidates.end(), g);

	for (int i = 0; i < ctx.missing; ++i)
	{
		Participant*				p = candidates[i % candidates.size()];

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
		Team*		t = new Team();

		t->addMember((*ctx.minoritary)[ctx.minIdx++]);
		t->addMember((*ctx.majoritary)[ctx.majIdx++]);
		
		if (t->getMembers()[0]->getIsMultiTeamPlayer() || t->getMembers()[1]->getIsMultiTeamPlayer())
			t->setHasMultiTeamPlayer(true);

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
		Team*		t = new Team();

		t->addMember((*ctx.missingPool)[ctx.missIdx++]);
		t->addMember((*ctx.majoritary)[ctx.majIdx++]);
		t->setHasMultiTeamPlayer(true);
		
		this->_teams.push_back(t);
	}
}

/**
 *	creation des equipes unigenre
 */
void				Tournament::createUnigenreTeams(TeamCreationCtx& ctx)
{
	VP_PART			leftovers;

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
			
		this->_teams.push_back(t);
	}
}

/********************/
/*	PUBLIC METHOD	*/
/********************/

/**
*	Ajoute un participant dans la liste
*/
void				Tournament::addParticipant(const Participant& p)
{
	this->_participants.push_back(new Participant(p.getPseudo(), p.getFirstName(), p.getLastName(), p.getGenderInt()));
}

/**
*	Initialise le debut du tournois en creant les teams et les pools
*/
bool				Tournament::initializeTournament()
{
	const int		required = this->_settings.getNbPlayers();
	const int		actual = static_cast<int>(this->_participants.size());

	if (actual < required && !this->_settings.getAllowMultiTeamPlayers())
	{
		std::cerr << "Erreur: Pas assez de joueurs (" << actual << "/" << required << ")" << std::endl;
		return (false);
	}

	if (actual == 0)
	{
		std::cerr << "Erreur: Aucun joueur inscrit." << std::endl;
		return (false);
	}

	generateTeams();
	generatePools();

	const int		nbPools = this->_settings.getNbPools();

	this->_hasSixteenth = (nbPools == 16);
	this->_hasHeighth = (nbPools >= 8);
	this->_isReady = true;
	
	return (true);
}

/**
*	Genere les equipes selon les settings MIXED/DOUBLE/ALLOWMULTITEAMPLAYER
*/
void				Tournament::generateTeams()
{
	if (!this->_teams.empty())
		return ;

	if (!this->_settings.getIsDouble())
		createTeamsUniplayer();
	else if (this->_settings.getIsMixed())
		createMixedTeams();
	else
		createDoubleTeams();

}

void				Tournament::generatePools()
{
	if (!this->_pools.empty())
		return ;

	const int		nbPools = this->_settings.getNbPools();

	if (nbPools <= 0)
		return ;

	for (int i = 0; i < nbPools; ++i)
		this->_pools.push_back(new Pool());

	VP_TEAM			mixedTeams;
	VP_TEAM			otherTeams;


	for (Team* t : this->_teams)
	{
		CVP_PART	members = t->getMembers();
		bool		hasMale = false;
		bool		hasFemale = false;

		if (members.size() == 2)
			for (const Participant* p : members)
			{
				if (p->getGenderInt() == Participant::MALE)
					hasMale = true;

				if (p->getGenderInt() == Participant::FEMALE)
					hasFemale = true;
			}

		if (hasMale && hasFemale)
			mixedTeams.push_back(t);
		else
			otherTeams.push_back(t);
	}

	int				poolIdx = 0;

	for (Team* t : mixedTeams)
		this->_pools[poolIdx++ % nbPools]->addTeam(t);

	for (Team* t : otherTeams)
		this->_pools[poolIdx++ % nbPools]->addTeam(t);

	for (Pool* p : this->_pools)
		p->generateMatches(this->_settings.getNbSetPlayedPools());
}

void				Tournament::generateSixteenths()
{
	if (this->_sixteenths || !this->_hasSixteenth || this->_pools.size() < 16)
		return ;

	for (Pool* p : this->_pools)
		p->sortTeams();

	this->_sixteenths = new Phase("1/16 de Finale", this->_settings.getNbSetPlayedSixteenth());
	
	for (int i = 0; i < 8; ++i)
	{
		this->_sixteenths->addEncounter(this->_pools[i]->getQualifiers()[0], this->_pools[15 - i]->getQualifiers()[1]);
		this->_sixteenths->addEncounter(this->_pools[15 - i]->getQualifiers()[0], this->_pools[i]->getQualifiers()[1]);
	}
}

void				Tournament::generateHeighths()
{
	if (this->_heighths || !this->_hasHeighth)
		return ;

	this->_heighths = new Phase("1/8 de Finale", this->_settings.getNbSetPlayedHeigth());

	if (this->_hasSixteenth)
	{
		if (!this->_sixteenths || !this->_sixteenths->isFinished())
		{
			delete this->_heighths;

			this->_heighths = nullptr;
			return ;
		}

		CVP_TEAM	winners = this->_sixteenths->getWinners();

		for (size_t i = 0; i < winners.size(); i += 2)
			this->_heighths->addEncounter(winners[i], winners[i + 1]);
	}
	else if (this->_pools.size() >= 8)
	{
		for (Pool* p : this->_pools)
			p->sortTeams();

		for (int i = 0; i < 4; ++i)
		{
			this->_heighths->addEncounter(this->_pools[i]->getQualifiers()[0], this->_pools[7 - i]->getQualifiers()[1]);
			this->_heighths->addEncounter(this->_pools[7 - i]->getQualifiers()[0], this->_pools[i]->getQualifiers()[1]);
		}
	}
}

void				Tournament::generateQuarters()
{
	if (this->_quarters)
		return ;

	this->_quarters = new Phase("Quarts de Finale", this->_settings.getNbSetPlayedQuarters());

	if (this->_hasHeighth)
	{
		if (!this->_heighths || !this->_heighths->isFinished())
		{
			delete this->_quarters;

			this->_quarters = nullptr;
			return ;
		}

		CVP_TEAM	winners = this->_heighths->getWinners();

		for (size_t i = 0; i < winners.size(); i += 2)
			this->_quarters->addEncounter(winners[i], winners[i + 1]);
	}
	else if (this->_pools.size() >= 4)
	{
		for (Pool* p : this->_pools)
			p->sortTeams();
		
		this->_quarters->addEncounter(this->_pools[0]->getQualifiers()[0], this->_pools[2]->getQualifiers()[1]);
		this->_quarters->addEncounter(this->_pools[1]->getQualifiers()[0], this->_pools[3]->getQualifiers()[1]);
		this->_quarters->addEncounter(this->_pools[2]->getQualifiers()[0], this->_pools[0]->getQualifiers()[1]);
		this->_quarters->addEncounter(this->_pools[3]->getQualifiers()[0], this->_pools[1]->getQualifiers()[1]);
	}
}

void				Tournament::generateSemis()
{
	if (this->_semis || !this->_quarters || !this->_quarters->isFinished())
		return ;

	const VP_TEAM	winners = this->_quarters->getWinners();

	if (winners.size() < 4)
		return ;

	this->_semis = new Phase("Demi-Finales", this->_settings.getNbSetPlayedSemis());
	
	this->_semis->addEncounter(winners[0], winners[2]);
	this->_semis->addEncounter(winners[1], winners[3]);
}

void				Tournament::generateFinal()
{
	if (this->_final || !this->_semis || !this->_semis->isFinished())
		return ;

	const VP_TEAM	winners = this->_semis->getWinners();

	if (winners.size() < 2)
		return ;

	this->_final = new Phase("Finale", this->_settings.getNbSetPlayedFinal());
	
	this->_final->addEncounter(winners[0], winners[1]);
}

