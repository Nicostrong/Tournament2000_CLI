//
// Created by Nicolas Fordoxcel on 22/07/2026.
//

#ifndef PHASECLI_HPP
# define PHASECLI_HPP

//	STDLIB
# include <string>
# include <fstream>

//	INCLUDES
# include "../class/Phase.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				CVP_MATCH	=	const std::vector<Match*>&;

/**
* Gere l affichage terminal et l export fichier d un objet Phase.
*/
class PhaseCLI
{

	private:

		static void					writeEncounter(std::ostream& out, CVP_MATCH matches,
									size_t startIdx, int nbSets, int encounterNum, bool toFile);

	public:

		static void					displayPhase(const Phase& phase);
		static void					displayResults(const Phase& phase);
		static bool					exportToTxt(const Phase& phase, C_STRING filename);

};
#endif //PHASECLI_HPP
