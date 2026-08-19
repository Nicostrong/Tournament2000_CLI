//
// Created by Nicolas Fordoxcel on 13/08/2026.
//

#ifndef GLOBAL_HPP
# define GLOBAL_HPP

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <map>
# include <tuple>
# include <string>
# include <format>
# include <string>
# include <vector>
# include <csignal>

#include "./Color.hpp"



/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;

using				cString			=	const std::string&;
using				cInt			=	const int;
using				cBool			=	const bool;

template<std::size_t N>
using				aInt			=	std::array<int, N>;

using				vString			=	std::vector<std::string>;
using				vInt			=	std::vector<int>;

using				cvInt			=	const std::vector<int>;

using				vtupleMsg		=	std::vector<std::tuple<std::string, bool>>;

/****************************************************************************************************/
/*	CLASSES 																						*/
/****************************************************************************************************/

class Pool;
class Team;
class Match;
class Phase;
class Settings;
class Participant;
class Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cGender			=	const Gender&;

using				pPool			=	Pool*;
using				cPool			=	const Pool&;
using				cpPool			=	const Pool*;
using				vpPool			=	std::vector<Pool*>;
using				cvpPool			=	const std::vector<Pool*>&;

using				pTeam			=	Team*;
using				cTeam			=	const Team&;
using				cpTeam			=	const Team*;
using				vpTeam			=	std::vector<Team*>;
using				cvpTeam			=	const std::vector<Team*>&;

using				pMatch			=	Match*;
using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

using				pPhase			=	Phase*;
using				cPhase			=	const Phase&;
using				cpPhase			=	const Phase*;
using				vpPhase			=	std::vector<Phase*>;
using				cvpPhase		=	const std::vector<Phase*>&;

using				pSet			=	Settings*;
using				cSet			=	const Settings&;
using				cpSet			=	const Settings*;

using				pPart			=	Participant*;
using				cPart			=	const Participant&;
using				cpPart			=	const Participant*;
using				vpPart			=	std::vector<Participant*>;
using				cvpPart			=	const std::vector<Participant*>;

using				pTour			=	Tournament*;
using				cTour			=	const Tournament&;
using				cpTour			=	const Tournament*;
using				cvpPart			=	const std::vector<Participant*>&;

using				mcpPartvpMatch	=	std::map<const Participant*, std::vector<Match*>>;

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include "./Constantes.hpp"

# include "./class/Pool.hpp"
# include "./class/Team.hpp"
# include "./class/Match.hpp"
# include "./class/Phase.hpp"
# include "./class/Settings.hpp"
# include "./class/Tournament.hpp"
# include "./class/Participant.hpp"

# include "./utils/PrintUtils.hpp"
# include "./utils/FormatUtils.hpp"

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

inline volatile std::sig_atomic_t	g_running = 1;

inline vtupleMsg					PrintUtils::_messages;

#endif
