//
// Created by Nicolas Fordoxcel on 22/07/2026.
//

#ifndef PHASECLI_HPP
# define PHASECLI_HPP

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <fstream>

#include "../Global.hpp"

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
* Gere l affichage terminal et l export fichier d un objet Phase.
*/
class PhaseCLI
{

	private:

		static void					writeEncounter(std::ostream& out, cvpMatch matches,
									size_t startIdx, int nbSets, int encounterNum, bool toFile);

	public:

		static void					displayPhase(cPhase phase);
		static void					displayResults(cPhase phase);
		static bool					exportToTxt(cPhase phase, cString filename);

};

#endif
