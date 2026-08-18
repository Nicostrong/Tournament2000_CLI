//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

//	STDLIB
#include <limits>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <exception>

//	INCLUDES
#include "../includes/Color.hpp"
#include "../includes/Global.hpp"
#include "../includes/cli/PoolCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"
#include "../includes/cli/PhaseCLI.hpp"
#include "../includes/utils/Exporter.hpp"
#include "../includes/cli/SettingsCLI.hpp"
#include "../includes/utils/PrintUtils.hpp"
#include "../includes/cli/TournamentCLI.hpp"
#include "../includes/cli/TournamentViewer.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VP_POOL		=	std::vector<Pool*>;
using				VP_MATCH	=	std::vector<Match*>;
using				CVP_MATCH	=	const std::vector<Match*>&;
using               CVP_TEAM	=	const std::vector<Team*>&;

//	STATIC VARIABLES

/**************************************************************************************************/
/*	EXCEPTION																					  */
/**************************************************************************************************/

namespace
{
	struct	UserInterruptedException : public std::exception {};

	void checkInterruption()
	{
		if (!g_running || std::cin.eof())
			throw UserInterruptedException();
	}
}

/**************************************************************************************************/
/*	PRIVATE METHOD																				  */
/**************************************************************************************************/

/************************/
/*  GESTION DU MENU		*/
/************************/

/**
 *	Gestion de l affichage des menu
 */
void				TournamentCLI::displayMenuUI(const Tournament& tournament)
{
	handleTitle();
    PrintUtils::handleMessages();
    menuTournament(tournament);
    checkInterruption();
}

/**
 *	gere l affichage des titres
 */
void				TournamentCLI::handleTitle()
{
	PrintUtils::clear();
	PrintUtils::banner();
	PrintUtils::tournament();
}

/**
 *	Affiche les menus sous conditions du tournoi
 */
void				TournamentCLI::menuTournament(const Tournament& tournament)
{
	std::cout << "\n========== TOURNOI : " << tournament.getSettings().getName() << " ==========" << std::endl;
	std::cout <<  Color::YELLOW << "\t1.\t" << Color::RESET << "Pools" << std::endl;
	std::cout <<  Color::YELLOW << "\t2.\t" << Color::RESET << "Teams" << std::endl;

	if (tournament.getHasSixteenth() && isSixteenthUnlocked(tournament))
		std::cout <<  Color::YELLOW << "\t3.\t" << Color::RESET << "1/16" << std::endl;

	if (tournament.getHasHeighth() && isHeighthUnlocked(tournament))
		std::cout <<  Color::YELLOW << "\t4.\t" << Color::RESET << "1/8" << std::endl;

	if (isQuartersUnlocked(tournament))
		std::cout <<  Color::YELLOW << "\t5.\t" << Color::RESET << "1/4" << std::endl;

	if (isSemisUnlocked(tournament))
		std::cout <<  Color::YELLOW << "\t6.\t" << Color::RESET << "1/2" << std::endl;

	//if (isThirdUnlocked(tournament))
	std::cout <<  Color::YELLOW << "\t7.\t" << Color::RESET << "3eme place" << std::endl;

	if (isFinalUnlocked(tournament))
		std::cout <<  Color::YELLOW << "\t7.\t" << Color::RESET << "Final" << std::endl;

	std::cout << "────────────────────────────────────────────────────────────" << std::endl;
	std::cout <<  Color::YELLOW << "\t8.\t" << Color::RESET << "Export" << std::endl;
	std::cout << "────────────────────────────────────────────────────────────" << std::endl;
	std::cout <<  Color::YELLOW << "\t9.\t" << Color::RESET << "Show" << std::endl;
	std::cout << "────────────────────────────────────────────────────────────" << std::endl;
	std::cout <<  Color::YELLOW << "\tQ.\t" << Color::RESET << "Quit" << std::endl;
	std::cout << "============================================================" << std::endl;
	std::cout << "Votre choix : ";
}


/********************/
/*  HANDLER SAISIE	*/
/********************/

/**
 * Vide le buffer stdin apres une lecture echouee.
 */
void				TournamentCLI::clearInput()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 *	Recupere la string entree par l utilisateur
 */
STRING				TournamentCLI::fetchInput()
{
	STRING input;
	
	if (!(std::cin >> input))
	{
		checkInterruption();
		clearInput();
		PrintUtils::addError("Saisie invalide.");
		return ("");
	}
	
	return (input);
}

/**
 *	Tramsforme la saisie utilisateur en int
 */
int					TournamentCLI::parseChoice(C_STRING input)
{
	try
	{
		return (std::stoi(input));
	}
	catch (...)
	{
		PrintUtils::addError("Saisie invalide.");
		return (-1);
	}
}

/**
 *	Appel la bonne methode d apres le choix de l utilisateur
 */
void				TournamentCLI::executeChoice(int choice, Tournament& tournament)
{
	switch (choice)
	{
		case 1:
			handlePoolSelection(tournament);
			break;

		case 2:
			for (const Pool* pool : tournament.getPools())
				PoolCLI::displayPoolDetails(*pool);
			break;

		case 3:
			if (tournament.getHasSixteenth() && isSixteenthUnlocked(tournament))
				handleEliminationPhase(tournament.getSixteenth(), [&]() { tournament.generateSixteenths(); },
					"1/16 DE FINALE", "1/16 generes avec succes !", "Impossible : verifiez que les poules soient terminees.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;

		case 4:
			if (tournament.getHasHeighth() && isHeighthUnlocked(tournament))
				handleEliminationPhase(tournament.getHeighth(), [&]() { tournament.generateHeighths(); },
					"1/8 DE FINALE", "1/8 generes avec succes !", "Impossible : verifiez que la phase precedente soit terminee.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;

		case 5:
			if (isQuartersUnlocked(tournament))
				handleEliminationPhase(tournament.getQuarters(), [&]() { tournament.generateQuarters(); },
					"QUARTS DE FINALE", "Quarts de finale generes avec succes !", "Impossible : verifiez que les poules/phases precedentes soient terminees.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;

		case 6:
			if (isSemisUnlocked(tournament))
				handleEliminationPhase(tournament.getSemis(), [&]() { tournament.generateSemis(); },
					"DEMI-FINALES", "Demi-finales generees avec succes !", "Impossible : verifiez que les quarts soient termines.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;

		case 7:
			if (isFinalUnlocked(tournament))
				handleEliminationPhase(tournament.getFinal(), [&]() { tournament.generateFinal(); },
					"FINALE", "Finale generee avec succes !", "Impossible : verifiez que les demi-finales soient terminees.");
			else
				PrintUtils::addError("Menu non disponible pour le moment.");
			break;

		case 8:
			if (tournament.getQuarters() != nullptr)
				TournamentViewer::displayFullBracket(tournament);
			else
				PrintUtils::addError("L'arbre n'est pas encore disponible.");
			break;

		case 9:
			if (tournament.getFinal() && tournament.getFinal()->isFinished())
				TournamentViewer::displayPodium(tournament);
			else
				PrintUtils::addError("Le podium n'est pas encore disponible.");
			break;

		default:
			PrintUtils::addError("Choix non disponible.");
			break;
	}
}

/**
 *	Demande un nom de fichier a l utilisateur
 */
STRING				TournamentCLI::promptFilename(C_STRING prompt)
{
	std::cout << prompt;

	STRING filename;

	std::getline(std::cin, filename);

	if (filename.empty())
		PrintUtils::addError("Nom de fichier vide — opération annulée.");

	return (filename);
}

/**
 * Gere la selection d une poule et l affichage de ses matchs.
 * Liste les poules, lit un choix, dispatch vers handleMatchList.
 */
void				TournamentCLI::handlePoolSelection(Tournament& tournament)
{
	const VP_POOL pools = tournament.getPools();

	if (pools.empty())
	{
		PrintUtils::addError("Aucune poule disponible.");
		return ;
	}

	std::cout << "\n--- Liste des Poules ---\n";

	for (size_t i = 0; i < pools.size(); ++i)
		std::cout << (i + 1) << ". " << pools[i]->getName() << "\n";

	std::cout << (pools.size() + 1) << ". Retour\n";
	std::cout << "Choisissez une poule : ";

	size_t pIdx;

	if (!(std::cin >> pIdx) || pIdx < 1 || pIdx > pools.size())
	{
		clearInput();
		return ;
	}

	const STRING title = "MATCHS " + pools[pIdx - 1]->getName();

	handleMatchList(pools[pIdx - 1]->getMatches(), title);
}

/**
 * Factorise le pattern commun à toutes les phases eliminatoires :
 *   - si phase == nullptr → appelle generateFn(), affiche succes ou erreur
 *   - sinon → affiche les matchs via handlePhase()
 */
void				TournamentCLI::handleEliminationPhase(Phase* phase,
															const std::function<void()>& generateFn,
															C_STRING phaseName, C_STRING successMsg,
															C_STRING errorMsg)
{
	if (!phase)
	{
		generateFn();

		if (phase)
			PrintUtils::addSuccess(successMsg);
		else
			PrintUtils::addError(errorMsg);
	}
	else
		handlePhase(phase, phaseName);
}

/**
 * Gere le flux complet de l export d une phase
 * Demande quelle phase, quel fichier, puis exporte.
 */
void				TournamentCLI::handleExport(Tournament& tournament)
{
	while (true)
	{
		handleTitle();
		PrintUtils::exportMenu();
		std::cout << Color::YELLOW << "\t1.\t" << Color::RESET << "Players\n";
		std::cout << Color::YELLOW << "\t2.\t" << Color::RESET << "Teams\n";
		std::cout << Color::YELLOW << "\t3.\t" << Color::RESET << "Pools\n";

		int menuIdx = 4;
		int sixteenthIdx = -1;
		int heighthIdx = -1;
		int quarterIdx = -1;
		int semiIdx = -1;
		int finalIdx = -1;
		int thirdIdx = -1;

		if (tournament.getSixteenth() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "1/16" << std::endl;
			sixteenthIdx = menuIdx++;
		}

		if (tournament.getHeighth() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "1/8" << std::endl;
			heighthIdx = menuIdx++;
		}

		if (tournament.getQuarters() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "1/4" << std::endl;
			quarterIdx = menuIdx++;
		}

		if (tournament.getSemis() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "1/2" << std::endl;
			semiIdx = menuIdx++;
		}

		if (tournament.getFinal() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "Final" << std::endl;
			finalIdx = menuIdx++;
		}

		if (tournament.getThirdPlace() != nullptr)
		{
			std::cout << Color::YELLOW << std::format("\t{}.\t", menuIdx) << Color::RESET << "3 place" << std::endl;
			thirdIdx = menuIdx++;
		}

		int fullIdx = menuIdx++;

		std::cout << "────────────────────────────────────────────────────────────\n";
		std::cout << std::format("  {}. Tournoi Complet\n", fullIdx);
		std::cout << Color::YELLOW << "\t0.\t" << Color::RESET << "Quitter" <<std::endl;
		std::cout << "=============================================================\n";
		std::cout << "\tChoix :\t";

		STRING rawInput;

		if (!(std::cin >> rawInput))
		{
			clearInput();
			return ;
		}

		int choice;

		try
		{
			choice = std::stoi(rawInput);
		}
		catch (...)
		{
			PrintUtils::addError("Saisie invalide.");
			continue ;
		}

		if (choice == 0)
			return ;

		clearInput();

		const STRING filename = promptFilename("Nom du fichier de sortie : ");

		if (filename.empty())
			continue ;

		bool ok = false;

		if (choice == 1)
		{
			std::ofstream out(filename);

			if (out.is_open())
			{
				out << "========== LISTE DES PARTICIPANTS ==========\n";
				for (const Pool* p : tournament.getPools())
				{
					if (!p)
						continue ;

					for (const Team* t : p->getTeams())
					{
						if (!t)
							continue ;

						out << "Equipe : " << t->getName() << "\n";
					}
				}
				out.close();
				ok = true;
			}
		} 
		else if (choice == 2)
		{
			std::ofstream out(filename);

			if (out.is_open())
			{
				out << "========== LISTE DES EQUIPES ==========\n";

				for (const Pool* p : tournament.getPools())
				{
					if (!p)
						continue ;

					out << "\n[" << p->getName() << "]\n";

					for (const Team* t : p->getTeams())
						if (t)
							out << "  - " << t->getName() << "\n";
				}
				out.close();
				ok = true;
			}
		} 
		else if (choice == 3)
			ok = Exporter::exportPoolsToTxt(tournament, filename);
		else if (choice == sixteenthIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getSixteenth(), filename);
		else if (choice == heighthIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getHeighth(), filename);
		else if (choice == quarterIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getQuarters(), filename);
		else if (choice == semiIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getSemis(), filename);
		else if (choice == finalIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getFinal(), filename);
		else if (choice == thirdIdx)
			ok = Exporter::exportPhaseToTxt(tournament.getThirdPlace(), filename);
		else if (choice == fullIdx)
			ok = Exporter::exportTournamentToTxt(tournament, filename);
		else
		{
			PrintUtils::addError("Choix non valide.");
			continue ;
		}

		if (ok)
		{
			PrintUtils::addSuccess(std::format("Export réussi dans {}.", filename));
			break ;
		}
		else
			PrintUtils::addError("Echec de l'export.");
	}
}

/**
 * Mappe un entier vers la Phase* correspondante.
 * Retourne nullptr si le choix est hors-plage ou la phase non generee.
 *
 * 2→1/16  3→1/8  4→Quarts  5→Demis  6→Finale  7→3e Place
 */
const Phase*		TournamentCLI::getPhaseByMenuChoice(const Tournament& tournament, const int choice)
{
	switch (choice)
	{
		case 2:
			return (tournament.getSixteenth());

		case 3:
			return (tournament.getHeighth());

		case 4:
			return (tournament.getQuarters());

		case 5:
			return (tournament.getSemis());

		case 6:
			return (tournament.getFinal());

		case 7:
			return (tournament.getThirdPlace());

		default:
			return (nullptr);
	}
}

/********************/
/*  CHECKER PHASE	*/
/********************/

/**
 *	Verifie si tout les matchs de pools sont finis
 */
bool				TournamentCLI::isPoolsFinished(const Tournament& tournament)
{
	for (const Pool* pool : tournament.getPools())
	{
		if (!pool)
			continue ;

		for (const Match* m : pool->getMatches())
			if (m && !m->isFinished())
				return (false);
	}

	return (!tournament.getPools().empty());
}

/**
 *	Active la phase de 1/16 si les maths de pools sont finis et qu il y a des 1/16 a jouer
 */
bool				TournamentCLI::isSixteenthUnlocked(const Tournament& tournament)
{
	return (tournament.getHasSixteenth() && isPoolsFinished(tournament));
}

/**
 *	Active la phase de 1/8 si les maths de pools ou de 1/16 sont finis et qu il y a des 1/8 a jouer
 */
bool				TournamentCLI::isHeighthUnlocked(const Tournament& tournament)
{
	if (!tournament.getHasHeighth())
		return (false);

	if (tournament.getHasSixteenth())
		return (tournament.getSixteenth() && tournament.getSixteenth()->isFinished());

	return (isPoolsFinished(tournament));
}

/**
 *	Active la phase de 1/4 si les maths de pools ou les 1/8 sont finis
 */
bool				TournamentCLI::isQuartersUnlocked(const Tournament& tournament)
{
	if (tournament.getHasHeighth())
		return (tournament.getHeighth() && tournament.getHeighth()->isFinished());

	return (isPoolsFinished(tournament));
}

/**
 *	Active la phase de 1/2 si les maths de 1/4 sont finis
 */
bool				TournamentCLI::isSemisUnlocked(const Tournament& tournament)
{
	return (tournament.getQuarters() && tournament.getQuarters()->isFinished());
}

/**
 *	Active la phase finale si les maths de 1/2 sont finis
 */
bool				TournamentCLI::isFinalUnlocked(const Tournament& tournament)
{
	return (tournament.getSemis() && tournament.getSemis()->isFinished());
}

/**
 *	Active le match de la 3 place si les maths de 1/2 sont finis
 */
bool				TournamentCLI::isThirdUnlocked(const Tournament& tournament)
{
	if (!tournament.getHasThirdMatch())
		return (false);

	return (tournament.getSemis() && tournament.getSemis()->isFinished());
}

/************************/
/*  HANDLERS AFFICHAGE	*/
/************************/

/**
 * Affiche la liste des matchs d une phase et permet d en saisir les scores.
 * Boucle jusqu a ce que l utilisateur choisisse "Retour".
 */
void				TournamentCLI::handleMatchList(CVP_MATCH matches, C_STRING title)
{
	size_t mIdx = 0;

	while (true)
	{
		std::cout << "\n--- " << title << " ---\n";

		for (size_t i = 0; i < matches.size(); ++i)
		{
			std::cout << (i + 1) << ". ";

			if (matches[i])
				MatchCLI::display(*matches[i]);
			else
				std::cout << "Match non defini\n";
		}

		std::cout << (matches.size() + 1) << ". Retour\n";
		std::cout << "Selectionnez un match : ";

		if (!(std::cin >> mIdx) || mIdx < 1 || mIdx > matches.size() + 1)
		{
			clearInput();
			PrintUtils::addError("Choix invalide.");
			continue ;
		}

		if (mIdx == matches.size() + 1)
			break ;

		if (matches[mIdx - 1])
			MatchCLI::inputScore(*matches[mIdx - 1]);
	}
}

/**
 * Affiche une phase si elle existe, sinon informe l utilisateur.
 */
void				TournamentCLI::handlePhase(Phase* phase, C_STRING phaseName)
{
	if (phase == nullptr)
	{
		PrintUtils::addError("Cette phase n'est pas encore active.");
		return ;
	}

	handleMatchList(phase->getMatches(), phaseName);
}

/**************************************************************************************************/
/*	PUBLIC METHOD																				  */
/**************************************************************************************************/

/**
 *	Gestion du menu Tournament du programme
 */
void				TournamentCLI::handleMenuTournament(Tournament& tournament)
{
	try
	{
		while (true)
		{
			displayMenuUI(tournament);

			STRING input = fetchInput();
			
			if (input.empty())
				continue;

			if (input == "q" || input == "Q")
				return;

			if (input == "e" || input == "E")
			{
				handleExport(tournament);
				continue;
			}

			int choice = parseChoice(input);
			
			if (choice != -1)
				executeChoice(choice, tournament);
		}
	}
	catch (const UserInterruptedException&)
	{
		return ;
	}
}
