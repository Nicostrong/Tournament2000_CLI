//
// Created by Nicolas Fordoxcel on 22/07/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <fstream>
#include <iomanip>

#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Phase.hpp"

#include "../includes/viewer/PhaseViewer.hpp"

#include "../includes/cli/PhaseCLI.hpp"

#include "../includes/utils/PrintUtils.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				cInt			=	const int;

using				cPhase			=	const Phase&;

using				cpTeam			=	const Team*;

using				cvpMatch		=	const std::vector<Match*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 * Exporte l historique complet d une phase dans un fichier texte.
 * Pas de codes ANSI dans le fichier (toFile = true).
 */
bool				PhaseCLI::exportToTxt(const Phase& phase, String filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
	{
		PrintUtils::addError(std::format("Impossible de creer le fichier : {}", filename));
		return (false);
	}

	cvpMatch matches = phase.getMatches();
	cInt nbSets = phase.getNbSetToPlay();

	file << "============================================================\n";
	file << "  PHASE : " << phase.getName() << "\n";
	file << "  Sets par rencontre : " << nbSets << "\n";
	file << "  Statut : " << (phase.isFinished() ? "Terminée" : "En cours") << "\n";
	file << "============================================================\n";

	if (matches.empty())
	{
		file << "\n  Aucun match enregistré.\n";
		file.close();
		return (true);
	}

	int encounterNum = 1;

	for (size_t i = 0; i < matches.size(); i += static_cast<size_t>(nbSets))
	{
		PhaseViewer::writeEncounter(file, matches, i, nbSets, encounterNum, true);
		file << "  " << String(50, '-') << "\n";
		++encounterNum;
	}

	if (phase.isFinished())
	{
		const vpTeam winners = phase.getWinners();
		const vpTeam losers = phase.getLosers();

		file << "\n============================================================\n";
		file << "  RÉSULTATS\n";
		file << "============================================================\n";

		file << "  Qualifiés :\n";

		for (size_t i = 0; i < winners.size(); ++i)
		{
			if (!winners[i])
				continue;

			file << "    " << (i + 1) << ". " << winners[i]->getName() << "\n";
		}

		if (!losers.empty())
		{
			file << "  Éliminés :\n";

			for (cpTeam t : losers)
			{
				if (!t)
					continue;

				file << "    - " << t->getName() << "\n";
			}
		}
	}

	file << "\n============================================================\n";
	file.close();

	return (true);
}
