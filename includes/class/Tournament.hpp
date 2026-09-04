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

# include "../manager/PoolManager.hpp"
# include "../manager/TeamManager.hpp"
# include "../manager/PhaseManager.hpp"

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Pool;
class				Team;
class				Phase;
class				Player;
class				Settings;
class				PlayerManager;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

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
/*	CLASS																							*/
/****************************************************************************************************/

class				Tournament
{
	private:

		cSet						_settings;
		
		PlayerManager&				_playerManager;

		TeamManager					_teamManager;
		PoolManager					_poolManager;
		PhaseManager				_phaseManager;

		bool						_isReady;
		bool						_isFinished;

	public:

		Tournament(cSet settings, PlayerManager& playerManager);
		Tournament() = delete;

		Tournament(const Tournament&) = delete;
		Tournament& operator=(const Tournament&) = delete;

		Tournament(Tournament&&) = delete;
		Tournament& operator=(Tournament&&) = delete;

		~Tournament() = default;

		//	GETTER
		[[nodiscard]]
		vpPlayer					getPlayers() const;
		[[nodiscard]]
		vpTeam						getTeams() const;
		[[nodiscard]]
		pTeam						getTeamById(int id) const;
		[[nodiscard]]
		cSet						getSettings() const;
		[[nodiscard]]
		vpPool						getPools() const;
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
		[[nodiscard]]
		bool						getIsReady() const;
		[[nodiscard]]
		bool						getIsFinished() const;

		//	SETTER
		void						setHasSixteenth(bool value);
		void						setHasEighth(bool value);
		void						setHasThirdMatch(bool value);
		void						setIsReady(bool value);
		void						setIsFinished(bool value);

		//	METHOD
		void						clean();
		
		void						generateTeams();
		void						generatePools();

		void						generateSixteenths();
		void						generateEighths();
		void						generateQuarters();
		void						generateSemis();
		void						generateFinal();
		void						generateThirdPlace();

		void						disqualifyTeam(pTeam team);
		void						unDisqualifyTeam(pTeam team);

		bool						initializeTournament();

		[[nodiscard]]
		bool						isPoolsFinished() const;
		[[nodiscard]]
		bool						isSixteenthUnlocked() const;
		[[nodiscard]]
		bool						isEighthUnlocked() const;
		[[nodiscard]]
		bool						isQuartersUnlocked() const;
		[[nodiscard]]
		bool						isSemisUnlocked() const;
		[[nodiscard]]
		bool						isFinalUnlocked() const;
		[[nodiscard]]
		bool						isThirdUnlocked() const;

};