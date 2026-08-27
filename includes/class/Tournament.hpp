//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <map>
# include <string>
# include <vector>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Team;
class				Pool;
class				Phase;
class				Match;
class				Settings;
class				Participant;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

using				pPool			=	Pool*;
using				cPool			=	const Pool&;
using				cpPool			=	const Pool*;
using				vpPool			=	std::vector<Pool*>;
using				cvpPool			=	const std::vector<Pool*>&;

using				pTeam			=	Team*;
using				cTeam			=	const Team&;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;
using				mpTeam			=	std::map<Team*, Team*>;

using				pMatch			=	Match*;
using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

using				pPhase			=	Phase*;
using				cPhase			=	const Phase&;
using				cpPhase			=	const Phase*;
using				vpPhase			=	std::vector<Phase*>;
using				cvpPhase		=	const std::vector<Phase*>&;

using				pSet			=	Settings*;
using				cSet			=	const Settings&;
using				cpSet			=	const Settings*;

using				pPart			=	Participant*;
using				cPart			=	const Participant&;
using				cpPart			=	const Participant*;
using				vpPart			=	std::vector<Participant*>;
using				cvpPart			=	const std::vector<Participant*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				Tournament
{
	private:

		pSet						_settings;

		vpPart						_participants;
		vpTeam						_teams;
		vpPool						_pools;

		Phase*						_sixteenths;
		Phase*						_eighth;
		Phase*						_quarters;
		Phase*						_semis;
		Phase*						_final;
		Phase*						_thirdPlace;

		mpTeam						_repechageHistory;

		bool						_isReady;
		bool						_isFinished;
		bool						_hasSixteenth;
		bool						_hasEighth;
		bool						_hasThirdMatch;

		void						generateTeams();
		void						generatePools();

		struct TeamCreationCtx
		{
			vpPart*					males			=	nullptr;
			vpPart*					females			=	nullptr;
			vpPart*					minoritary		=	nullptr;
			vpPart*					majoritary		=	nullptr;
			vpPart*					missingPool		=	nullptr;

			size_t					minIdx			=	0;
			size_t					majIdx			=	0;
			size_t					missIdx			=	0;

			int						missing			=	0;
		};

		//	METHOD
		bool						teamsShareMember(cpTeam a, cpTeam b);
		bool						poolHasConflict(cpPool pool, cpTeam incoming);
		void						createTeamsUniplayer();
		void						createMixedTeams();
		void						createDoubleTeams();
		bool						checkMissingPlayers(int& missing) const;
		static void					generateMissingPool(const TeamCreationCtx& ctx);

		static void					cloneForEqualGenders(const TeamCreationCtx& ctx);
		static void					cloneForUnequalGenders(const TeamCreationCtx& ctx);
		void						createStandardMixedTeams(TeamCreationCtx& ctx);
		void						createMissingMixedTeams(TeamCreationCtx& ctx);
		void						createUnigenreTeams(TeamCreationCtx& ctx);

		void						createEmptyPools(cInt nbPools);
		void						distributeTeamsToPools(cInt nbPools);
		void						resolvePoolConflicts(cInt nbPools);
		void						generatePoolMatches();
		void						sortAllPools() const;
		void						generateSymmetricPoolEncounters(pPhase targetPhase, size_t nbPools) const;
		bool						addEncountersFromPreviousPhase(pPhase currentPhase, cpPhase previousPhase);
		void						applyPoolDisqualification(pTeam team);
		void						applyBracketDisqualification(pTeam team);
		pMatch						findCurrentActiveMatch(pTeam team) const;
		pMatch						findPreviousMatch(pTeam team) const;
		pTeam						getNextBestTeamFromPool(pTeam disqualifiedTeam) const;

		//	GETTER
		[[nodiscard]]
		vpPart						getAllMales() const;
		[[nodiscard]]
		vpPart						getAllFemales() const;
		[[nodiscard]]
		vpPart						getMultiTeamsPlayers(vpPart participants) const;
		
		//	SETTER
		void						setIsReady(cBool value);
		void						setIsFinished(cBool value);

	public:

		explicit Tournament(pSet settings);
		Tournament(pSet settings, vpPart participants);
		Tournament() = delete;
		Tournament(const Tournament& ) = delete;
		~Tournament();

		Tournament&					operator=(const Tournament& ) = delete;

		//	GETTER
		[[nodiscard]]
		vpPart						getParticipants() const;
		[[nodiscard]]
		vpTeam						getTeams() const;
		[[nodiscard]]
		pTeam						getTeamById(int id) const;
		[[nodiscard]]
		cpSet						getSettings() const;
		[[nodiscard]]
		cvpPool						getPools() const;
		[[nodiscard]]
		Phase*						getSixteenth() const;
		[[nodiscard]]
		Phase*						getEighth() const;
		[[nodiscard]]
		Phase*						getQuarters() const;
		[[nodiscard]]
		Phase*						getSemis() const;
		[[nodiscard]]
		Phase*						getFinal() const;
		[[nodiscard]]
		Phase*						getThirdPlace() const;
		[[nodiscard]]
		bool						getHasSixteenth() const;
		[[nodiscard]]
		bool						getHasEighth() const;
		[[nodiscard]]
		bool						getHasThirdMatch() const;
		[[nodiscard]]
		bool						getIsReady() const;
		[[nodiscard]]
		bool						getIsFinished() const;

		//	SETTER
		void						setHasSixteenth(cBool value);
		void						setHasEighth(cBool value);
		void						setHasThirdMatch(cBool value);

		//	METHOD
		void						clean();
		void						addParticipant(cPart p);
		void						generateSixteenths();
		void						generateEighths();
		void						generateQuarters();
		void						generateSemis();
		void						generateFinal();
		void						generateThirdPlace();

		void						disqualifyTeam(pTeam team);
		void						unDisqualifyTeam(pTeam team);

		bool						initializeTournament();

		//	CHECKER
		bool						isPoolsFinished() const;
		bool						isSixteenthUnlocked() const;
		bool						isEighthUnlocked() const;
		bool						isQuartersUnlocked() const;
		bool						isSemisUnlocked() const;
		bool						isFinalUnlocked() const;
		bool						isThirdUnlocked() const;

};
