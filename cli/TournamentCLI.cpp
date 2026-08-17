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
#include "../includes/cli/TournamentCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"
#include "../includes/cli/SettingsCLI.hpp"
#include "../includes/cli/PhaseCLI.hpp"
#include "../includes/cli/PoolCLI.hpp"
#include "../includes/utils/PrintUtils.hpp"
#include "../includes/Global.hpp"
#include "../includes/Color.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VP_POOL		=	std::vector<Pool*>;
using				VP_MATCH	=	std::vector<Match*>;
using				CVP_MATCH	=	const std::vector<Match*>&;
using               CVP_TEAM	=	const std::vector<Team*>&;

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

/**
 * Vide le buffer stdin apres une lecture echouee.
 */
void				TournamentCLI::clearInput()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * Affiche le prompt, lit une ligne sur stdin.
 * Retourne la chaîne saisie, ou "" si l utilisateur n a rien tape.
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
 * Retourne le nom de l equipe en position teamPos (1=A, 2=B) du match
 * matchIdx dans phase, ou "A determiner" si indisponible.
 */
STRING				TournamentCLI::getTeamNameOrPlaceholder(const Phase* phase,
															const size_t matchIdx,
															const int teamPos)
{
	if (!phase)
		return ("A determiner");

	const VP_MATCH&	matches = phase->getMatches();

	if (matchIdx >= matches.size() || !matches[matchIdx])
		return ("A determiner");

	Team* t = (teamPos == 1)	? matches[matchIdx]->getTeamA()
								: matches[matchIdx]->getTeamB();

	return (t ? t->getName() : "A determiner");
}

/************************/
/*  AFFICHAGE DU MENU	*/
/************************/

/**
 * Affiche les options du menu en fonction de l etat courant du tournoi.
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

void				TournamentCLI::handleTitle()
{
	PrintUtils::clear();
	PrintUtils::banner();
	PrintUtils::tournament();
}

/************************/
/*  HANDLERS DE SAISIE	*/
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

/**
 * Exporte toutes les poules dans un fichier .txt.
 */
bool				TournamentCLI::exportPoolsToFile(const Tournament& tournament, C_STRING filename)
{
	std::ofstream out(filename);

	if (!out.is_open())
	{
		PrintUtils::addError(std::format("Impossible de créer le fichier : {}", filename));
		return (false);
	}

	writePools(out, tournament);
	out.close();

	return (true);
}

/**
 * Gere le flux complet de l export d une phase
 * Demande quelle phase, quel fichier, puis exporte.
 */
void				TournamentCLI::handleExportPhase(Tournament& tournament)
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
			ok = exportPoolsToFile(tournament, filename);
		else if (choice == sixteenthIdx)
			ok = exportPhaseToTxt(tournament.getSixteenth(), filename);
		else if (choice == heighthIdx)
			ok = exportPhaseToTxt(tournament.getHeighth(), filename);
		else if (choice == quarterIdx)
			ok = exportPhaseToTxt(tournament.getQuarters(), filename);
		else if (choice == semiIdx)
			ok = exportPhaseToTxt(tournament.getSemis(), filename);
		else if (choice == finalIdx)
			ok = exportPhaseToTxt(tournament.getFinal(), filename);
		else if (choice == thirdIdx)
			ok = exportPhaseToTxt(tournament.getThirdPlace(), filename);
		else if (choice == fullIdx)
			ok = exportTournamentToTxt(tournament, filename);
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

bool				TournamentCLI::isSixteenthUnlocked(const Tournament& tournament)
{
	return (tournament.getHasSixteenth() && isPoolsFinished(tournament));
}

bool				TournamentCLI::isHeighthUnlocked(const Tournament& tournament)
{
	if (!tournament.getHasHeighth())
		return (false);

	if (tournament.getHasSixteenth())
		return (tournament.getSixteenth() && tournament.getSixteenth()->isFinished());

	return (isPoolsFinished(tournament));
}

bool				TournamentCLI::isQuartersUnlocked(const Tournament& tournament)
{
	if (tournament.getHasHeighth())
		return (tournament.getHeighth() && tournament.getHeighth()->isFinished());

	if (tournament.getHasSixteenth())
		return (tournament.getSixteenth() && tournament.getSixteenth()->isFinished());

	return (isPoolsFinished(tournament));
}

bool				TournamentCLI::isSemisUnlocked(const Tournament& tournament)
{
	return (tournament.getQuarters() && tournament.getQuarters()->isFinished());
}

bool				TournamentCLI::isFinalUnlocked(const Tournament& tournament)
{
	return (tournament.getSemis() && tournament.getSemis()->isFinished());
}

/************/
/*  PUBLIC	*/
/************/

/**
 * Point d entree du tournoi.
 */
void				TournamentCLI::handleMenuTournament(Tournament& tournament)
{
	try
	{
		STRING input;
		int choice;

		while (true)
		{
			handleTitle();
			PrintUtils::handleMessages();
			menuTournament(tournament);
			checkInterruption();

			if (!(std::cin >> input))
			{
				checkInterruption();
				clearInput();
				PrintUtils::addError("Saisie invalide.");
				continue ;
			}

			if (input == "q" || input == "Q")
				return ;

			if (input == "e" || input == "E")
			{
				handleExportPhase(tournament);
				continue ;
			}

			try
			{
				choice = std::stoi(input);
			}
			catch (...)
			{
				PrintUtils::addError("Saisie invalide.");
				continue ;
			}

			switch (choice)
			{
				case 1:
					handlePoolSelection(tournament);
					break ;

				case 2:
					for (const Pool* pool : tournament.getPools())
						PoolCLI::displayPoolDetails(*pool);
					break ;

				case 3:
					if (tournament.getHasSixteenth() && isSixteenthUnlocked(tournament))
						handleEliminationPhase(tournament.getSixteenth(),[&]() { tournament.generateSixteenths(); },
							"1/16 DE FINALE", "1/16 generes avec succes !",	"Impossible : verifiez que les poules soient terminees.");
					else
						PrintUtils::addError("Menu non disponible pour le moment.");
					break ;

				case 4:
					if (tournament.getHasHeighth() && isHeighthUnlocked(tournament))
						handleEliminationPhase(tournament.getHeighth(), [&]() { tournament.generateHeighths(); },
							"1/8 DE FINALE", "1/8 generes avec succes !", "Impossible : verifiez que la phase precedente soit terminee.");
					else
						PrintUtils::addError("Menu non disponible pour le moment.");
					break ;

				case 5:
					if (isQuartersUnlocked(tournament))
						handleEliminationPhase(tournament.getQuarters(), [&]() { tournament.generateQuarters(); },
							"QUARTS DE FINALE", "Quarts de finale generes avec succes !", "Impossible : verifiez que les poules/phases precedentes soient terminees.");
					else
						PrintUtils::addError("Menu non disponible pour le moment.");
					break ;

				case 6:
					if (isSemisUnlocked(tournament))
						handleEliminationPhase(tournament.getSemis(), [&]() { tournament.generateSemis(); },
							"DEMI-FINALES", "Demi-finales generees avec succes !", "Impossible : verifiez que les quarts soient termines.");
					else
						PrintUtils::addError("Menu non disponible pour le moment.");
					break ;

				case 7:
					if (isFinalUnlocked(tournament))
						handleEliminationPhase(tournament.getFinal(), [&]() { tournament.generateFinal(); },
							"FINALE", "Finale generee avec succes !", "Impossible : verifiez que les demi-finales soient terminees.");
					else
						PrintUtils::addError("Menu non disponible pour le moment.");
					break ;

				case 8:
					if (tournament.getQuarters() != nullptr)
						displayFullBracket(tournament);
					else
						PrintUtils::addError("L'arbre n'est pas encore disponible.");
					break ;

				case 9:
					if (tournament.getFinal() && tournament.getFinal()->isFinished())
						displayPodium(tournament);
					else
						PrintUtils::addError("Le podium n'est pas encore disponible.");
					break ;

				default:
					PrintUtils::addError("Choix non disponible.");
					break ;
			}
		}
	}
	catch (const UserInterruptedException&)
	{
		return ;
	}
}

/********************************/
/*  PUBLIC — Affichage terminal	*/
/********************************/

/**
 * Affiche l arbre ASCII de la phase finale.
 */
void				TournamentCLI::displayFullBracket(Tournament& tournament)
{
	const Phase* Q = tournament.getQuarters();
	const Phase* S = tournament.getSemis();
	const Phase* F = tournament.getFinal();
	const Phase* T = tournament.getThirdPlace();

	PrintUtils::clear();

	std::cout << "\n=================== ARBRE DE LA PHASE FINALE ===================\n\n";
	std::cout << "\tQUARTS\t\t\tDEMIS\t\t\t\t\t3e PLACE\tFINALE\t\t\tVAINQUEUR\n\n";

	const STRING q[4][2] = {
		{ getTeamNameOrPlaceholder(Q, 0, 1), getTeamNameOrPlaceholder(Q, 0, 2) },
		{ getTeamNameOrPlaceholder(Q, 1, 1), getTeamNameOrPlaceholder(Q, 1, 2) },
		{ getTeamNameOrPlaceholder(Q, 2, 1), getTeamNameOrPlaceholder(Q, 2, 2) },
		{ getTeamNameOrPlaceholder(Q, 3, 1), getTeamNameOrPlaceholder(Q, 3, 2) }
	};
	const STRING s[2][2] = {
		{ getTeamNameOrPlaceholder(S, 0, 1), getTeamNameOrPlaceholder(S, 0, 2) },
		{ getTeamNameOrPlaceholder(S, 1, 1), getTeamNameOrPlaceholder(S, 1, 2) }
	};
	const STRING f[2]    = {
		getTeamNameOrPlaceholder(F, 0, 1),
		getTeamNameOrPlaceholder(F, 0, 2)
	};
	const STRING t3[2]   = {
		getTeamNameOrPlaceholder(T, 0, 1),
		getTeamNameOrPlaceholder(T, 0, 2)
	};

	STRING winner = "A determiner";

	if (F && F->isFinished() && !F->getMatches().empty())
	{
		const Team*	w = F->getMatches()[0]->getWinner();

		if (w)
			winner = w->getName();
	}

	const bool hasThird = tournament.getSettings().getIsThirdPlaceMatch();

	std::cout << "\t" << q[0][0] << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[0][0] << " ---\n";
	std::cout << "\t" << q[0][1] << " ---\n";
	std::cout << "\t\t\t\t\t\t\t\t\t\t|---> " << f[0] << " ---\n";
	std::cout << "\t" << q[1][0] << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[0][1] << " ---\n";
	std::cout << "\t" << q[1][1] << " ---\n";

	if (hasThird)
		std::cout << "\t\t\t\t\t\t\t\t|--> " << t3[0] << " ---\n";

	std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t|---> " << winner << "\n";

	if (hasThird)
		std::cout << "\t\t\t\t\t\t\t\t|--> " << t3[1] << " ---\n";

	std::cout << "\t" << q[2][0] << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[1][0] << " ---\n";
	std::cout << "\t" << q[2][1] << " ---\n";
	std::cout << "\t\t\t\t\t\t\t\t\t\t|---> " << f[1] << " ---\n";
	std::cout << "\t" << q[3][0] << " ---\n";
	std::cout << "\t\t\t\t|---> " << s[1][1] << " ---\n";
	std::cout << "\t" << q[3][1] << " ---\n";
	std::cout << "==============================================================================\n";
}

void				TournamentCLI::displayPodium(const Tournament& tournament)
{
	const Phase* final = tournament.getFinal();
	const Phase* thirdPlace = tournament.getThirdPlace();

	std::cout << "\n╔══════════════════════════════════════╗\n";
	std::cout << "║            PALMARES FINAL            ║\n";
	std::cout << "╚══════════════════════════════════════╝\n";

	if (!final || !final->isFinished())
	{
		PrintUtils::addError("La finale n'est pas encore terminee.");
		return ;
	}

	CVP_TEAM winners = final->getWinners();
	CVP_TEAM losers = final->getLosers();

	if (!winners.empty() && winners[0])
		std::cout << "\t\t\t\t1er:\t" << winners[0]->getName() << std::endl;

	if (!losers.empty() && losers[0])
		std::cout << "\t\t\t2e:\t\t" << losers[0]->getName() << std::endl;

	if (thirdPlace && thirdPlace->isFinished())
	{
		CVP_TEAM third = thirdPlace->getWinners();
		CVP_TEAM fourth = thirdPlace->getLosers();

		if (!third.empty()  && third[0])
			std::cout << "\t\t3e:\t\t\t" << third[0]->getName()  << std::endl;

		if (!fourth.empty() && fourth[0])
			std::cout << "\t4e:\t\t\t\t" << fourth[0]->getName() << std::endl;
	}

	std::cout << "══════════════════════════════════════\n";
}

/****************************/
/*  PUBLIC — Export fichier	*/
/****************************/

bool				TournamentCLI::exportPhaseToTxt(const Phase* phase, C_STRING filename)
{
	if (!phase)
	{
		PrintUtils::addError("Phase inexistante ou non generee — export annule.");
		return (false);
	}

	return (PhaseCLI::exportToTxt(*phase, filename));
}

bool				TournamentCLI::exportTournamentToTxt(const Tournament& tournament, C_STRING filename)
{
	std::ofstream out(filename);

	if (!out.is_open())
	{
		PrintUtils::addError(std::format("Impossible de creer le fichier : {}", filename));
		return (false);
	}

	writeHeader(out, tournament);
	writePools(out, tournament);
	writePhaseBlock(out, tournament.getSixteenth());
	writePhaseBlock(out, tournament.getHeighth());
	writePhaseBlock(out, tournament.getQuarters());
	writePhaseBlock(out, tournament.getSemis());
	writePhaseBlock(out, tournament.getThirdPlace());
	writePhaseBlock(out, tournament.getFinal());
	writePalmares(out, tournament);

	out.close();

	return (true);
}

/********************************/
/*  HELPERS ECRITURE FICHIER	*/
/********************************/

void				TournamentCLI::writeHeader(std::ofstream& out, const Tournament& tournament)
{
	const Settings&	s = tournament.getSettings();

	out << "############################################################\n";
	out << "##\t\tTOURNOI :\t" << s.getName() << "\n";
	out << "############################################################\n\n";

	out << "\tType\t\t\t:\t";

	if (s.getIsDouble() && s.getIsMixed())
		out << "Double mixte\n";
	else if (s.getIsDouble())
		out << "Double\n";
	else
		out << "Simple\n";

	out << "\tJoueurs\t\t\t:\t" << s.getNbPlayers() << "\n";
	out << "\tPoules\t\t\t:\t" << s.getNbPools() << " x " << s.getNbPlayerByPool() << " equipes\n";
	out << "\tScore min/max\t:\t" << s.getScoreMin() << " / " << s.getScoreMax()
		<< " (ecart " << s.getDiffPointsToWin() << ")\n";
	out << "\tMulti-team\t\t:\t" << (s.getAllowMultiTeamPlayers() ? "Oui" : "Non") << "\n";
	out << "\tPetite finale\t:\t" << (s.getIsThirdPlaceMatch()    ? "Oui" : "Non") << "\n\n";
}

void				TournamentCLI::writePools(std::ofstream& out, const Tournament& tournament)
{
	const VP_POOL pools = tournament.getPools();

	out << "============================================================\n";
	out << "\tPHASE DE POULES\n";
	out << "============================================================\n";

	if (pools.empty())
	{
		PrintUtils::addError("Aucune poule enregistree.");
		return ;
	}

	for (const Pool* pool : pools)
	{
		if (!pool)
			continue ;

		out << "\n------------------------------------------------------------\n";
		out << "\t" << pool->getName() << "\n";
		out << "------------------------------------------------------------\n";

		writePoolMatches(out, *pool);
		writePoolStandings(out, *pool);
	}

	out << "\n";
}

void				TournamentCLI::writePoolMatches(std::ofstream& out, const Pool& pool)
{
	out << "\n\t[MATCHS]\n";

	int idx = 1;

	for (const Match* m : pool.getMatches())
	{
		if (!m)
			continue ;

		out << "\t\t" << std::setw(2) << idx++ << ". "
			<< m->getTeamA()->getName() << " vs " << m->getTeamB()->getName();

		if (m->isFinished())
		{
			out << "\t[ " << m->getScoreA() << " - " << m->getScoreB() << " ]";

			if (m->getWinner())
				out << "  ->  " << m->getWinner()->getName();
		}
		else
			out << "\t[ A jouer ]";

		out << "\n";
	}
}

void				TournamentCLI::writePoolStandings(std::ofstream& out, const Pool& pool)
{
	CVP_TEAM teams = pool.getTeams();
	size_t maxLen = 6;

	for (const Team* t : teams)
		if (t->getName().size() > maxLen)
			maxLen = t->getName().size();

	const int w = static_cast<int>(maxLen) + 2;

	out << "\n\t[CLASSEMENT]\n";
	out << "\t\t" << std::left << std::setw(4) << "#"
		<< std::setw(w) << "Equipe"
		<< std::setw(6) << "Pts"
		<< "Diff\n";
	out << "\t\t" << STRING(4 + w + 12, '-') << "\n";

	for (size_t i = 0; i < teams.size(); ++i)
	{
		const int diff = teams[i]->getScoreDiff();

		out << "\t\t" << std::left << std::setw(4) << (i + 1)
			<< std::setw(w) << teams[i]->getName()
			<< std::setw(6) << teams[i]->getPoint()
			<< (diff >= 0 ? "+" : "") << diff << "\n";
	}
}

void				TournamentCLI::writePhaseBlock(std::ofstream& out, const Phase* phase)
{
	if (!phase)
		return ;

	CVP_MATCH matches = phase->getMatches();
	const int nbSets = phase->getNbSetToPlay();

	out << "============================================================\n";
	out << "\t" << phase->getName() << "  (" << nbSets << " set(s) par rencontre)\n";
	out << "============================================================\n";

	if (matches.empty())
	{
		PrintUtils::addError("Aucun match enregistre.");
		return ;
	}

	int encounterNum = 1;

	for (size_t i = 0; i < matches.size(); i += static_cast<size_t>(nbSets))
		writeEncounterBlock(out, matches, i, nbSets, encounterNum++);

	if (phase->isFinished())
		writePhaseResults(out, *phase);

	out << "\n";
}

void				TournamentCLI::writeEncounterBlock(std::ofstream& out, CVP_MATCH matches,
														const size_t startIdx, const int nbSets,
														const int encounterNum)
{
	if (startIdx >= matches.size() || !matches[startIdx])
		return ;

	const Match* first = matches[startIdx];
	C_STRING nameA = first->getTeamA() ? first->getTeamA()->getName() : "?";
	C_STRING nameB = first->getTeamB() ? first->getTeamB()->getName() : "?";

	out << "\n\tRencontre " << encounterNum << " :  " << nameA << "  vs  " << nameB << "\n";
	out << "\t" << STRING(nameA.size() + nameB.size() + 14, '-') << "\n";

	int winsA = 0;
	int winsB = 0;

	for (int s = 0; s < nbSets; ++s)
	{
		const size_t idx = startIdx + static_cast<size_t>(s);

		if (idx >= matches.size() || !matches[idx])
			break ;

		const Match* m = matches[idx];

		out << "\tSet " << (s + 1) << " : ";

		if (!m->isFinished())
		{
			out << "[ a jouer ]\n";
			continue ;
		}

		out << std::setw(3) << m->getScoreA() << " - " << std::setw(3) << m->getScoreB();

		if (m->getWinner())
		{
			out << "  ->  " << m->getWinner()->getName();

			if (m->getWinner() == first->getTeamA()) winsA++;
			else                                      winsB++;
		}

		out << "\n";
	}

	out << "\tVainqueur : ";

	if (winsA > winsB)
		out << nameA;
	else if (winsB > winsA)
		out << nameB;
	else
		out << "Non determine";

	out << "\n  " << STRING(50, '-') << "\n";
}

void				TournamentCLI::writePhaseResults(std::ofstream& out, const Phase& phase)
{
	CVP_TEAM winners = phase.getWinners();
	CVP_TEAM losers = phase.getLosers();

	out << "\n\tQualifies :\n";

	for (size_t i = 0; i < winners.size(); ++i)
		if (winners[i])
			out << "\t\t" << (i + 1) << ". " << winners[i]->getName() << "\n";

	if (!losers.empty())
	{
		out << "\tElimines :\n";

		for (const Team* t : losers)
			if (t)
				out << "\t\t- " << t->getName() << "\n";
	}
}

void				TournamentCLI::writePalmares(std::ofstream& out, const Tournament& tournament)
{
	const Phase* final = tournament.getFinal();
	const Phase* thirdPlace = tournament.getThirdPlace();

	out << "============================================================\n";
	out << "\tPALMARES\n";
	out << "============================================================\n";

	if (!final || !final->isFinished())
	{
		PrintUtils::addError("Finale non terminee — palmares indisponible.");
		return ;
	}

	CVP_TEAM winners = final->getWinners();
	CVP_TEAM losers  = final->getLosers();

	if (!winners.empty() && winners[0])
		out << "\t1. (Or)\t\t" << winners[0]->getName() << "\n";

	if (!losers.empty()  && losers[0])
		out << "\t2. (Argent)\t" << losers[0]->getName()  << "\n";

	if (thirdPlace && thirdPlace->isFinished())
	{
		CVP_TEAM third = thirdPlace->getWinners();
		CVP_TEAM fourth = thirdPlace->getLosers();

		if (!third.empty()  && third[0])
			out << "\t3. (Bronze)\t" << third[0]->getName()  << "\n";

		if (!fourth.empty() && fourth[0])
			out << "\t4.\t\t\t" << fourth[0]->getName() << "\n";
	}

	out << "\n";
}
