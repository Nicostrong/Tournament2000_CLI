//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>
# include <fstream>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Phase;
class				Match;
class				Tournament;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;


using				pPhase			=	Phase*;
using				cPhase			=	const Phase&;
using				cpPhase			=	const Phase*;
using				vpPhase			=	std::vector<Phase*>;
using				cvpPhase		=	const std::vector<Phase*>&;

using				cMatch			=	const Match&;
using				cpMatch			=	const Match*;
using				vpMatch			=	std::vector<Match*>;
using				cvpMatch		=	const std::vector<Match*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

class				PhaseViewer
{
	public:

		static void					writeEncounter(std::ostream& out, cvpMatch matches,
									size_t startIdx, int nbSets, int encounterNum, bool toFile);
		static void					displayPhase(cPhase phase);
		static void					displayResults(cPhase phase);

		static void					printAll(Tournament& tournament);

};

