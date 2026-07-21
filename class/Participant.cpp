//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB

//	INCLUDES
#include "../includes/class/Participant.hpp"
#include "../includes/utils/FormatUtils.hpp"

//	TYPEDEF
using				STRING			=	std::string;
using				C_STRING		=	const std::string&;

//	STATIC VARIABLES
int					Participant::_idCounter = 0;

/****************/
/*	CANONICAL	*/
/****************/

Participant::Participant(C_STRING pseudo, C_STRING lastName, C_STRING firstName, const Gender gender)
	:	_id(_idCounter++), _pseudo(pseudo), _lastName(lastName), _firstName(firstName), _gender(gender),
		_isEliminated(false), _isMultiTeamPlayer(false)
{
	FormatUtils::trimAndCapitalize(this->_pseudo);
	FormatUtils::trimAndCapitalize(this->_lastName);
	FormatUtils::trimAndCapitalize(this->_firstName);
}

/*Participant&		Participant::operator=(const Participant& p)
{
	if (this != &p)
	{
		this->_pseudo = p._pseudo;
		this->_lastName = p._lastName;
		this->_firstName = p._firstName;
		this->_gender = p._gender;
		this->_isEliminated = p._isEliminated;
		this->_isMultiTeamPlayer = p._isMultiTeamPlayer;
	}

	return (*this);
}*/

/************/
/*	GETTER	*/
/************/

size_t				Participant::getId() const
{
	return (this->_id);
}

C_STRING			Participant::getPseudo() const
{
	return (this->_pseudo);
}

C_STRING			Participant::getLastName() const
{
	return (this->_lastName);
}

C_STRING			Participant::getFirstName() const
{
	return (this->_firstName);
}

Participant::Gender	Participant::getGenderInt() const
{
	return (this->_gender);
}

STRING				Participant::getGenderStr() const
{
	return (this->_gender == MALE ? "Homme" : this->_gender == FEMALE ? "Femme" : "Autre");
}

bool				Participant::getIsEliminated() const
{
	return (this->_isEliminated);
}

bool				Participant::getIsMultiTeamPlayer() const
{
	return (this->_isMultiTeamPlayer);
}

/************/
/*	SETTER	*/
/************/

void				Participant::setPseudo(C_STRING value)
{
	this->_pseudo = value;
	FormatUtils::capitalize(this->_pseudo);
}

void				Participant::setLastName(C_STRING value)
{
	this->_lastName = value;
	FormatUtils::toUpper(this->_lastName);
}

void				Participant::setFirstName(C_STRING value)
{
	this->_firstName = value;
	FormatUtils::capitalize(this->_firstName);
}

void				Participant::setGender(const Gender value)
{
	this->_gender = value;
}

void				Participant::setIsEliminated(const bool value)
{
	this->_isEliminated = value;
}

void				Participant::setIsMultiTeamPlayer(const bool value)
{
	this->_isMultiTeamPlayer = value;
}

/********************/
/*	PRIVATE METHODS	*/
/********************/

/********************/
/*	PUBLIC METHODS	*/
/********************/
