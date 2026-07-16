//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef TOURNAMANT_HPP
# define TOURNAMANT_HPP

//	STDLIB
# include <vector>

//	INCLUDES
# include "./Settings.hpp"
# include "./Participant.hpp"
# include "./Team.hpp"
# include "./Phase.hpp"
# include "./Pool.hpp"

//	TYPEDEF
typedef std::vector<Participant*>	VP_PART;
typedef std::vector<Team*>			VP_TEAM;
typedef std::vector<Pool*>			VP_POOL;

class Tournament
{
	private:

		Settings					_settings;

		VP_PART						_participants;
		VP_TEAM						_teams;
		VP_POOL						_pools;

		Phase*						_sixteenths;
		Phase*						_heighths;
		Phase*						_quarters;
		Phase*						_semis;
		Phase*						_final;
		Phase*						_thirdPlace;

		bool						_isReady;
		bool						_isFinished;
		bool						_hasSixteenth;
		bool						_hasHeighth;

		void						generateTeams();
		void						generatePools();

		struct TeamCreationCtx
		{
			VP_PART*				males = nullptr;
			VP_PART*				females = nullptr;
			VP_PART*				minoritary = nullptr;
			VP_PART*				majoritary = nullptr;
			VP_PART*				missingPool = nullptr;

			size_t					minIdx = 0;
			size_t					majIdx = 0;
			size_t					missIdx = 0;

			int						missing = 0;
		};

		void						createTeamsUniplayer();
		void						createMixedTeams();
		void						createDoubleTeams();
		bool						checkMissingPlayers(int& missing) const;
		void						generateMissingPool(TeamCreationCtx& ctx);

		static void					cloneForEqualGenders(TeamCreationCtx& ctx);
		static void					cloneForUnequalGenders(TeamCreationCtx& ctx);
		static void					createStandardMixedTeams(TeamCreationCtx& ctx);
		static void					createMissingMixedTeams(TeamCreationCtx& ctx);
		static void					createUnigenreTeams(TeamCreationCtx& ctx);

		VP_PART						getAllMales();
		VP_PART						getAllFemales();
		VP_PART						getMultiTeamsPlayers(VP_PART pool);

	public:

		//	CANONICAL
		explicit Tournament(const Settings& settings);
		Tournament() = delete;
		Tournament(const Tournament& ) = delete;
		~Tournament();

		Tournament&					operator=(const Tournament& ) = delete;

		//	GETTER
		[[nodiscard]]
		const Settings&				getSettings() const;
		[[nodiscard]]
		const VP_POOL&				getPools() const;

		[[nodiscard]]
		Phase*						getSixteenth() const;
		[[nodiscard]]
		Phase*						getHeighth() const;
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
		bool						getHasHeighth() const;

		//	METHOD
		void						addParticipant(const Participant& p);
		void						generateSixteenths();
		void						generateHeighths();
		void						generateQuarters();
		void						generateSemis();
		void						generateFinal();
		void						generateThirdPlace();

		bool						initializeTournament();

};

#endif