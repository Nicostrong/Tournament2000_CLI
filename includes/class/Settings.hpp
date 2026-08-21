//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>

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

using				cGender			=	const Gender&;

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

};
