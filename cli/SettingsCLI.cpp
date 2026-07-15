//
// Created by Nicolas Fordoxcel on 23/06/2026.
//

//	STDLIB
#include <iostream>
#include <sstream>
#include <vector>

//	INCLUDES
#include "../includes/SettingsCLI.hpp"
#include "../includes/Constantes.hpp"

//	TYPEFED
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef std::vector<std::string>	V_STRING;
typedef const std::vector<int>&		CV_INT;

//	STATIC VARIABLES

/********************/
/*	PRIVATE METHOD	*/
/********************/

int									SettingsCLI::inputInt(C_STRING prompt, int min, int max, int defaultVal)
{
	STRING	input;
	int		val;

	while (true)
	{
		std::cout << prompt << " [" << defaultVal << "] (" << min << "-" << max << ") : ";
		std::getline(std::cin, input);

		if (input.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			return (defaultVal);

		std::stringstream ss(input);

		if (ss >> val && val >= min && val <= max)
			return (val);

		std::cout << "Saisie invalide. Entrez un nombre entre " << min << " et " << max << ".\n";
	}
}

bool								SettingsCLI::inputBool(C_STRING prompt, bool defaultVal)
{
	STRING	input;
	STRING	defStr = defaultVal ? "o" : "n";

	while (true)
	{
		std::cout << prompt << " [" << defStr << "] (o/n) : ";
		std::getline(std::cin, input);

		if (input.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			return (defaultVal);

		char c = tolower(input[0]);

		if (c == 'o')
			return (true);

		if (c == 'n')
			return (false);
		
		std::cout << "Veuillez repondre par 'o' ou 'n'.\n";
	}
}

STRING								SettingsCLI::inputString(C_STRING prompt, C_STRING defaultVal)
{
	STRING	input;

	while (true)
	{
		std::cout << prompt << " [" << defaultVal << "] : ";
		std::getline(std::cin, input);

		if (input.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			return (defaultVal);
			
		return (input);
	}
}

int									SettingsCLI::inputIntList(C_STRING prompt, CV_INT allowedValues, int defaultVal)
{
	STRING  input;
	int     val;

	while (true)
	{
		std::cout << prompt << " [" << defaultVal << "] (";
		for (size_t i = 0; i < allowedValues.size(); ++i) {
			std::cout << allowedValues[i] << (i < allowedValues.size() - 1 ? "/" : "");
		}
		std::cout << ") : ";
		
		std::getline(std::cin, input);

		if (input.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			return (defaultVal);

		std::stringstream ss(input);

		if (ss >> val)
		{
			for (int allowed : allowedValues)
			{
				if (val == allowed)
					return (val);
			}
		}

		std::cout << "[!] Saisie invalide. Valeurs autorisees : ";
		for (size_t i = 0; i < allowedValues.size(); ++i)
			std::cout << allowedValues[i] << (i < allowedValues.size() - 1 ? ", " : "\n");
	}
}

/********************/
/*	PUBLIC METHOD	*/
/********************/

void								SettingsCLI::setupWizard(Settings& s)
{
	V_STRING errors;

	std::cout << "\n--- CONFIGURATION DU TOURNOI ---" << std::endl;
	std::cout << "(Appuyez sur 'Entree' pour conserver la valeur entre crochets [])\n";

	do
	{
		if (!errors.empty())
		{
			std::cout << "\n[!] ECHEC DE LA VALIDATION FINALE :\n";

			for (const std::string& err : errors)
				std::cout << " - " << err << "\n";

			errors.clear();
			std::cout << "\n=== CORRECTION DES DONNEES ===\n";
		}

		s.setName(inputString("Nom du tournoi", s.getName()));
		
		std::cout << "\n> Structure des joueurs" << std::endl;
		s.setNbPlayers(inputInt("Nombre total de participants", MINPLAYERS, MAXPLAYERS, s.getNbPlayers()));
		s.setIsDouble(inputBool("Est-ce un tournoi en DOUBLE ?", s.getIsDouble()));
		s.setIsMixed(inputBool("Est-ce un tournoi MIXTE ?", s.getIsMixed()));
		s.setAllowMultiTeamPlayers(inputBool("Autoriser un joueur a completer une deuxieme equipe ?", s.getAllowMultiTeamPlayers()));

		std::cout << "\n> Structure des poules" << std::endl;

		bool isPoolLogicValid = false;
		
		while (!isPoolLogicValid)
		{
			s.setNbPools(inputIntList("Nombre de poules", {4, 8, 16}, s.getNbPools()));
			s.setNbPlayerByPool(inputInt("Nombre de joueurs/equipes par poule", NBPLAYERPERPOOLMIN, NBPLAYERPERPOOLMAX, s.getNbPlayerByPool()));

			int requieredPlayers = s.getNbPools() * s.getNbPlayerByPool();

			if (s.getNbPlayers() < requieredPlayers && !s.getAllowMultiTeamPlayers())
			{
				std::cout << "\n[!] ERREUR LOGIQUE : Vous avez declare " << s.getNbPlayers() << " joueurs.\n";
				std::cout << "Mais " << s.getNbPools() << " poules de " << s.getNbPlayerByPool() 
						<< " necessitent au moins " << requieredPlayers << " joueurs.\n";
				std::cout << "Veuillez reajuster vos poules.\n\n";
			}
			else
				isPoolLogicValid = true;
		}

		std::cout << "\n> Parametres de match (Badminton)" << std::endl;

		s.setNbBadmintonCourt(inputInt("Nombre de terrains disponibles", NBTERRAINMIN, NBTERRAINMAX, s.getNbBadmintonCourt()));
		
		bool isScoreLogicValid = false;

		while (!isScoreLogicValid)
		{
			s.setScoreMin(inputInt("Score pour gagner un set", SCOREMINTOWIN, SCOREMAXTOWIN, s.getScoreMin()));
			s.setScoreMax(inputInt("Score maximum en cas de prolongation", SCOREMINTOWIN, SCOREMAXTOWIN, s.getScoreMax()));
			s.setDiffPointsToWin(inputInt("Ecart de points necessaire", ECARTMIN, ECARTMAX, s.getDiffPointsToWin()));

			if (s.getScoreMin() > s.getScoreMax())
				std::cout << "\n[!] ERREUR LOGIQUE : Le score minimum (" << s.getScoreMin() 
						<< ") ne peut pas etre superieur au score maximum (" << s.getScoreMax() << ").\n\n";
			else
				isScoreLogicValid = true;
		}

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

	} while (!s.isValid(errors));
}

void								SettingsCLI::display(const Settings& s)
{
	std::cout << "\n========== CONFIGURATION ACTUELLE ==========" << std::endl;
	std::cout << "Nom : " << s.getName() << std::endl;
	std::cout << "Mode : " << (s.getIsDouble() ? "Double" : "Simple") << (s.getIsMixed() ? " Mixte" : " Open") << std::endl;
	std::cout << "Un joueur peut jouer dans plusieurs team : " << (s.getAllowMultiTeamPlayers() ? "Oui" : "Non") << std::endl;
	std::cout << "Joueurs : " << s.getNbPlayers() << " | Poules : " << s.getNbPools() << " (" << s.getNbPlayerByPool() << "/poule)" << std::endl;
	std::cout << "Terrains : " << s.getNbBadmintonCourt() << std::endl;
	std::cout << "Regles : " << s.getScoreMin() << " pts (Max " << s.getScoreMax() << "), Ecart: " << s.getDiffPointsToWin() << std::endl;
	std::cout << "Sets : Poules(" << s.getNbSetPlayedPools() << ") Quarts(" << s.getNbSetPlayedQuarters() << ") Demis(" << s.getNbSetPlayedSemis() << ") Finale(" << s.getNbSetPlayedFinal() << ")" << std::endl;
	std::cout << "Petite finale : " << (s.getIsThirdPlaceMatch() ? "Oui" : "Non") << std::endl;
	std::cout << "============================================" << std::endl;
}