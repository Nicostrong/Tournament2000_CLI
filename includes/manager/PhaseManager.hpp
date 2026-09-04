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
# include <memory>

# include "../class/Settings.hpp"

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Team;
class				Pool;
class				Phase;
class				Match;
class				Settings;

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
using				uPhase			=	std::unique_ptr<Phase>;

using				cSet			=	const Settings&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 *	S occupe de gerer les phases du tournoi
 */
class				PhaseManager
{
	private:

		cSet						_settings;

		uPhase						_sixteenths;
		uPhase						_eighth;
		uPhase						_quarters;
		uPhase						_semis;
		uPhase						_final;
		uPhase						_thirdPlace;

		mpTeam						_repechageHistory;

		bool						_hasSixteenth;
		bool						_hasEighth;
		bool						_hasThirdMatch;

		void						generateSymmetricPoolEncounters(pPhase targetPhase, cvpPool pools, size_t nbPools) const;
		[[nodiscard]]
		bool						addEncountersFromPreviousPhase(pPhase currentPhase, cpPhase previousPhase);
		[[nodiscard]]
		pMatch						findCurrentActiveMatch(pTeam team) const;
		[[nodiscard]]
		pMatch						findPreviousMatch(pTeam team) const;
		[[nodiscard]]
		pTeam						getNextBestTeamFromPool(pTeam disqualifiedTeam, cvpPool pools) const;

	public:
		explicit PhaseManager(cSet settings);


		PhaseManager(const PhaseManager&) = delete;
		PhaseManager& operator=(const PhaseManager&) = delete;

		PhaseManager(PhaseManager&&) = delete;
		PhaseManager& operator=(PhaseManager&&) = delete;

		~PhaseManager();

		void						clean();

		void						generateSixteenths(cvpPool pools);
		void						generateEighths(cvpPool pools);
		void						generateQuarters(cvpPool pools);
		void						generateSemis();
		void						generateFinal();
		void						generateThirdPlace();

		void						applyBracketDisqualification(pTeam team, cvpPool pools);
		void						unDisqualifyTeam(pTeam team);

		//	GETTER
		[[nodiscard]]
		pPhase						getSixteenth() const;
		[[nodiscard]]
		pPhase						getEighth() const;
		[[nodiscard]]
		pPhase						getQuarters() const;
		[[nodiscard]]
		pPhase						getSemis() const;
		[[nodiscard]]
		pPhase						getFinal() const;
		[[nodiscard]]
		pPhase						getThirdPlace() const;

		[[nodiscard]]
		bool						getHasSixteenth() const;
		[[nodiscard]]
		bool						getHasEighth() const;
		[[nodiscard]]
		bool						getHasThirdMatch() const;

		//	SETTER
		void						setHasSixteenth(bool value);
		void						setHasEighth(bool value);
		void						setHasThirdMatch(bool value);

		//	UNLOCKS
		[[nodiscard]]
		bool						isSixteenthUnlocked(bool poolsFinished) const;
		[[nodiscard]]
		bool						isEighthUnlocked(bool poolsFinished) const;
		[[nodiscard]]
		bool						isQuartersUnlocked(bool poolsFinished) const;
		[[nodiscard]]
		bool						isSemisUnlocked() const;
		[[nodiscard]]
		bool						isFinalUnlocked() const;
		[[nodiscard]]
		bool						isThirdUnlocked() const;
};