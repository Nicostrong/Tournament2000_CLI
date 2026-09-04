//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>

# include "./Player.hpp"

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

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

	public:

		Settings();

		Settings(const Settings&) = default;
		Settings& operator=(const Settings&) = default;

		Settings(Settings&&) = default;
		Settings& operator=(Settings&&) = default;

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
		void						setName(String value);
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
		void						setIsValid(bool value);
		void						setTournamentGender(Gender value);

};
