//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include "../includes/class/Player.hpp"

#include "../includes/utils/FormatUtils.hpp"

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

int					Player::_idCounter = 0;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Player::Player(String pseudo, String lastName, String firstName, Gender gender)
	: _id(_idCounter++), _pseudo(pseudo), _lastName(lastName), _firstName(firstName),
	_gender(gender), _isEliminated(false), _isMultiTeamPlayer(false)
{
	FormatUtils::trimAndCapitalize(this->_pseudo);
	FormatUtils::trimAndCapitalize(this->_lastName);
	FormatUtils::trimAndCapitalize(this->_firstName);
}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

size_t				Player::getId() const					{	return (this->_id);									}
cString				Player::getPseudo() const				{	return (this->_pseudo);								}
cString				Player::getLastName() const				{	return (this->_lastName);							}
cString				Player::getFirstName() const			{	return (this->_firstName);							}
String				Player::getFullName() const				{	return (this->_firstName + " " + this->_lastName);	}
Gender				Player::getGenderInt() const			{	return (this->_gender);								}
bool				Player::getIsEliminated() const			{	return (this->_isEliminated);						}
bool				Player::getIsMultiTeamPlayer() const	{	return (this->_isMultiTeamPlayer);					}


String				Player::getGenderStr() const
{
	switch (this->_gender)
	{
		case MALE:
			return ("Homme");

		case FEMALE:
			return ("Femme");

		default:
			return ("Mixte");
	}
}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				Player::setPseudo(String value)				{	this->_pseudo = value; FormatUtils::capitalize(this->_pseudo);		}
void				Player::setLastName(String value)			{	this->_lastName = value; FormatUtils::toUpper(this->_lastName);		}
void				Player::setFirstName(String value)			{	this->_firstName = value; FormatUtils::capitalize(this->_firstName);	}
void				Player::setGender(Gender value)				{	this->_gender = value;													}
void				Player::setIsEliminated(cBool value)		{	this->_isEliminated = value;											}
void				Player::setIsMultiTeamPlayer(cBool value)	{	this->_isMultiTeamPlayer = value;										}

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHODS																					*/
/****************************************************************************************************/

bool				Player::operator==(const Player& other) const
{
	return (this->_id == other._id);
}