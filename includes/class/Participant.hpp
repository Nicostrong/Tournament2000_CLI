//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef PARTICIPANT_HPP
# define PARTICIPANT_HPP

//	STDLIB
# include <string>

//	INCLUDES

//	TYPEDEF
using				STRING = std::string;
using				C_STRING = const std::string&;

//	STATIC VARIABLES

/**
 * la classe Participant represente un joueur humain.
 */
class Participant
{
	public:

		enum Gender { MALE, FEMALE };

	private:

		STRING						_pseudo;
		STRING						_firstName;
		STRING						_lastName;
		Gender						_gender;
		bool						_isEliminated;
		bool						_isMultiTeamPlayer;

	public:

		//	CANONICAL
		Participant() = delete;
		Participant(C_STRING pseudo, C_STRING firstName, C_STRING lastName, Gender gender);
		Participant(const Participant& p) = default;
		Participant&				operator=(const Participant& p);
		~Participant() = default;

		//	GETTER
		[[nodiscard]]
		C_STRING					getPseudo() const;
		[[nodiscard]]
		C_STRING					getFirstName() const;
		[[nodiscard]]
		C_STRING					getLastName() const;
		[[nodiscard]]
		STRING						getGenderStr() const;
		[[nodiscard]]
		Gender						getGenderInt() const;
		[[nodiscard]]
		bool						getIsEliminated() const;
		[[nodiscard]]
		bool						getIsMultiTeamPlayer() const;

		//	SETTER
		void						setPseudo(C_STRING value);
		void						setFirstName(C_STRING value);
		void						setLastName(C_STRING value);
		void						setGender(Gender value);
		void						setIsEliminated(bool value);
		void						setIsMultiTeamPlayer(bool value);

		//	METHOD

};

#endif