//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "../includes/class/Settings.hpp"

#include "../includes/cli/CLIUtils.hpp"
#include "../includes/cli/SettingsCLI.hpp"

#include "../includes/viewer/SettingsViewer.hpp"
#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/SettingsChecker.hpp"

#include "../includes/Color.hpp"
#include "../includes/Constantes.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;
using				vString			=	std::vector<std::string>;

using				cInt			=	const int;
using				vInt			=	std::vector<int>;
using				cvInt			=	const std::vector<int>&;

using				cBool			=	const bool;

using				cSet			=	const Settings&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/**
 *	Setting de la structure des joueurs
 */
void				SettingsCLI::setupPlayers(Settings& s)
{
	std::cout << Color::BLUE << "\n> Structure des joueurs" << std::endl << Color::RESET;

	s.setIsDouble(CLIUtils::askBool("Est-ce un tournoi en DOUBLE ?", s.getIsDouble()));
	s.setIsMixed(CLIUtils::askBool("Est-ce un tournoi MIXTE ?", s.getIsMixed()));

	if (!s.getIsMixed())
	{
		cInt defaultG = (s.getTournamentGender() == Gender::FEMALE) ? 1 : 0;
		cInt g = CLIUtils::askInt("Genre du tournoi (0 = HOMME, 1 = FEMME)", 0, 1, defaultG);

		s.setTournamentGender(g == 0 ? Gender::MALE : Gender::FEMALE);
	}
	else
		s.setTournamentGender(Gender::MIXED);

	vInt allowedPlayers;

	if (s.getIsDouble())
		allowedPlayers.assign(allowedNbPlayersDouble.begin(), allowedNbPlayersDouble.end());
	else
		allowedPlayers.assign(allowedNbPlayersSimple.begin(), allowedNbPlayersSimple.end());

	int defaultVal = s.getNbPlayers();

	if (std::ranges::find(allowedPlayers, defaultVal) == allowedPlayers.end())
		defaultVal = allowedPlayers[0];

	s.setNbPlayers(CLIUtils::askIntList("Nombre total de participants", allowedPlayers, defaultVal));
	s.setAllowMultiTeamPlayers(CLIUtils::askBool("Autoriser un joueur a completer une deuxieme equipe ?", s.getAllowMultiTeamPlayers()));
}

/**
 *	Setting de la structure des pools
 */
void				SettingsCLI::setupPools(Settings& s)
{
	std::cout << Color::BLUE << "\n> Structure des pools" << std::endl << Color::RESET;

	while (true)
	{
		s.setNbPools(CLIUtils::askIntList("Nombre de poules", {4, 8, 16}, s.getNbPools()));
		s.setNbPlayerByPool(CLIUtils::askInt("Nombre de joueurs/equipes par poule", NBPLAYERPERPOOLMIN, NBPLAYERPERPOOLMAX, s.getNbPlayerByPool()));

		if (!SettingsChecker::isPoolMathConsistent(s.getNbPlayers(), s.getIsDouble(), s.getNbPools(), s.getNbPlayerByPool()) && !s.getAllowMultiTeamPlayers())
		{
			std::cout << Color::RED << "\n[!] ERREUR LOGIQUE : La repartition choisie ne correspond pas au total de joueurs declares.\n";
			std::cout << "Veuillez reajuster vos poules.\n\n" << Color::RESET;
		}
		else
			break;
	}
}

/**
 *	Setting des parametres des matchs
 */
void				SettingsCLI::setupMatchRules(Settings& s)
{
	std::cout << Color::BLUE << "\n> Parametres de match (Badminton)" << std::endl << Color::RESET;

	s.setNbBadmintonCourt(CLIUtils::askInt("Nombre de terrains disponibles", NBTERRAINMIN, NBTERRAINMAX, s.getNbBadmintonCourt()));

	while (true)
	{
		s.setScoreMin(CLIUtils::askInt("Score pour gagner un set", SCOREMINTOWIN, SCOREMAXTOWIN, s.getScoreMin()));
		s.setScoreMax(CLIUtils::askInt("Score maximum en cas de prolongation", SCOREMINTOWIN, SCOREMAXTOWIN, s.getScoreMax()));
		s.setDiffPointsToWin(CLIUtils::askInt("Ecart de points necessaire", ECARTMIN, ECARTMAX, s.getDiffPointsToWin()));

		if (s.getScoreMin() > s.getScoreMax())
		{
			std::cout << Color::RED << "\n[!] ERREUR LOGIQUE : Le score minimum (" << s.getScoreMin()
					<< ") ne peut pas etre superieur au score maximum (" << s.getScoreMax() << ").\n\n" << Color::RESET;
		}
		else
			break;
	}
}

/**
 *	Setting du nombre de set des differentes phases
 */
void				SettingsCLI::setupPhaseSets(Settings& s)
{
	std::cout << Color::BLUE << "\n> Nombre de sets par phase" << std::endl << Color::RESET;

	s.setNbSetPlayedPools(CLIUtils::askInt("Sets en poules", NBSETPOOLMIN, NBSETPOOLMAX, s.getNbSetPlayedPools()));
	s.setNbSetPlayedSixteenth(CLIUtils::askInt("Sets en 1/16", NBSETSIXTEENTHMIN, NBSETSIXTEENTHMAX, s.getNbSetPlayedSixteenth()));
	s.setNbSetPlayedHeigth(CLIUtils::askInt("Sets en 1/8", NBSETHEIGTHMIN, NBSETHEIGTHMAX, s.getNbSetPlayedHeigth()));
	s.setNbSetPlayedQuarters(CLIUtils::askInt("Sets en Quarts", NBSETQUARTERMIN, NBSETQUARTERMAX, s.getNbSetPlayedQuarters()));
	s.setNbSetPlayedSemis(CLIUtils::askInt("Sets en Demis", NBSETSEMIMIN, NBSETSEMIMAX, s.getNbSetPlayedSemis()));
	s.setNbSetPlayedFinal(CLIUtils::askInt("Sets en Finale", NBSETFINALMIN, NBSETFINALMAX, s.getNbSetPlayedFinal()));

	s.setIsThirdPlaceMatch(CLIUtils::askBool("Jouer la petite finale (3eme place) ?", s.getIsThirdPlaceMatch()));

	if (s.getIsThirdPlaceMatch())
		s.setNbSetPlayedThirdPlace(CLIUtils::askInt("Sets pour la 3e place", NBSETTHIRDMIN, NBSETTHIRDMAX, s.getNbSetPlayedThirdPlace()));
	else
		s.setNbSetPlayedThirdPlace(0);
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 *	Initialisation des settings via les donnees utilisateur
 */
void				SettingsCLI::setupWizard(Settings& s)
{
	try
	{
		vString errors;
		bool confirmed = false;

		while (!confirmed)
		{
			PrintUtils::clear();
			TitleViewer::banner();
			TitleViewer::setting();

			if (!errors.empty())
			{
				std::cout << Color::RED << "\n[!] ECHEC DE LA VALIDATION FINALE :\n";

				for (cString err : errors)
					std::cout << " - " << err << "\n";

				errors.clear();
				std::cout << "\n=== CORRECTION DES DONNEES ===\n" << Color::RESET;
			}

			std::cout << std::endl << Color::YELLOW << "(Appuyez sur 'Entree' pour conserver la valeur entre crochets [])\n" << Color::RESET;
			s.setName(CLIUtils::askString("Nom du tournoi", s.getName()));


			setupPlayers(s);
			setupPools(s);
			setupMatchRules(s);
			setupPhaseSets(s);

			SettingsViewer::print(s);

			if (!SettingsChecker::isValid(s, errors))
				continue;

			if (CLIUtils::askBool("Validez-vous ces parametres pour passer a l'etape suivante ?", true))
				confirmed = true;
		}
	}
	catch (const CLIInterrupted&)
	{
		return;
	}
}
