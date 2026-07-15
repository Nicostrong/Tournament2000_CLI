//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef TOURNAMANT_HPP
# define TOURNAMANT_HPP

//	STDLIB
#include <cstddef>
# include <vector>

//	INCLUDES
# include "./Settings.hpp"
# include "./Participant.hpp"
# include "./Team.hpp"
# include "./Phase.hpp"
# include "./Pool.hpp"

//	TYPEDEF
typedef std::vector<Participant*>	V_PART;
typedef std::vector<Team*>			V_TEAM;
typedef std::vector<Pool*>			V_POOL;

class Tournament
{
	private:

		Settings					_settings;

		V_PART						_participants;
		V_TEAM						_teams;
		V_POOL						_pools;

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

		Tournament();
		Tournament(const Tournament& );

		Tournament&					operator=(const Tournament& );

		struct TeamCreationCtx
		{
			V_PART*					males = nullptr;
			V_PART*					females = nullptr;			
			V_PART*					minoritary = nullptr;
			V_PART*					majoritary = nullptr;
			V_PART*					missingPool = nullptr;

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

		void						cloneForEqualGenders(TeamCreationCtx& ctx);
		void						cloneForUnequalGenders(TeamCreationCtx& ctx);
		void						createStandardMixedTeams(TeamCreationCtx& ctx);
		void						createMissingMixedTeams(TeamCreationCtx& ctx);
		void						createUnigenreTeams(TeamCreationCtx& ctx);

		V_PART						getAllMales();
		V_PART						getAllFemales();
		V_PART						getMultiTeamsPlayers(V_PART pool);

	public:

		Tournament(const Settings& settings);
		~Tournament();

		//	GETTER
		const Settings&				getSettings() const;
		const V_POOL&				getPools() const;

		Phase*						getSixteenth() const;
		Phase*						getHeighth() const;
		Phase*						getQuarters() const;
		Phase*						getSemis() const;
		Phase*						getFinal() const;
		Phase*						getThirdPlace() const;

		bool						getHasSixteenth();
		bool						getHasHeighth();

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
