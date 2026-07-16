//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB

//	INCLUDES
#include "../includes/class/Participant.hpp"
#include "../includes/utils/FormatUtils.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;

//	STATIC VARIABLES

/****************/
/*	CANONICAL	*/
/****************/

Participant::Participant(C_STRING pseudo, C_STRING firstName, C_STRING lastName, Gender gender)
	:	_pseudo(FormatUtils::capitalize(pseudo)), _firstName(FormatUtils::capitalize(firstName)), 
		_lastName(FormatUtils::toUpper(lastName)), _gender(gender), _isEliminated(false),
		_isMultiTeamPlayer(false)
{}

Participant::Participant(const Participant& p)
{
	*this = p;
}

Participant&						Participant::operator=(const Participant& p)
{
	if (this != &p)
	{
		this->_pseudo = p._pseudo;
		this->_firstName = p._firstName;
		this->_lastName = p._lastName;
		this->_gender = p._gender;
		this->_isEliminated = p._isEliminated;
		this->_isMultiTeamPlayer = p._isMultiTeamPlayer;
	}

	return (*this);
}

Participant::~Participant() {}

/************/
/*	GETTER	*/
/************/

C_STRING							Participant::getPseudo() const
{
	return (this->_pseudo);
}

C_STRING							Participant::getFirstName() const
{
	return (this->_firstName);
}

C_STRING							Participant::getLastName() const
{
	return (this->_lastName);
}

Participant::Gender					Participant::getGenderInt() const
{
	return (this->_gender);
}

STRING								Participant::getGenderStr() const
{
	return (this->_gender == MALE ? "Homme" :"Femme");
}

bool								Participant::getIsEliminated() const
{
	return (this->_isEliminated);
}

bool								Participant::getIsMultiTeamPlayer() const
{
	return (this->_isMultiTeamPlayer);
}

/************/
/*	SETTER	*/
/************/

void								Participant::setPseudo(C_STRING value)
{
	this->_pseudo = FormatUtils::capitalize(value);
}

void							 	Participant::setFirstName(C_STRING value)
{
	this->_firstName = FormatUtils::capitalize(value);
}

void								Participant::setLastName(C_STRING value)
{
	this->_lastName = FormatUtils::toUpper(value);
}

void								Participant::setGender(Gender value)
{
	this->_gender = value;
}

void								Participant::setIsEliminated(bool value)
{
	this->_isEliminated = value;
}

void								Participant::setIsMultiTeamPlayer(bool value)
{
	this->_isMultiTeamPlayer = value;
}

/********************/
/*	PRIVATE METHODS	*/
/********************/

/********************/
/*	PUBLIC METHODS	*/
/********************/
