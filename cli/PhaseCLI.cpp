//
// Created by Nicolas Fordoxcel on 22/07/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "../includes/class/Team.hpp"
#include "../includes/class/Match.hpp"
#include "../includes/class/Phase.hpp"

#include "../includes/viewer/MatchViewer.hpp"
#include "../includes/viewer/PhaseViewer.hpp"
#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/cli/CLIUtils.hpp"
#include "../includes/cli/MatchCLI.hpp"
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

/**
 * Affiche une phase si elle existe, sinon informe l utilisateur.
 */
/*void				PhaseCLI::handlePhase(Phase* phase, cString phaseName)
{
	if (!phase)
	{
		PrintUtils::addError(std::format("La phase '{}' n'existe pas ou n'est pas encore generee.", phaseName));
		return;
	}

	while (true)
	{
		CLIUtils::handleTitle(TitleViewer::tournament);
		PrintUtils::handleMessages();
		PrintUtils::printTitle(phaseName);

		auto matches = phase->getMatches();

		if (matches.empty())
		{
			std::cout << "Aucun match pour cette phase.\n";
			CLIUtils::waitForEnter();
			continue;
		}

		int count = 1;
		for (auto* m : matches)
		{
			if (m)
			{
				std::cout << "  " << std::setw(2) << count << ". ";
				MatchViewer::showMatchTitle(*m);
				std::cout << std::endl;
			}
			count++;
		}

		std::cout << "\n────────────────────────────────────────────────────────────\n";
		std::cout <<  Color::YELLOW << "\tR.\t" << Color::RESET << "Retour" << std::endl;
		std::cout << "============================================================\n";
		std::cout << "Votre choix : ";

		String input = CLIUtils::input();

		if (input.empty())
			continue;

		if (input == "r" || input == "R")
			return;

		auto choice = CLIUtils::parseInt(input);

		if (choice.has_value())
		{
			int idx = choice.value();
			if (idx >= 1 && idx <= static_cast<int>(matches.size()))
			{
				auto* m = matches[idx - 1];

				if (m)
					MatchCLI::submenuMatch(m);
			}
			else
				PrintUtils::addError("Numero de match invalide.");
		}
		else
		PrintUtils::addError("Saisie invalide. Entrez un numero ou R.");
	}
}*/

void				PhaseCLI::handleMenuPhase(Phase* phase, cString phaseName)
{
    if (!phase)
	    return (PrintUtils::addError(std::format("La phase '{}' n'est pas encore generee.", phaseName)));

	MatchCLI::handleMenuMatch(phase->getMatches(), phaseName);
}