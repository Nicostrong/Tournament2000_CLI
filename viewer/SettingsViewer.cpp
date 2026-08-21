//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <iostream>

#include "../includes/class/Settings.hpp"

#include "../includes/Color.hpp"

#include "../includes/viewer/TitleViewer.hpp"
#include "../includes/viewer/SettingsViewer.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

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


void				SettingsViewer::print(cSet s)
{
	TitleViewer::printSeparator(Color::YELLOW);
	std::cout << Color::YELLOW << "=                  SYNTHESE DES PARAMETRES                 =\n";
	TitleViewer::printSeparator(Color::YELLOW);

	std::cout << Color::BLUE << "[ TOURNOI ]\n" << Color::RESET;
    std::cout << "\tNom\t\t\t:\t" << s.getName() << "\n";
    std::cout << "\tFormat\t\t\t:\t" << (s.getIsDouble() ? "Double" : "Simple")
       << (s.getIsMixed() ? " (Mixte)" : (s.getTournamentGender() == Gender::MALE ? " (Homme)" : " (Femme)")) << "\n";
    std::cout << "\tJoueur multi-equipes\t:\t" << (s.getAllowMultiTeamPlayers() ? "Autorise" : "Interdit") << "\n\n";

	std::cout << Color::BLUE << "[ PARTICIPANTS & TERRAINS ]\n" << Color::RESET;
    std::cout << "\tNombre de participants\t:\t" << s.getNbPlayers() << "\n";
    std::cout << "\tPoules\t\t\t:\t" << s.getNbPools() << " poules de " << s.getNbPlayerByPool() << " joueurs/equipes\n";
    std::cout << "\tTerrains disponibles\t:\t" << s.getNbBadmintonCourt() << "\n\n";

	std::cout << Color::BLUE << "[ REGLES DU MATCH ]\n" << Color::RESET;
    std::cout << "\tScore de gain du set\t:\t" << s.getScoreMin() << " pts (Maximum : " << s.getScoreMax() << " pts)\n";
    std::cout << "\tEcart requis\t\t:\t" << s.getDiffPointsToWin() << " pt(s)\n\n";

	std::cout << Color::BLUE << "[ SETS PAR PHASE ]\n" << Color::RESET;
    std::cout << "\tPoules\t\t\t:\t" << s.getNbSetPlayedPools() << " set(s)\n";
    std::cout << "\t1/16e de Finale\t\t:\t" << s.getNbSetPlayedSixteenth() << " set(s)\n";
	std::cout << "\t1/8e de Finale\t\t:\t" << s.getNbSetPlayedHeigth() << " set(s)\n";
	std::cout << "\t1/4 de Finale\t\t:\t" << s.getNbSetPlayedQuarters() << " set(s)\n";
	std::cout << "\t1/2 Finale\t\t:\t" << s.getNbSetPlayedSemis() << " set(s)\n";
	std::cout << "\tFinale\t\t\t:\t" << s.getNbSetPlayedFinal() << " set(s)\n";
	if (s.getIsThirdPlaceMatch())
		std::cout << "\tPetite Finale (3e place):\t" << s.getNbSetPlayedThirdPlace() << " set(s)\n";
	else
		std::cout << "\tPetite Finale (3e place):\tNon jouee\n";

	TitleViewer::printSeparator(Color::YELLOW);

}