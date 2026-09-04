//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <array>
# include <string>
# include <vector>
# include <algorithm>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Settings;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;

using				cInt			=	const int;
template<std::size_t N>
using				aInt			=	std::array<int, N>;

using				cSet			=	const Settings&;

/****************************************************************************************************/
/*	ENUM																							*/
/****************************************************************************************************/

enum class			nbPlayer : int
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

constexpr aInt<9>	allowedNbPlayersSimple =
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

constexpr aInt<9>	allowedNbPlayersDouble =
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

enum class			nbPool : int
{
	quatre = 4,
	huit = 8,
	seize = 16
};

constexpr aInt<3>	allowedNbPools =
{
	static_cast<int>(nbPool::quatre),
	static_cast<int>(nbPool::huit),
	static_cast<int>(nbPool::seize)
};

enum class			nbPlayersOrTeamPerPool : int
{
	trois = 3,
	quatre = 4,
	cinq = 5
};

constexpr aInt<3>	allowedNbPlayersOrTeamsPerPools =
{
	static_cast<int>(nbPlayersOrTeamPerPool::trois),
	static_cast<int>(nbPlayersOrTeamPerPool::quatre),
	static_cast<int>(nbPlayersOrTeamPerPool::cinq)
};

enum class			nbSetToPlay : int
{
	un = 1,
	deux = 2,
	trois = 3,
	quatre = 4,
	cinq = 5
};

constexpr aInt<5>	allowedNbSetToPlay =
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

class				SettingsChecker
{
	private:

		static bool					addErrorIf(bool condition, cString message, vString& errors);
		static void					checkLogicalTournament(cSet settings, vString& errors);

		template<typename Container>
		static bool					isInList(cInt value, const Container& list)
		{
			return (std::find(list.begin(), list.end(), value) != list.end());
		}

	public:

		static bool					isPlayerCountAllowed(int nbPlayers, bool isDouble);
		static bool					isPoolMathConsistent(int nbPlayers, bool isDouble, int nbPools, int teamsPerPool);
		static bool					isValid(Settings& settings, vString& errors);
		static bool					validateAll(cSet settings);
		static bool					canAccommodate(cSet settings, cInt actualPlayers);

};