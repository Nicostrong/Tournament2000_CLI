//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

//	STDLIB
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>

//	INCLUDES
#include "../includes/cli/TournamentCLI.hpp"
#include "../includes/cli/MatchCLI.hpp"
#include "../includes/cli/SettingsCLI.hpp"
#include "../includes/cli/PhaseCLI.hpp"
#include "../includes/cli/PoolCLI.hpp"
#include "../includes/utils/PrintUtils.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VP_POOL		=	std::vector<Pool*>;
using				VP_MATCH	=	std::vector<Match*>;
using				CVP_MATCH	=	const std::vector<Match*>&;
using               CVP_TEAM	=	const std::vector<Team*>&;

//	STATIC VARIABLES

/********************/
/*	PRIVATE METHOD	*/
/********************/

void				TournamentCLI::handlePhase(const Phase* phase, C_STRING phaseName)
{
	if (!phase)
	{
		std::cout << "Cette phase n'est pas encore active.\n";
		return ;
	}
	
	handleMatchList(phase->getMatches(), phaseName);
}

void				TournamentCLI::handleMatchList(const VP_MATCH& matches, C_STRING title)
{
	size_t			mIdx = 0;

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
		std::cout << "Selectionnez un match pour entrer/modifier le score : ";

		if (!(std::cin >> mIdx) || mIdx < 1 || mIdx > matches.size() + 1)
		{
			clearInput();

			std::cout << "Choix invalide.\n";
			continue ;
		}

		if (mIdx == matches.size() + 1)
			break ;

		if (matches[mIdx - 1])
			MatchCLI::inputScore(*matches[mIdx - 1]);
	}
}

STRING				TournamentCLI::getTeamNameOrPlaceholder(const Phase* phase, const size_t matchIdx, const int teamPos)
{
	if (!phase)
		return ("A determiner");

	const VP_MATCH	matches = phase->getMatches();

	if (matchIdx >= matches.size() || !matches[matchIdx])
		return ("A determiner");

	Team*			t = (teamPos == 1) ? matches[matchIdx]->getTeamA() : matches[matchIdx]->getTeamB();
	
	if (t) 
		return (t->getName());
		
	return ("A determiner");
}

void				TournamentCLI::clearInput()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * Ecrit l en-tête du fichier export : nom du tournoi + settings essentiels.
 */
void				TournamentCLI::writeHeader(std::ofstream& out, const Tournament& tournament)
{
	const Settings&	s = tournament.getSettings();

	out << "############################################################\n";
	out << "##  TOURNOI : " << s.getName() << "\n";
	out << "############################################################\n";
	out << "\n";
	out << "  Type           : ";

	if (s.getIsDouble() && s.getIsMixed())
		out << "Double mixte\n";
	else if (s.getIsDouble())
		out << "Double\n";
	else
		out << "Simple\n";

	out << "  Joueurs        : " << s.getNbPlayers() << "\n";
	out << "  Poules         : " << s.getNbPools()
		<< " x " << s.getNbPlayerByPool() << " équipes\n";
	out << "  Score mini/maxi: " << s.getScoreMin() << " / " << s.getScoreMax()
		<< " (écart " << s.getDiffPointsToWin() << ")\n";
	out << "  Joueur multi team : " << (s.getAllowMultiTeamPlayers() ? "Oui" : "Non") << "\n";
	out << "  Petite finale  : " << (s.getIsThirdPlaceMatch()    ? "Oui" : "Non") << "\n";
	out << "\n";
}

/**
 * Ecrit le bloc [POULES] : pour chaque poule, matchs + classement.
 * Reutilise les helpers de PoolCLI via ofstream.
 */
void				TournamentCLI::writePools(std::ofstream& out, const Tournament& tournament)
{
	const VP_POOL	pools = tournament.getPools();

	out << "============================================================\n";
	out << "  PHASE DE POULES\n";
	out << "============================================================\n";

	if (pools.empty())
	{
		out << "  Aucune poule enregistrée.\n\n";
		return ;
	}

	for (const Pool* pool : pools)
	{
		if (!pool)
			continue ;

		out << "\n------------------------------------------------------------\n";
		out << "  " << pool->getName() << "\n";
		out << "------------------------------------------------------------\n";

		out << "\n  [MATCHS]\n";

		CVP_MATCH	matches = pool->getMatches();
		int			idx = 1;

		for (const Match* m : matches)
		{
			if (!m)
				continue ;

			out << "    " << std::setw(2) << idx++ << ". "
				<< m->getTeamA()->getName() << " vs " << m->getTeamB()->getName();

			if (m->isFinished())
			{
				out << "  [ " << m->getScoreA() << " - " << m->getScoreB() << " ]";

				if (m->getWinner())
					out << "  ->  " << m->getWinner()->getName();
			}
			else
				out << "  [ À jouer ]";

			out << "\n";
		}

		out << "\n  [CLASSEMENT]\n";

		CVP_TEAM	teams	= pool->getTeams();
		size_t		maxLen = 6;

		for (const Team* t : teams)
			if (t->getName().size() > maxLen)
				maxLen = t->getName().size();

		const int	w = static_cast<int>(maxLen) + 2;

		out << "    " << std::left << std::setw(4) << "#"
			<< std::setw(w) << "Equipe"
			<< std::setw(6) << "Pts"
			<< "Diff\n";

		out << "    " << STRING(4 + w + 6 + 6, '-') << "\n";

		for (size_t i = 0; i < teams.size(); ++i)
		{
			const Team*				t = teams[i];
			const int				diff = t->getScoreDiff();

			out << "    " << std::left << std::setw(4) << (i + 1)
				<< std::setw(w)  << t->getName()
				<< std::setw(6)  << t->getPoint()
				<< (diff >= 0 ? "+" : "") << diff << "\n";
		}
	}

	out << "\n";
}

/**
 * Ecrit le bloc d une phase eliminatoire dans le flux.
 * Ne fait rien si phase == nullptr.
 * Delegue l ecriture rencontre par rencontre a PhaseCLI.
 */
void				TournamentCLI::writePhaseBlock(std::ofstream& out, const Phase* phase)
{
	if (!phase)
		return ;

	CVP_MATCH		matches = phase->getMatches();
	const int		nbSets = phase->getNbSetToPlay();

	out << "============================================================\n";
	out << "  " << phase->getName() << "  (" << nbSets << " set(s) par rencontre)\n";
	out << "============================================================\n";

	if (matches.empty())
	{
		out << "  Aucun match enregistré.\n\n";
		return ;
	}

	int				encounterNum = 1;

	for (size_t i = 0; i < matches.size(); i += static_cast<size_t>(nbSets))
	{
		if (i >= matches.size())
			break ;

		const Match*				first = matches[i];
		C_STRING					nameA = first->getTeamA() ? first->getTeamA()->getName() : "?";
		C_STRING					nameB = first->getTeamB() ? first->getTeamB()->getName() : "?";

		out << "\n  Rencontre " << encounterNum++ << " :  " << nameA << "  vs  " << nameB << "\n";
		out << "  " << STRING(nameA.size() + nameB.size() + 14, '-') << "\n";

		int			winsA = 0;
		int			winsB = 0;

		for (int s = 0; s < nbSets; ++s)
		{
			const size_t			idx = i + static_cast<size_t>(s);

			if (idx >= matches.size())
				break ;

			const Match*			m = matches[idx];

			out << "    Set " << (s + 1) << " : ";

			if (!m || !m->isFinished())
			{
				out << "[ à jouer ]\n";
				continue ;
			}

			out << std::setw(3) << m->getScoreA() << " - " << std::setw(3) << m->getScoreB();

			if (m->getWinner())
			{
				out << "  ->  " << m->getWinner()->getName();

				if (m->getWinner() == first->getTeamA())
					winsA++;
				else
					winsB++;
			}

			out << "\n";
		}

		out << "  Vainqueur : ";

		if (winsA > winsB)
			out << nameA;
		else if (winsB > winsA)
			out << nameB;
		else
			out << "Non déterminé";

		out << "\n  " << STRING(50, '-') << "\n";
	}

	if (phase->isFinished())
	{
		CVP_TEAM	winners = phase->getWinners();
		CVP_TEAM	losers = phase->getLosers();

		out << "\n  Qualifiés :\n";

		for (size_t i = 0; i < winners.size(); ++i)
			if (winners[i])
				out << "    " << (i + 1) << ". " << winners[i]->getName() << "\n";

		if (!losers.empty())
		{
			out << "  Éliminés :\n";

			for (const Team* t : losers)
				if (t)
					out << "    - " << t->getName() << "\n";
		}
	}

	out << "\n";
}

/**
 * Ecrit le bloc [PALMARES] : podium final 🥇🥈🥉 dans le flux.
 */
void				TournamentCLI::writePalmares(std::ofstream& out, const Tournament& tournament)
{
	const Phase*	final = tournament.getFinal();
	const Phase*	thirdPlace = tournament.getThirdPlace();

	out << "============================================================\n";
	out << "  PALMARES\n";
	out << "============================================================\n";

	if (!final || !final->isFinished())
	{
		out << "  Finale non terminée — palmarès indisponible.\n\n";
		return ;
	}

	CVP_TEAM		finalists = final->getWinners();
	CVP_TEAM		losers = final->getLosers();

	if (!finalists.empty() && finalists[0])
		out << "  1. (Or)    " << finalists[0]->getName() << "\n";

	if (!losers.empty() && losers[0])
		out << "  2. (Argent)" << losers[0]->getName() << "\n";

	if (thirdPlace && thirdPlace->isFinished())
	{
		CVP_TEAM	third = thirdPlace->getWinners();
		CVP_TEAM	fourth = thirdPlace->getLosers();

		if (!third.empty() && third[0])
			out << "  3. (Bronze)" << third[0]->getName() << "\n";

		if (!fourth.empty() && fourth[0])
			out << "  4.         " << fourth[0]->getName() << "\n";
	}

	out << "\n";
}

/********************/
/*	PUBLIC METHOD	*/
/********************/

void				TournamentCLI::displayMenu(Tournament& tournament)
{
	int				choice = 0;

	PrintUtils::clear();

	while (true)
	{
		std::cout << "\n========== TOURNOI : " << tournament.getSettings().getName() << " ==========" << std::endl;
		std::cout << "1. Poules (Scores/Classement)" << std::endl;
		std::cout << "2. Composition detaillee des Equipes par Poule\n";

		if (tournament.getHasSixteenth())
		{
			if (!tournament.getSixteenth())
				std::cout << "3. Generer les 1/16 de Finale\n";
			else
				std::cout << "3. Acceder aux 1/16 de Finale\n";
		}

		if (tournament.getHasHeighth())
		{
			if (!tournament.getHeighth())
				std::cout << "4. Generer les 1/8 de Finale\n";
			else
				std::cout << "4. Acceder aux 1/8 de Finale\n";
		}

		if (!tournament.getQuarters())
			std::cout << "5. Generer les Quarts de Finale\n";
		else
			std::cout << "5. Acceder aux Quarts de Finale\n";

		if (!tournament.getSemis())
			std::cout << "6. Generer les Demi-Finales\n";
		else
			std::cout << "6. Acceder aux Demi-Finales\n";

		if (!tournament.getFinal())
			std::cout << "7. Generer la Finale\n";
		else
			std::cout << "7. Acceder aux Finale\n";
		
		std::cout << "8. Afficher l'arbre complet du tournoi (Bracket)\n";
		std::cout << "9. Afficher le podium\n";
		std::cout << "──────────────────────────────────────────────────\n";
		std::cout << "E. Exporter une phase en .txt\n";
		std::cout << "T. Exporter le tournoi complet en .txt\n";
		std::cout << "──────────────────────────────────────────────────\n";
		std::cout << "q. Quitter le programme\n";
		std::cout << "====================================================\n";
		std::cout << "Votre choix : ";

		char		input;

		if (!(std::cin >> input))
		{
			clearInput();
			std::cout << "Saisie invalide. Veuillez entrer un nombre.\n";
			continue;
		}

		if (input == 'q')
			return ;

		if (input == 'E' || input == 'e')
		{
			// ── Export d'une phase ──────────────────────────────────────────
			std::cout << "\nQuelle phase exporter ?\n";
			std::cout << "  1. Poules entières\n";
			std::cout << "  2. 1/16 de Finale\n";
			std::cout << "  3. 1/8 de Finale\n";
			std::cout << "  4. Quarts de Finale\n";
			std::cout << "  5. Demi-Finales\n";
			std::cout << "  6. Finale\n";
			std::cout << "  7. Match pour la 3e Place\n";
			std::cout << "Choix : ";

			int		phaseChoice;

			if (!(std::cin >> phaseChoice))
			{
				clearInput();
				continue ;
			}

			clearInput();

			std::cout << "Nom du fichier de sortie (ex: demi_finales.txt) : ";

			STRING		filename;
			std::getline(std::cin, filename);

			if (filename.empty())
			{
				std::cout << "Nom de fichier vide, export annulé.\n";
				continue ;
			}

			if (phaseChoice == 1)
			{
				std::ofstream		poolFile(filename);

				if (!poolFile.is_open())
				{
					std::cout << "[!] Impossible de créer le fichier.\n";
					continue ;
				}

				for (const Pool* pool : tournament.getPools())
				{
					if (!pool)
						continue ;

					poolFile << "\n============================================================\n";
					poolFile << "  " << pool->getName() << "\n";
					poolFile << "============================================================\n";

					CVP_MATCH		matches = pool->getMatches();
					int				idx = 1;

					poolFile << "\n  [MATCHS]\n";

					for (const Match* m : matches)
					{
						if (!m)
							continue ;

						poolFile << "    " << std::setw(2) << idx++ << ". "
								 << m->getTeamA()->getName() << " vs " << m->getTeamB()->getName();

						if (m->isFinished())
						{
							poolFile << "  [ " << m->getScoreA() << " - " << m->getScoreB() << " ]";

							if (m->getWinner())
								poolFile << "  ->  " << m->getWinner()->getName();
						}
						else
							poolFile << "  [ À jouer ]";

						poolFile << "\n";
					}

					CVP_TEAM		teams = pool->getTeams();
					size_t			maxLen = 6;

					for (const Team* t : teams)
						if (t->getName().size() > maxLen)
							maxLen = t->getName().size();

					const int		w = static_cast<int>(maxLen) + 2;

					poolFile << "\n  [CLASSEMENT]\n";
					poolFile << "    " << std::left << std::setw(4) << "#"
							 << std::setw(w) << "Equipe"
							 << std::setw(6) << "Pts"
							 << "Diff\n";

					poolFile << "    " << STRING(4 + w + 12, '-') << "\n";

					for (size_t i = 0; i < teams.size(); ++i)
					{
						const Team*	t = teams[i];
						const int	diff = t->getScoreDiff();

						poolFile << "    " << std::left << std::setw(4) << (i + 1)
								 << std::setw(w) << t->getName()
								 << std::setw(6) << t->getPoint()
								 << (diff >= 0 ? "+" : "") << diff << "\n";
					}
				}

				poolFile.close();
				std::cout << "\033[1;32m[v] Poules exportées dans '" << filename << "'.\033[0m\n";
			}
			else
			{
				const Phase*		targetPhase = nullptr;

				switch (phaseChoice)
				{
					case 2:
						targetPhase = tournament.getSixteenth();
						break ;

					case 3:
						targetPhase = tournament.getHeighth();
						break ;

					case 4:
						targetPhase = tournament.getQuarters();
						break ;

					case 5:
						targetPhase = tournament.getSemis();
						break ;

					case 6:
						targetPhase = tournament.getFinal();
						break ;

					case 7:
						targetPhase = tournament.getThirdPlace();
						break ;

					default:
						std::cout << "Choix invalide.\n";
						continue ;
				}

				if (exportPhaseToTxt(targetPhase, filename))
					std::cout << "\033[1;32m[v] Phase exportée dans '" << filename << "'.\033[0m\n";
			}

			continue ;
		}

		if (input == 'T' || input == 't')
		{
			std::cout << "Nom du fichier de sortie (ex: tournoi_complet.txt) : ";
			clearInput();

			STRING	filename;

			std::getline(std::cin, filename);

			if (filename.empty())
			{
				std::cout << "Nom de fichier vide, export annulé.\n";
				continue ;
			}

			if (exportTournamentToTxt(tournament, filename))
				std::cout << "\033[1;32m[v] Tournoi exporté dans '" << filename << "'.\033[0m\n";

			continue ;
		}

		choice = input - '0';

		CVP_POOL	pools = tournament.getPools();

		switch (choice)
		{
			case 1:
			{
				if (pools.empty())
				{
					std::cout << "Aucune poule n'est disponible.\n";
					break ;
				}
				
				std::cout << "\n--- Liste des Poules ---\n";

				for (size_t i = 0; i < pools.size(); ++i)
					std::cout << (i + 1) << ". Poule " << (i + 1) << "\n";

				std::cout << (pools.size() + 1) << ". Retour\n";
				std::cout << "Choisissez une poule : ";
				
				size_t				pIdx;

				if (std::cin >> pIdx && pIdx >= 1 && pIdx <= pools.size())
					handleMatchList(pools[pIdx - 1]->getMatches(), "MATCHS POULE " + std::to_string(pIdx));
				else
					clearInput();

				break ;
			}

			case 2:
			{
				for (const auto& pool : tournament.getPools())
					PoolCLI::displayPoolDetails(*pool);
				
				break ;
			}

			case 3:
				if (tournament.getHasSixteenth())
				{
					if (!tournament.getSixteenth())
					{
						tournament.generateSixteenths();

						if (tournament.getSixteenth())
							std::cout << "1/16 de finale generes avec succes !\n";
						else
							std::cout << "Impossible : verifiez que les poules soient finies.\n";
					}
					else
						handlePhase(tournament.getSixteenth(), "1/16 DE FINALE");
				}
				break ;

			case 4:
				if (tournament.getHasHeighth())
				{
					if (!tournament.getHeighth())
					{
						tournament.generateHeighths();

						if (tournament.getHeighth())
							std::cout << "1/8 de finale generes avec succes !\n";
						else
							std::cout << "Impossible : verifiez que la phase precedente soit finie.\n";
					}
					else
						handlePhase(tournament.getHeighth(), "1/8 DE FINALE");
				}
				break ;

			case 5:
				if (!tournament.getQuarters())
				{
					tournament.generateQuarters();

					if (tournament.getQuarters())
						std::cout << "Quarts de finale generes avec succes !\n";
					else
						std::cout << "Impossible de generer : verifiez que les poules soient finies.\n";
				}
				else
					handlePhase(tournament.getQuarters(), "QUARTS DE FINALE");

				break ;

			case 6:
				if (!tournament.getSemis())
				{
					tournament.generateSemis();

					if (tournament.getSemis())
						std::cout << "Demi-finales generees avec succes !\n";
					else
						std::cout << "Impossible de generer : verifiez que les quarts soient finis.\n";
				}
				else
					handlePhase(tournament.getSemis(), "DEMI-FINALES");

				break ;

			case 7:
				if (!tournament.getFinal())
				{
					tournament.generateFinal();

					if (tournament.getFinal())
						std::cout << "Finale generee avec succes !\n";
					else
						std::cout << "Impossible de generer : verifiez que les demis soient finies.\n";
				}
				else
					handlePhase(tournament.getFinal(), "FINALE");

				break ;

			case 8:
				displayFullBracket(tournament);
				break ;

			default:
				std::cout << "Choix non disponible.\n";
				break ;
		}
	}
}

void				TournamentCLI::displayFullBracket(Tournament& tournament)
{
	Phase*			Q = tournament.getQuarters();
	Phase*			S = tournament.getSemis();
	Phase*			F = tournament.getFinal();
	Phase*			T = tournament.getThirdPlace();

	PrintUtils::clear();
	
	std::cout << "\n=================== ARBRE DE LA PHASE FINALE ===================\n\n";

	STRING			q1_1 = getTeamNameOrPlaceholder(Q, 0, 1);
	STRING			q1_2 = getTeamNameOrPlaceholder(Q, 0, 2);
	STRING			q2_1 = getTeamNameOrPlaceholder(Q, 1, 1);
	STRING			q2_2 = getTeamNameOrPlaceholder(Q, 1, 2);
	STRING			q3_1 = getTeamNameOrPlaceholder(Q, 2, 1);
	STRING			q3_2 = getTeamNameOrPlaceholder(Q, 2, 2);
	STRING			q4_1 = getTeamNameOrPlaceholder(Q, 3, 1);
	STRING			q4_2 = getTeamNameOrPlaceholder(Q, 3, 2);

	STRING			s1_1 = getTeamNameOrPlaceholder(S, 0, 1);
	STRING			s1_2 = getTeamNameOrPlaceholder(S, 0, 2);
	STRING			s2_1 = getTeamNameOrPlaceholder(S, 1, 1);
	STRING			s2_2 = getTeamNameOrPlaceholder(S, 1, 2);

	STRING			f1_1 = getTeamNameOrPlaceholder(F, 0, 1);
	STRING			f1_2 = getTeamNameOrPlaceholder(F, 0, 2);

	STRING			t1_1 = getTeamNameOrPlaceholder(T, 0, 1);
	STRING			t1_2 = getTeamNameOrPlaceholder(T, 0, 2);

	STRING			winner = "A determiner";

	if (F && F->isFinished() && !F->getMatches().empty() && F->getMatches()[0]->isFinished())
	{
		Team* w = F->getMatches()[0]->getWinner();

		if (w)winner = w->getName();
	}

	std::cout << "\tQUARTS\t\t\t\tDEMIS\t\t\t3e PLACE\t\tFINALE\t\t\tVAINQUEUR\n\n";
	std::cout << "\t" << q1_1 << " ---\n";
	std::cout << "\t\t\t\t|---> " << s1_1 << " ---\n";
	std::cout << "\t" << q1_2 << " ---\n";
	std::cout << "\t\t\t\t\t\t\t\t\t\t|---> " << f1_1 << " ---\n";
	std::cout << "\t" << q2_1 << " ---\n";
	std::cout << "\t\t\t\t|---> " << s1_2 << " ---\n";
	std::cout << "\t" << q2_2 << " ---\n";
	std::cout << "\t\t\t\t\t\t\t\t|--> " << t1_1 << " ---\n";
	std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t|---> " << winner << "\n";
	std::cout << "\t\t\t\t\t\t\t\t|--> " << t1_2 << " ---\n";
	std::cout << "\t" << q3_1 << " ---\n";
	std::cout << "\t\t\t\t|---> " << s2_1 << " ---\n";
	std::cout << "\t" << q3_2 << " ---\n";
	std::cout << "\t\t\t\t\t\t\t\t\t\t|---> " << f1_2 << " ---\n";
	std::cout << "\t" << q4_1 << " ---\n";
	std::cout << "\t\t\t\t|---> " << s2_2 << " ---\n";
	std::cout << "\t" << q4_2 << " ---\n";
	std::cout << "==============================================================================\n";

}

/**
 * Affiche le podium final dans le terminal avec couleurs ANSI.
 */
void				TournamentCLI::displayPodium(const Tournament& tournament)
{
	const Phase*	final = tournament.getFinal();
	const Phase*	thirdPlace = tournament.getThirdPlace();

	std::cout << "\n╔══════════════════════════════════════╗\n";
	std::cout << "║            PALMARES FINAL            ║\n";
	std::cout << "╚══════════════════════════════════════╝\n";

	if (!final || !final->isFinished())
	{
		std::cout << "\033[1;33m  La finale n'est pas encore terminée.\033[0m\n";
		return ;
	}

	CVP_TEAM		finalists = final->getWinners();
	CVP_TEAM		losers = final->getLosers();

	if (!finalists.empty() && finalists[0])
		std::cout << "\033[1;33m  🥇  1er : " << finalists[0]->getName() << "\033[0m\n";

	if (!losers.empty() && losers[0])
		std::cout << "\033[0;37m  🥈  2e  : " << losers[0]->getName() << "\033[0m\n";

	if (thirdPlace && thirdPlace->isFinished())
	{
		CVP_TEAM	third = thirdPlace->getWinners();
		CVP_TEAM	fourth = thirdPlace->getLosers();

		if (!third.empty() && third[0])
			std::cout << "\033[0;33m  🥉  3e  : " << third[0]->getName() << "\033[0m\n";

		if (!fourth.empty() && fourth[0])
			std::cout << "       4e  : " << fourth[0]->getName() << "\n";
	}

	std::cout << "══════════════════════════════════════\n";
}

/**
 * Exporte une phase dans un fichier TXT en deleguant a PhaseCLI.
 */
bool				TournamentCLI::exportPhaseToTxt(const Phase* phase, C_STRING filename)
{
	if (!phase)
	{
		std::cerr << "\033[1;31m[!] Phase inexistante ou non générée — export annulé.\033[0m\n";
		return (false);
	}

	return (PhaseCLI::exportToTxt(*phase, filename));
}

/**
 * Exporte le tournoi complet (poules + toutes les phases) dans un fichier TXT.
 */
bool				TournamentCLI::exportTournamentToTxt(const Tournament& tournament, C_STRING filename)
{
	std::ofstream	out(filename);

	if (!out.is_open())
	{
		std::cerr << "\033[1;31m[!] Impossible de créer le fichier : " << filename << "\033[0m\n";
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
