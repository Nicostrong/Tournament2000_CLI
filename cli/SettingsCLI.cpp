//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

//	STDLIB
#include <iostream>
#include <sstream>
#include <vector>
#include <exception>

//	INCLUDES
#include "../includes/cli/SettingsCLI.hpp"
#include "../includes/Constantes.hpp"
#include "../includes/utils/PrintUtils.hpp"
#include "../includes/Global.hpp"

//	TYPEFED
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				V_STRING	=	std::vector<std::string>;
using				CV_INT		=	const std::vector<int>&;

//	STATIC VARIABLES

/****************/
/*	EXCEPTION	*/
/****************/

namespace
{
	struct	UserInterruptedException : public std::exception {};

	void checkInterruption()
	{
		if (!g_running || std::cin.eof())
			throw UserInterruptedException();
	}
}

/********************/
/*	PRIVATE METHOD	*/
/********************/

/**
 *	Lit l input de l utilisateur et en valide la valeur numerique
 */
int					SettingsCLI::inputInt(C_STRING prompt, const int min, const int max, const int defaultVal)
{
	STRING input;
	int val;

	while (true)
	{
		checkInterruption();
		std::cout << prompt << " [" << defaultVal << "] (" << min << "-" << max << ") : ";

		if (!std::getline(std::cin, input))
		{
			checkInterruption();
			return (defaultVal);
		}

		if (input.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			return (defaultVal);

		std::stringstream ss(input);

		if (ss >> val && val >= min && val <= max)
			return (val);

		std::cout << "Saisie invalide. Entrez un nombre entre " << min << " et " << max << ".\n";
	}
}

/**
 *	Lit l input de l utilisateur pour une question boolean et en valide la valeur
 */
bool				SettingsCLI::inputBool(C_STRING prompt, const bool defaultVal)
{
	STRING input;
	C_STRING defStr = defaultVal ? "o" : "n";

	while (true)
	{
		checkInterruption();
		std::cout << prompt << " [" << defStr << "] (o/n) : ";

		if (!std::getline(std::cin, input))
		{
			checkInterruption();
			return (defaultVal);
		}

		if (input.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			return (defaultVal);

		const char c = std::tolower(input[0]);

		if (c == 'o')
			return (true);

		if (c == 'n')
			return (false);

		std::cout << "Veuillez repondre par 'o' ou 'n'.\n";
	}
}

/**
 *	Lit l input de l utilisateur
 */
STRING				SettingsCLI::inputString(C_STRING prompt, C_STRING defaultVal)
{
	STRING input;

	while (true)
	{
		checkInterruption();
		std::cout << prompt << " [" << defaultVal << "] : ";

		if (!std::getline(std::cin, input))
		{
			checkInterruption();
			return (defaultVal);
		}

		if (input.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			return (defaultVal);

		return (input);
	}
}

/**
 *	Lit l input de l utilisateur et en valide la valeur d apres une liste predefinie
 */
int					SettingsCLI::inputIntList(C_STRING prompt, CV_INT allowedValues, int defaultVal)
{
	STRING input;
	int val;

	while (true)
	{
		checkInterruption();
		std::cout << prompt << " [" << defaultVal << "] (";

		for (size_t i = 0; i < allowedValues.size(); ++i)
			std::cout << allowedValues[i] << (i < allowedValues.size() - 1 ? "/" : "");

		std::cout << ") : ";

		if (!std::getline(std::cin, input))
		{
			checkInterruption();
			return (defaultVal);
		}

		if (input.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			return (defaultVal);

		std::stringstream ss(input);

		if (ss >> val)
			for (const int allowed : allowedValues)
				if (val == allowed)
					return (val);

		std::cout << "[!] Saisie invalide. Valeurs autorisees : ";

		for (size_t i = 0; i < allowedValues.size(); ++i)
			std::cout << allowedValues[i] << (i < allowedValues.size() - 1 ? ", " : "\n");
	}
}

void				SettingsCLI::setupPlayers(Settings& s)
{
	std::cout << "\n> Structure des joueurs" << std::endl;

	s.setNbPlayers(inputInt("Nombre total de participants", MINPLAYERS, MAXPLAYERS, s.getNbPlayers()));
	s.setIsDouble(inputBool("Est-ce un tournoi en DOUBLE ?", s.getIsDouble()));
	s.setIsMixed(inputBool("Est-ce un tournoi MIXTE ?", s.getIsMixed()));
	s.setAllowMultiTeamPlayers(inputBool("Autoriser un joueur a completer une deuxieme equipe ?", s.getAllowMultiTeamPlayers()));
}

void				SettingsCLI::setupPools(Settings& s)
{
	std::cout << "\n> Structure des poules" << std::endl;

	while (true)
	{
		s.setNbPools(inputIntList("Nombre de poules", {4, 8, 16}, s.getNbPools()));
		s.setNbPlayerByPool(inputInt("Nombre de joueurs/equipes par poule", NBPLAYERPERPOOLMIN, NBPLAYERPERPOOLMAX, s.getNbPlayerByPool()));

		const int requiredPlayers = s.getNbPools() * s.getNbPlayerByPool();

		if (s.getNbPlayers() < requiredPlayers && !s.getAllowMultiTeamPlayers())
		{
			std::cout << "\n[!] ERREUR LOGIQUE : Vous avez declare " << s.getNbPlayers() << " joueurs.\n";
			std::cout << "Mais " << s.getNbPools() << " poules de " << s.getNbPlayerByPool()
					<< " necessitent au moins " << requiredPlayers << " joueurs.\n";
			std::cout << "Veuillez reajuster vos poules.\n\n";
		}
		else
			break ;
	}
}

void				SettingsCLI::setupMatchRules(Settings& s)
{
	std::cout << "\n> Parametres de match (Badminton)" << std::endl;

	s.setNbBadmintonCourt(inputInt("Nombre de terrains disponibles", NBTERRAINMIN, NBTERRAINMAX, s.getNbBadmintonCourt()));

	while (true)
	{
		s.setScoreMin(inputInt("Score pour gagner un set", SCOREMINTOWIN, SCOREMAXTOWIN, s.getScoreMin()));
		s.setScoreMax(inputInt("Score maximum en cas de prolongation", SCOREMINTOWIN, SCOREMAXTOWIN, s.getScoreMax()));
		s.setDiffPointsToWin(inputInt("Ecart de points necessaire", ECARTMIN, ECARTMAX, s.getDiffPointsToWin()));

		if (s.getScoreMin() > s.getScoreMax())
		{
			std::cout << "\n[!] ERREUR LOGIQUE : Le score minimum (" << s.getScoreMin()
					<< ") ne peut pas etre superieur au score maximum (" << s.getScoreMax() << ").\n\n";
		}
		else
			break ;
	}
}

void				SettingsCLI::setupPhaseSets(Settings& s)
{
	std::cout << "\n> Nombre de sets par phase" << std::endl;

	s.setNbSetPlayedPools(inputInt("Sets en poules", NBSETPOOLMIN, NBSETPOOLMAX, s.getNbSetPlayedPools()));
	s.setNbSetPlayedSixteenth(inputInt("Sets en 1/16 (0 pour desactiver)", 0, NBSETSIXTEENTHMAX, s.getNbSetPlayedSixteenth()));
	s.setNbSetPlayedHeigth(inputInt("Sets en 1/8 (0 pour desactiver)", 0, NBSETHEIGTHMAX, s.getNbSetPlayedHeigth()));
	s.setNbSetPlayedQuarters(inputInt("Sets en Quarts (0 pour desactiver)", 0, NBSETQUARTERMAX, s.getNbSetPlayedQuarters()));
	s.setNbSetPlayedSemis(inputInt("Sets en Demis (0 pour desactiver)", 0, NBSETSEMIMAX, s.getNbSetPlayedSemis()));
	s.setNbSetPlayedFinal(inputInt("Sets en Finale", NBSETFINALMIN, NBSETFINALMAX, s.getNbSetPlayedFinal()));

	s.setIsThirdPlaceMatch(inputBool("Jouer la petite finale (3eme place) ?", s.getIsThirdPlaceMatch()));

	if (s.getIsThirdPlaceMatch())
		s.setNbSetPlayedThirdPlace(inputInt("Sets pour la 3e place", NBSETTHIRDMIN, NBSETTHIRDMAX, s.getNbSetPlayedThirdPlace()));
	else
		s.setNbSetPlayedThirdPlace(0);
}

/********************/
/*	PUBLIC METHOD	*/
/********************/

/**
 *	Initialisation des settings via les donnees utilisateur
 */
void				SettingsCLI::setupWizard(Settings& s)
{
	try
	{
		V_STRING errors;

		PrintUtils::setting();

		std::cout << "\n--- CONFIGURATION DU TOURNOI ---" << std::endl;
		std::cout << "(Appuyez sur 'Entree' pour conserver la valeur entre crochets [])\n";

		do
		{
			if (!errors.empty())
			{
				std::cout << "\n[!] ECHEC DE LA VALIDATION FINALE :\n";

				for (C_STRING err : errors)
					std::cout << " - " << err << "\n";

				errors.clear();
				std::cout << "\n=== CORRECTION DES DONNEES ===\n";
			}

			s.setName(inputString("Nom du tournoi", s.getName()));

			setupPlayers(s);
			setupPools(s);
			setupMatchRules(s);
			setupPhaseSets(s);

			std::cout << s << std::endl;

		} while (!s.isValid(errors));
	}
	catch (const UserInterruptedException&)
	{
		return ;
	}
}

/********************/
/*	PRINT METHOD	*/
/********************/

std::ostream&		operator<<(std::ostream& os, const Settings& s)
{
	os << "\n============= SETTING ACTUEL ===============" << std::endl;
	os << "Nom : " << s.getName() << std::endl;
	os << "Mode : " << (s.getIsDouble() ? "Double" : "Simple") << (s.getIsMixed() ? " Mixte" : " Open") << std::endl;
	os << "Un joueur peut jouer dans plusieurs team : " << (s.getAllowMultiTeamPlayers() ? "Oui" : "Non") << std::endl;
	os << "Nb. joueurs : " << s.getNbPlayers() << " | Nb. pools : " << s.getNbPools() << " (" << s.getNbPlayerByPool() << "/pool)" << std::endl;
	os << "Nb. terrains : " << s.getNbBadmintonCourt() << std::endl;
	os << "Regles : " << s.getScoreMin() << " pts (Max " << s.getScoreMax() << "), Ecart: " << s.getDiffPointsToWin() << std::endl;
	os << "Sets : Pool(" << s.getNbSetPlayedPools() << ") Quarts(" << s.getNbSetPlayedQuarters() << ") Demis(" << s.getNbSetPlayedSemis() << ") Finale(" << s.getNbSetPlayedFinal() << ")" << std::endl;
	os << "Petite finale : " << (s.getIsThirdPlaceMatch() ? "Oui" : "Non") << std::endl;
	os << "============================================" << std::endl;

	return (os);
}