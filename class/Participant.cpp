//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

//	STDLIB
#include <string>
#include <vector>

//	INCLUDES
#include "../includes/class/Pool.hpp"
#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Phase.hpp"
#include "../includes/class/Settings.hpp"
#include "../includes/class/Tournament.hpp"
#include "../includes/class/Participant.hpp"

#include "../includes/utils/FormatUtils.hpp"

//	TYPEDEF
using				string		=	std::string;
using				cString		=	const std::string&;

using				cPool		=	const Pool&;
using				cTeam		=	const Team&;
using				cMatch		=	const Match&;
using				cPhase		=	const Phase&;
using				cSet		=	const Settings&;
using				cPart		=	const Participant&;

using				vpPool		=	std::vector<Pool*>;
using				vpTeam		=	std::vector<Team*>;
using				vpMatch		=	std::vector<Match*>;
using				vpPhase		=	std::vector<Phase*>;
using				vpPart		=	std::vector<Participant*>;

using				cvpPool		=	const std::vector<Pool*>&;
using				cvpTeam		=	const std::vector<Team*>&;
using				cvpMatch	=	const std::vector<Match*>&;
using				cvpPhase	=	const std::vector<Phase*>&;
using				cvpPart		=	const std::vector<Participant*>&;

//	STATIC VARIABLES
int					Participant::_idCounter = 0;

/************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																	*/
/************************************************************************************************/

Participant::Participant(cString pseudo, cString lastName, cString firstName, const Gender gender):
	_id(_idCounter++), _pseudo(pseudo), _lastName(lastName), _firstName(firstName), _gender(gender),
	_isEliminated(false), _isMultiTeamPlayer(false)
{
	FormatUtils::trimAndCapitalize(this->_pseudo);
	FormatUtils::trimAndCapitalize(this->_lastName);
	FormatUtils::trimAndCapitalize(this->_firstName);
}

/************************************************************************************************/
/*	GETTER																						*/
/************************************************************************************************/

size_t				Participant::getId() const					{	return (this->_id);																			}
cString				Participant::getPseudo() const				{	return (this->_pseudo);																		}
cString				Participant::getLastName() const			{	return (this->_lastName);																	}
cString				Participant::getFirstName() const			{	return (this->_firstName);																	}
Participant::Gender	Participant::getGenderInt() const			{	return (this->_gender);																		}
string				Participant::getGenderStr() const			{	return (this->_gender == MALE ? "Homme" : this->_gender == FEMALE ? "Femme" : "Mixte");		}
bool				Participant::getIsEliminated() const		{	return (this->_isEliminated);																}
bool				Participant::getIsMultiTeamPlayer() const	{	return (this->_isMultiTeamPlayer);															}

/************************************************************************************************/
/*	SETTER																						*/
/************************************************************************************************/

void				Participant::setPseudo(cString value)				{	this->_pseudo = value; FormatUtils::capitalize(this->_pseudo);			}
void				Participant::setLastName(cString value)				{	this->_lastName = value; FormatUtils::toUpper(this->_lastName);			}
void				Participant::setFirstName(cString value)			{	this->_firstName = value; FormatUtils::capitalize(this->_firstName);	}
void				Participant::setGender(const Gender value)			{	this->_gender = value;													}
void				Participant::setIsEliminated(const bool value)		{	this->_isEliminated = value;											}
void				Participant::setIsMultiTeamPlayer(const bool value)	{	this->_isMultiTeamPlayer = value;										}

/************************************************************************************************/
/*	PRIVATE METHODS																				*/
/************************************************************************************************/

/************************************************************************************************/
/*	PUBLIC METHODS																				*/
/************************************************************************************************/
