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
using				VP_PART		=	std::vector<Participant*>;
using				VP_TEAM		=	std::vector<Team*>;
using				VP_POOL		=	std::vector<Pool*>;
using				CVP_POOL	=	const std::vector<Pool*>;

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
			VP_PART*				males			=	nullptr;
			VP_PART*				females			=	nullptr;
			VP_PART*				minoritary		=	nullptr;
			VP_PART*				majoritary		=	nullptr;
			VP_PART*				missingPool		=	nullptr;

			size_t					minIdx			=	0;
			size_t					majIdx			=	0;
			size_t					missIdx			=	0;

			int						missing			=	0;
		};

		//	METHOD
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

		//	GETTER
		[[nodiscard]]
		VP_PART						getAllMales() const;
		[[nodiscard]]
		VP_PART						getAllFemales() const;
		[[nodiscard]]
		VP_PART						getMultiTeamsPlayers(VP_PART participants) const;

		//	SETTER
		void						setIsReady(bool value);
		void						setIsFinished(bool value);
		void						setHasSixteenth(bool value);
		void						setHasHeighth(bool value);

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
		CVP_POOL					getPools() const;
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