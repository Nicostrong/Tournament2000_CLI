//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <sstream>
#include <iostream>
#include <exception>
#include <algorithm>

#include "../includes/class/Settings.hpp"

#include "../includes/cli/SettingsCLI.hpp"

#include "../includes/viewer/TitleViewer.hpp"

#include "../includes/utils/PrintUtils.hpp"

#include "../includes/Color.hpp"
#include "../includes/Constantes.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	EXCEPTION																						*/
/****************************************************************************************************/

namespace
{
	struct	UserInterruptedException : public std::exception {};

	void checkInterruption()
	{
		if (!g_running || std::cin.eof())
			throw UserInterruptedException();
	}
}

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

/**
 *	Lit l input de l utilisateur et en valide la valeur numerique
 */
int					SettingsCLI::inputInt(cString prompt, cInt min, cInt max, cInt defaultVal)
{
	String input;
	int val;

	while (true)
	{
		checkInterruption();
		std::cout << prompt << " [" << Color::YELLOW << defaultVal << Color::RESET << "] (" << min << "-" << max << ") : ";

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

		std::cout << Color::RED << "Saisie invalide. Entrez un nombre entre " << min << " et " << max << ".\n" << Color::RESET;
	}
}

/**
 *	Lit l input de l utilisateur pour une question boolean et en valide la valeur
 */
bool				SettingsCLI::inputBool(cString prompt, cBool defaultVal)
{
	String input;
	cString defStr = defaultVal ? "o" : "n";

	while (true)
	{
		checkInterruption();
		std::cout << prompt << " [" << Color::YELLOW << defStr << Color::RESET << "] (o/n) : ";

		if (!std::getline(std::cin, input))
		{
			checkInterruption();
			return (defaultVal);
		}

		if (input.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			return (defaultVal);

		const unsigned char c = std::tolower(input[0]);

		if (c == 'o')
			return (true);

		if (c == 'n')
			return (false);

		std::cout << Color::RED << "Veuillez repondre par 'o' ou 'n'.\n" << Color::RESET;
	}
}

/**
 *	Lit l input de l utilisateur
 */
String				SettingsCLI::inputString(cString prompt, cString defaultVal)
{
	String input;

	while (true)
	{
		checkInterruption();
		std::cout << prompt << " [" << Color::YELLOW << defaultVal << Color::RESET << "] : ";

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
int					SettingsCLI::inputIntList(cString prompt, cvInt allowedValues, int defaultVal)
{
	String input;
	int val;

	while (true)
	{
		checkInterruption();
		std::cout << prompt << " [" << Color::YELLOW << defaultVal << Color::RESET << "] (";

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

		std::cout << Color::RED << "[!] Saisie invalide. Valeurs autorisees : " << Color::RESET;

		for (size_t i = 0; i < allowedValues.size(); ++i)
			std::cout << Color::GREEN << allowedValues[i] << (i < allowedValues.size() - 1 ? ", " : "\n") << Color::RESET;
	}
}

/**
 *	Setting de la structure des joueurs
 */
void				SettingsCLI::setupPlayers(pSet s)
{
	std::cout << Color::BLUE << "\n> Structure des joueurs" << std::endl << Color::RESET;

	s->setIsDouble(inputBool("Est-ce un tournoi en DOUBLE ?", s->getIsDouble()));
	s->setIsMixed(inputBool("Est-ce un tournoi MIXTE ?", s->getIsMixed()));

	if (!s->getIsMixed())
	{
		cInt defaultG = (s->getTournamentGender() == Gender::FEMALE) ? 1 : 0;
		cInt g = inputInt("Genre du tournoi (0 = HOMME, 1 = FEMME)", 0, 1, defaultG);

		s->setTournamentGender(g == 0 ? Gender::MALE : Gender::FEMALE);
	}
	else
		s->setTournamentGender(Gender::MIXED);

	vInt allowedPlayers;

	if (s->getIsDouble())
		allowedPlayers.assign(allowedNbPlayersDouble.begin(), allowedNbPlayersDouble.end());
	else
		allowedPlayers.assign(allowedNbPlayersSimple.begin(), allowedNbPlayersSimple.end());

	int defaultVal = s->getNbPlayers();

	if (std::ranges::find(allowedPlayers, defaultVal) == allowedPlayers.end())
		defaultVal = allowedPlayers[0];

	s->setNbPlayers(inputIntList("Nombre total de participants", allowedPlayers, defaultVal));
	s->setAllowMultiTeamPlayers(inputBool("Autoriser un joueur a completer une deuxieme equipe ?", s->getAllowMultiTeamPlayers()));
}

/**
 *	Setting de la structure des pools
 */
void				SettingsCLI::setupPools(pSet s)
{
	std::cout << Color::BLUE << "\n> Structure des pools" << std::endl << Color::RESET;

	while (true)
	{
		s->setNbPools(inputIntList("Nombre de poules", {4, 8, 16}, s->getNbPools()));
		s->setNbPlayerByPool(inputInt("Nombre de joueurs/equipes par poule", NBPLAYERPERPOOLMIN, NBPLAYERPERPOOLMAX, s->getNbPlayerByPool()));

		cInt requiredPlayers = s->getNbPools() * s->getNbPlayerByPool();

		if (s->getNbPlayers() < requiredPlayers && !s->getAllowMultiTeamPlayers())
		{
			std::cout << Color::RED << "\n[!] ERREUR LOGIQUE : Vous avez declare " << s->getNbPlayers() << " joueurs.\n";
			std::cout << "Mais " << s->getNbPools() << " poules de " << s->getNbPlayerByPool()
					<< " necessitent au moins " << requiredPlayers << " joueurs.\n";
			std::cout << "Veuillez reajuster vos poules.\n\n" << Color::RESET;
		}
		else
			break;
	}
}

/**
 *	Setting des parametres des matchs
 */
void				SettingsCLI::setupMatchRules(pSet s)
{
	std::cout << Color::BLUE << "\n> Parametres de match (Badminton)" << std::endl << Color::RESET;

	s->setNbBadmintonCourt(inputInt("Nombre de terrains disponibles", NBTERRAINMIN, NBTERRAINMAX, s->getNbBadmintonCourt()));

	while (true)
	{
		s->setScoreMin(inputInt("Score pour gagner un set", SCOREMINTOWIN, SCOREMAXTOWIN, s->getScoreMin()));
		s->setScoreMax(inputInt("Score maximum en cas de prolongation", SCOREMINTOWIN, SCOREMAXTOWIN, s->getScoreMax()));
		s->setDiffPointsToWin(inputInt("Ecart de points necessaire", ECARTMIN, ECARTMAX, s->getDiffPointsToWin()));

		if (s->getScoreMin() > s->getScoreMax())
		{
			std::cout << Color::RED << "\n[!] ERREUR LOGIQUE : Le score minimum (" << s->getScoreMin()
					<< ") ne peut pas etre superieur au score maximum (" << s->getScoreMax() << ").\n\n" << Color::RESET;
		}
		else
			break;
	}
}

/**
 *	Setting du nombre de set des differentes phases
 */
void				SettingsCLI::setupPhaseSets(pSet s)
{
	std::cout << Color::BLUE << "\n> Nombre de sets par phase" << std::endl << Color::RESET;

	s->setNbSetPlayedPools(inputInt("Sets en poules", NBSETPOOLMIN, NBSETPOOLMAX, s->getNbSetPlayedPools()));
	s->setNbSetPlayedSixteenth(inputInt("Sets en 1/16", NBSETSIXTEENTHMIN, NBSETSIXTEENTHMAX, s->getNbSetPlayedSixteenth()));
	s->setNbSetPlayedHeigth(inputInt("Sets en 1/8", NBSETHEIGTHMIN, NBSETHEIGTHMAX, s->getNbSetPlayedHeigth()));
	s->setNbSetPlayedQuarters(inputInt("Sets en Quarts", NBSETQUARTERMIN, NBSETQUARTERMAX, s->getNbSetPlayedQuarters()));
	s->setNbSetPlayedSemis(inputInt("Sets en Demis", NBSETSEMIMIN, NBSETSEMIMAX, s->getNbSetPlayedSemis()));
	s->setNbSetPlayedFinal(inputInt("Sets en Finale", NBSETFINALMIN, NBSETFINALMAX, s->getNbSetPlayedFinal()));

	s->setIsThirdPlaceMatch(inputBool("Jouer la petite finale (3eme place) ?", s->getIsThirdPlaceMatch()));

	if (s->getIsThirdPlaceMatch())
		s->setNbSetPlayedThirdPlace(inputInt("Sets pour la 3e place", NBSETTHIRDMIN, NBSETTHIRDMAX, s->getNbSetPlayedThirdPlace()));
	else
		s->setNbSetPlayedThirdPlace(0);
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

		std::cout << Color::BLINK << Color::YELLOW << "(Appuyez sur 'Entree' pour conserver la valeur entre crochets [])\n" << Color::RESET;

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

			s.setName(inputString("Nom du tournoi", s.getName()));

			setupPlayers(&s);
			setupPools(&s);
			setupMatchRules(&s);
			setupPhaseSets(&s);

			std::cout << s << std::endl;

			if (!s.isValid(errors))
				continue;

			if (inputBool("Validez-vous ces parametres pour passer a l'etape suivante ?", true))
				confirmed = true;
		}
	}
	catch (const UserInterruptedException&)
	{
		return;
	}
}

/********************/
/*	PRINT METHOD	*/
/********************/

std::ostream&		operator<<(std::ostream& os, cSet s)
{
	os << Color::YELLOW;
	os << "\n============================================================\n";
    os << "=                  SYNTHESE DES PARAMETRES                 =\n";
    os << "============================================================\n";

	os << Color::BLUE << "[ TOURNOI ]\n" << Color::RESET;
    os << "\tNom\t\t\t:\t" << s.getName() << "\n";
    os << "\tFormat\t\t\t:\t" << (s.getIsDouble() ? "Double" : "Simple")
       << (s.getIsMixed() ? " (Mixte)" : (s.getTournamentGender() == Gender::MALE ? " (Homme)" : " (Femme)")) << "\n";
    os << "\tJoueur multi-equipes\t:\t" << (s.getAllowMultiTeamPlayers() ? "Autorise" : "Interdit") << "\n\n";

	os << Color::BLUE << "[ PARTICIPANTS & TERRAINS ]\n" << Color::RESET;
    os << "\tNombre de participants\t:\t" << s.getNbPlayers() << "\n";
    os << "\tPoules\t\t\t:\t" << s.getNbPools() << " poules de " << s.getNbPlayerByPool() << " joueurs/equipes\n";
    os << "\tTerrains disponibles\t:\t" << s.getNbBadmintonCourt() << "\n\n";

	os << Color::BLUE << "[ REGLES DU MATCH ]\n" << Color::RESET;
    os << "\tScore de gain du set\t:\t" << s.getScoreMin() << " pts (Maximum : " << s.getScoreMax() << " pts)\n";
    os << "\tEcart requis\t\t:\t" << s.getDiffPointsToWin() << " pt(s)\n\n";

	os << Color::BLUE << "[ SETS PAR PHASE ]\n" << Color::RESET;
    os << "\tPoules\t\t\t:\t" << s.getNbSetPlayedPools() << " set(s)\n";
    os << "\t1/16e de Finale\t\t:\t" << s.getNbSetPlayedSixteenth() << " set(s)\n";
	os << "\t1/8e de Finale\t\t:\t" << s.getNbSetPlayedHeigth() << " set(s)\n";
	os << "\t1/4 de Finale\t\t:\t" << s.getNbSetPlayedQuarters() << " set(s)\n";
	os << "\t1/2 Finale\t\t:\t" << s.getNbSetPlayedSemis() << " set(s)\n";
	os << "\tFinale\t\t\t:\t" << s.getNbSetPlayedFinal() << " set(s)\n";
	if (s.getIsThirdPlaceMatch())
		os << "\tPetite Finale (3e place):\t" << s.getNbSetPlayedThirdPlace() << " set(s)\n";
	else
		os << "\tPetite Finale (3e place):\tNon jouee\n";

    os << "====================================================";

	return (os);
}