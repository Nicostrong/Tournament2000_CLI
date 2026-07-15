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
	
		Participant();

		//	PRIVATE METHODE
		static STRING				capitalize(STRING s);
		static STRING				toUpper(STRING s);

	public:
	
		Participant(C_STRING pseudo, C_STRING firstName, C_STRING lastName, Gender gender);
		
		Participant(const Participant& p);

		Participant&				operator=(const Participant& p);

		~Participant();

		//	GETTER
		C_STRING					getPseudo() const;
		C_STRING					getFirstName() const;
		C_STRING					getLastName() const;
		STRING						getGenderStr() const;
		Gender						getGenderInt() const;
		bool						getIsEliminated() const;
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