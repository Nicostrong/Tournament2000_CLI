//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef PARTICIPANT_HPP
# define PARTICIPANT_HPP

//	STDLIB
# include <string>

//	INCLUDES

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;

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
		Participant(const Participant& p);
		Participant&				operator=(const Participant& p);
		~Participant();

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
		void						setGender(Gender gender);
		void						setIsEliminated(bool value);
		void						setIsMultiTeamPlayer(bool value);

		//	METHOD

};

#endif