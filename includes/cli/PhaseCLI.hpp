//
// Created by Nicolas Fordoxcel on 22/07/2026.
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

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
* Gere l affichage terminal et l export fichier d un objet Phase.
*/
class				PhaseCLI
{

	private:

		static void					writeEncounter(std::ostream& out, cvpMatch matches,
									size_t startIdx, int nbSets, int encounterNum, bool toFile);

	public:

		static void					displayPhase(cPhase phase);
		static void					displayResults(cPhase phase);
		static bool					exportToTxt(cPhase phase, cString filename);

};
