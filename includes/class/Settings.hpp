//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

#ifndef SETTINGS_HPP
# define SETTINGS_HPP

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <array>
# include <string>
# include <vector>
# include <algorithm>

# include "./Participant.hpp"

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cBool			=	const bool;

/****************************************************************************************************/
/*	ENUM																							*/
/****************************************************************************************************/

enum class nbPlayer : int
{
	douze = 12,
	seize = 16,
	vingt = 20,
	vingtQuatre = 24,
	trenteDeux = 32,
	quarante = 40,
	quaranteHuit = 48,
	soixanteQuatre = 64,
	quatreVingt = 80,
	nonanteSix = 96,
	centVingtHuit = 128,
	centSoixante = 160
};

constexpr aInt<9> allowedNbPlayersSimple =
{
	static_cast<int>(nbPlayer::douze),
	static_cast<int>(nbPlayer::seize),
	static_cast<int>(nbPlayer::vingt),
	static_cast<int>(nbPlayer::vingtQuatre),
	static_cast<int>(nbPlayer::trenteDeux),
	static_cast<int>(nbPlayer::quarante),
	static_cast<int>(nbPlayer::quaranteHuit),
	static_cast<int>(nbPlayer::soixanteQuatre),
	static_cast<int>(nbPlayer::quatreVingt)
};

constexpr aInt<9> allowedNbPlayersDouble =
{
	static_cast<int>(nbPlayer::vingtQuatre),
	static_cast<int>(nbPlayer::trenteDeux),
	static_cast<int>(nbPlayer::quarante),
	static_cast<int>(nbPlayer::quaranteHuit),
	static_cast<int>(nbPlayer::soixanteQuatre),
	static_cast<int>(nbPlayer::quatreVingt),
	static_cast<int>(nbPlayer::nonanteSix),
	static_cast<int>(nbPlayer::centVingtHuit),
	static_cast<int>(nbPlayer::centSoixante)
};

enum class nbPool : int
{
	quatre = 4,
	huit = 8,
	seize = 16
};

constexpr aInt<3>					allowedNbPools =
{
	static_cast<int>(nbPool::quatre),
	static_cast<int>(nbPool::huit),
	static_cast<int>(nbPool::seize)
};

enum class nbPlayersOrTeamPerPool : int
{
	trois = 3,
	quatre = 4,
	cinq = 5
};

constexpr aInt<3>					allowedNbPlayersOrTeamsPerPools =
{
	static_cast<int>(nbPlayersOrTeamPerPool::trois),
	static_cast<int>(nbPlayersOrTeamPerPool::quatre),
	static_cast<int>(nbPlayersOrTeamPerPool::cinq)
};

enum class nbSetToPlay : int
{
	un = 1,
	deux = 2,
	trois = 3,
	quatre = 4,
	cinq = 5
};

constexpr aInt<5>					allowedNbSetToPlay =
{
	static_cast<int>(nbSetToPlay::un),
	static_cast<int>(nbSetToPlay::deux),
	static_cast<int>(nbSetToPlay::trois),
	static_cast<int>(nbSetToPlay::quatre),
	static_cast<int>(nbSetToPlay::cinq)
};

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				Settings
{
	private:

		String						_name;

		int							_nbPlayers;
		int							_nbPlayerByPool;
		int							_nbPools;
		int							_nbBadmintonCourt;

		int							_scoreMin;
		int							_scoreMax;
		int							_diffPointsToWin;
		int							_nbSetPlayedPools;
		int							_nbSetPlayedSixteenth;
		int							_nbSetPlayedHeigth;
		int							_nbSetPlayedQuarters;
		int							_nbSetPlayedSemis;
		int							_nbSetPlayedFinal;
		int							_nbSetPlayedThirdPlace;

		bool						_isMixed;
		bool						_isDouble;
		bool						_allowMultiTeamPlayers;
		bool						_isThirdPlaceMatch;
		bool						_isValid;

		Gender						_gender;

		static bool					addErrorIf(cBool condition, cString message, vString errors);
		void						checkLogicalTournament(vString errors) const;

		template<typename Container>
		bool						isInList(cInt value, const Container& list) const
		{
			return (std::find(list.begin(), list.end(), value) != list.end());
		}

	public:

		//	CANONICAL
		Settings();
		Settings(const Settings& ) = default;
		Settings&					operator=(const Settings& ) = default;
		~Settings() = default;

		/*  GETTERS */
		[[nodiscard]]
		cString						getName() const;
		[[nodiscard]]
		int							getNbPlayers() const;
		[[nodiscard]]
		int							getNbPlayerByPool() const;
		[[nodiscard]]
		int							getNbPools() const;
		[[nodiscard]]
		int							getNbBadmintonCourt() const;
		[[nodiscard]]
		int							getScoreMin() const;
		[[nodiscard]]
		int							getScoreMax() const;
		[[nodiscard]]
		int							getDiffPointsToWin() const;
		[[nodiscard]]
		int							getNbSetPlayedPools() const;
		[[nodiscard]]
		int							getNbSetPlayedSixteenth() const;
		[[nodiscard]]
		int							getNbSetPlayedHeigth() const;
		[[nodiscard]]
		int							getNbSetPlayedQuarters() const;
		[[nodiscard]]
		int							getNbSetPlayedSemis() const;
		[[nodiscard]]
		int							getNbSetPlayedFinal() const;
		[[nodiscard]]
		int							getNbSetPlayedThirdPlace() const;

		[[nodiscard]]
		bool						getIsMixed() const;
		[[nodiscard]]
		bool						getIsDouble() const;
		[[nodiscard]]
		bool						getAllowMultiTeamPlayers() const;
		[[nodiscard]]
		bool						getIsThirdPlaceMatch() const;
		[[nodiscard]]
		bool						getIsValid() const;

		[[nodiscard]]
		Gender						getTournamentGender() const;

		/*  SETTERS */
		void						setName(cString value);
		void						setNbPlayers(cInt value);
		void						setNbPlayerByPool(cInt value);
		void						setNbPools(cInt value);
		void						setNbBadmintonCourt(cInt value);
		void						setScoreMin(cInt value);
		void						setScoreMax(cInt value);
		void						setDiffPointsToWin(cInt value);

		void						setNbSetPlayedPools(cInt value);
		void						setNbSetPlayedSixteenth(cInt value);
		void						setNbSetPlayedHeigth(cInt value);
		void						setNbSetPlayedQuarters(cInt value);
		void						setNbSetPlayedSemis(cInt value);
		void						setNbSetPlayedFinal(cInt value);
		void						setNbSetPlayedThirdPlace(cInt value);

		void						setIsMixed(cBool value);
		void						setIsDouble(cBool value);
		void						setAllowMultiTeamPlayers(cBool value);
		void						setIsThirdPlaceMatch(cBool value);
		void						setIsValid(cBool value);
		void						setTournamentGender(cGender value);

		/*	METHOD	*/
		bool						isValid(vString errors);
		[[nodiscard]]
		bool						canAccommodate(cInt actualParticipants) const;

};

#endif
