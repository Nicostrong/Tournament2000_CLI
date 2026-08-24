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

using				cGender			=	const Gender&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 * la classe Participant represente un joueur humain.
 */
class				Participant
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

		Participant() = delete;
		Participant(cString pseudo, cString lastName, cString firstName, cGender gender);
		Participant(const Participant& p) = delete;
		Participant&				operator=(const Participant& p) = delete;
		~Participant() = default;

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
		void						setPseudo(cString value);
		void						setFirstName(cString value);
		void						setLastName(cString value);
		void						setGender(cGender value);
		void						setIsEliminated(cBool value);
		void						setIsMultiTeamPlayer(cBool value);

		//	METHODE

		bool						operator==(const Participant& other) const;

};
