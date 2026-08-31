//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/


/****************************************************************************************************/
/*	ENUM    																						*/
/****************************************************************************************************/

enum				Gender			{ MALE, FEMALE, MIXED };

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cBool			=	const bool;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 * la classe Player represente un joueur humain.
 */
class				Player
{
	private:

		static int					_idCounter;
		size_t						_id;
		String						_pseudo;
		String						_lastName;
		String						_firstName;
		Gender						_gender;
		bool						_isEliminated;
		bool						_isMultiTeamPlayer;

	public:

		Player() = delete;
		Player(String pseudo, String lastName, String firstName, Gender gender);

		Player(const Player&) = delete;
		Player& operator=(const Player&) = delete;

		Player(Player&&) = delete;
		Player& operator=(Player&&) = delete;

		~Player() = default;

		//	GETTER
		[[nodiscard]]
		size_t						getId() const;
		[[nodiscard]]
		cString						getPseudo() const;
		[[nodiscard]]
		cString						getFirstName() const;
		[[nodiscard]]
		cString						getLastName() const;
		[[nodiscard]]
		String						getGenderStr() const;
		[[nodiscard]]
		String						getFullName() const;
		[[nodiscard]]
		Gender						getGenderInt() const;
		[[nodiscard]]
		bool						getIsEliminated() const;
		[[nodiscard]]
		bool						getIsMultiTeamPlayer() const;

		//	SETTER
		void						setPseudo(String value);
		void						setFirstName(String value);
		void						setLastName(String value);
		void						setGender(Gender value);
		void						setIsEliminated(bool value);
		void						setIsMultiTeamPlayer(bool value);

		//	METHODE

		bool						operator==(const Player& other) const;

};
