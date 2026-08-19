//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <vector>

#include "../includes/class/Participant.hpp"

#include "../includes/utils/FormatUtils.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;
using				vtupleMsg		=	std::vector<std::tuple<std::string, bool>>;

using				cInt			=	const int;
using				vInt			=	std::vector<int>;
using				cvInt			=	const std::vector<int>;
template<std::size_t N>
using				aInt			=	std::array<int, N>;

using				cBool			=	const bool;

using				cPart			=	const Participant&;
using				cpPart			=	const Participant*;
using				vpPart			=	std::vector<Participant*>;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

int					Participant::_idCounter = 0;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

Participant::Participant(cString pseudo, cString lastName, cString firstName, cGender gender):
	_id(_idCounter++), _pseudo(pseudo), _lastName(lastName), _firstName(firstName), _gender(gender),
	_isEliminated(false), _isMultiTeamPlayer(false)
{
	FormatUtils::trimAndCapitalize(this->_pseudo);
	FormatUtils::trimAndCapitalize(this->_lastName);
	FormatUtils::trimAndCapitalize(this->_firstName);
}

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

size_t				Participant::getId() const					{	return (this->_id);																			}
cString				Participant::getPseudo() const				{	return (this->_pseudo);																		}
cString				Participant::getLastName() const			{	return (this->_lastName);																	}
cString				Participant::getFirstName() const			{	return (this->_firstName);																	}
Gender				Participant::getGenderInt() const			{	return (this->_gender);																		}
String				Participant::getGenderStr() const			{	return (this->_gender == MALE ? "Homme" : this->_gender == FEMALE ? "Femme" : "Mixte");		}
bool				Participant::getIsEliminated() const		{	return (this->_isEliminated);																}
bool				Participant::getIsMultiTeamPlayer() const	{	return (this->_isMultiTeamPlayer);															}

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

void				Participant::setPseudo(cString value)			{	this->_pseudo = value; FormatUtils::capitalize(this->_pseudo);			}
void				Participant::setLastName(cString value)			{	this->_lastName = value; FormatUtils::toUpper(this->_lastName);			}
void				Participant::setFirstName(cString value)		{	this->_firstName = value; FormatUtils::capitalize(this->_firstName);	}
void				Participant::setGender(cGender value)			{	this->_gender = value;													}
void				Participant::setIsEliminated(cBool value)		{	this->_isEliminated = value;											}
void				Participant::setIsMultiTeamPlayer(cBool value)	{	this->_isMultiTeamPlayer = value;										}

/****************************************************************************************************/
/*	PRIVATE METHODS																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHODS																					*/
/****************************************************************************************************/
