//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

#ifndef SETTINGS_HPP
# define SETTINGS_HPP

//	STDLIB
# include <algorithm>
# include <string>
# include <vector>
# include <array>

//	INCLUDES

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				V_STRING	=	std::vector<STRING>;

template<std::size_t N>
using				A_INT		=	std::array<int, N>;

//	ENUM
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

constexpr A_INT<12>					allowedNbPlayers =
{ 
	static_cast<int>(nbPlayer::douze),
	static_cast<int>(nbPlayer::seize),
	static_cast<int>(nbPlayer::vingt),
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

constexpr A_INT<3>					allowedNbPools =
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

constexpr A_INT<3>					allowedNbPlayersOrTeamsPerPools =
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

constexpr A_INT<5>					allowedNbSetToPlay =
{
	static_cast<int>(nbSetToPlay::un),
	static_cast<int>(nbSetToPlay::deux),
	static_cast<int>(nbSetToPlay::trois),
	static_cast<int>(nbSetToPlay::quatre),
	static_cast<int>(nbSetToPlay::cinq)
};

class Settings
{
	private:

		STRING						_name;

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

		static bool					addErrorIf(bool condition, C_STRING message, V_STRING& errors);

		template<typename Container>
		bool						isInList(int value, const Container& list) const
		{
			return (std::find(list.begin(), list.end(), value) != list.end());
		}

	public:

		//	CANONICAL
		Settings();
		Settings(const Settings& ) = default;
		Settings&					operator=(const Settings& );
		~Settings() = default;

		/*  GETTERS */
		[[nodiscard]]
		C_STRING					getName() const;
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

		/*  SETTERS */
		void						setName(C_STRING value);
		void						setNbPlayers(int value);
		void						setNbPlayerByPool(int value);
		void						setNbPools(int value);
		void						setNbBadmintonCourt(int value);
		void						setScoreMin(int value);
		void						setScoreMax(int value);
		void						setDiffPointsToWin(int value);

		void						setNbSetPlayedPools(int value);
		void						setNbSetPlayedSixteenth(int value);
		void						setNbSetPlayedHeigth(int value);
		void						setNbSetPlayedQuarters(int value);
		void						setNbSetPlayedSemis(int value);
		void						setNbSetPlayedFinal(int value);
		void						setNbSetPlayedThirdPlace(int value);

		void						setIsMixed(bool value);
		void						setIsDouble(bool value);
		void						setAllowMultiTeamPlayers(bool value);
		void						setIsThirdPlaceMatch(bool value);

		/*	METHOD	*/
		bool						isValid(V_STRING& errors) const;
		[[nodiscard]]
		bool						canAccommodate(int actualParticipants) const;

};

#endif
