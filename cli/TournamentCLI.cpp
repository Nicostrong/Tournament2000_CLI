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

/************************************************/
/*  HELPERS INTERNES — utilitaires sans état	*/
/************************************************/

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

	STRING			filename;

	std::getline(std::cin, filename);

	if (filename.empty())
		std::cout << "[!] Nom de fichier vide — opération annulée.\n";

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

	Team*			t = (teamPos == 1)	? matches[matchIdx]->getTeamA()
										: matches[matchIdx]->getTeamB();

	return (t ? t->getName() : "A determiner");
}

/************************/
/*  AFFICHAGE DU MENU	*/
/************************/

/**
 * Affiche les options du menu en fonction de l etat courant du tournoi.
 */
void				TournamentCLI::printMenu(const Tournament& tournament)
{
	std::cout << "\n========== TOURNOI : " << tournament.getSettings().getName() << " ==========\n";
	std::cout << "1. Poules (Scores / Classement)\n";
	std::cout << "2. Composition des equipes par Poule\n";

	if (tournament.getHasSixteenth())
		std::cout << "3. " << (tournament.getSixteenth() ? "Acceder aux" : "Generer les") << " 1/16 de Finale\n";

	if (tournament.getHasHeighth())
		std::cout << "4. " << (tournament.getHeighth() ? "Acceder aux" : "Generer les") << " 1/8 de Finale\n";

	std::cout << "5. " << (tournament.getQuarters() ? "Acceder aux" : "Generer les") << " Quarts de Finale\n";
	std::cout << "6. " << (tournament.getSemis()    ? "Acceder aux" : "Generer les") << " Demi-Finales\n";
	std::cout << "7. " << (tournament.getFinal()    ? "Acceder a la" : "Generer la") << " Finale\n";
	std::cout << "8. Afficher l'arbre complet (Bracket)\n";
	std::cout << "9. Afficher le podium\n";
	std::cout << "──────────────────────────────────\n";
	std::cout << "E. Exporter une phase en .txt\n";
	std::cout << "T. Exporter le tournoi complet en .txt\n";
	std::cout << "──────────────────────────────────\n";
	std::cout << "q. Quitter\n";
	std::cout << "==================================\n";
	std::cout << "Votre choix : ";
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
		std::cout << "Selectionnez un match : ";

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

/**
 * Affiche une phase si elle existe, sinon informe l utilisateur.
 */
void				TournamentCLI::handlePhase(Phase* phase, C_STRING phaseName)
{
	if (phase == nullptr)
	{
		std::cout << "Cette phase n'est pas encore active.\n";
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
	const VP_POOL	pools = tournament.getPools();

	if (pools.empty())
	{
		std::cout << "Aucune poule disponible.\n";
		return ;
	}

	std::cout << "\n--- Liste des Poules ---\n";

	for (size_t i = 0; i < pools.size(); ++i)
		std::cout << (i + 1) << ". " << pools[i]->getName() << "\n";

	std::cout << (pools.size() + 1) << ". Retour\n";
	std::cout << "Choisissez une poule : ";

	size_t			pIdx;

	if (!(std::cin >> pIdx) || pIdx < 1 || pIdx > pools.size())
	{
		clearInput();
		return ;
	}

	const STRING	title = "MATCHS " + pools[pIdx - 1]->getName();

	handleMatchList(pools[pIdx - 1]->getMatches(), title);
}

/**
 * Factorise le pattern commun à toutes les phases eliminatoires :
 *   - si phase == nullptr → appelle generateFn(), affiche succes ou erreur
 *   - sinon → affiche les matchs via handlePhase()
 */
void				TournamentCLI::handleEliminationPhase(Phase* phase,
															const std::function<void()>& generateFn,
															C_STRING phaseName,
															C_STRING successMsg,
															C_STRING errorMsg)
{
	if (!phase)
	{
		generateFn();

		std::cout << (phase ? successMsg : errorMsg) << "\n";
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
	std::ofstream	out(filename);

	if (!out.is_open())
	{
		std::cerr << "[!] Impossible de créer le fichier : " << filename << "\n";
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
	std::cout << "\nQuelle phase exporter ?\n";
	std::cout << "  1. Poules\n";
	std::cout << "  2. 1/16 de Finale\n";
	std::cout << "  3. 1/8 de Finale\n";
	std::cout << "  4. Quarts de Finale\n";
	std::cout << "  5. Demi-Finales\n";
	std::cout << "  6. Finale\n";
	std::cout << "  7. Match pour la 3e Place\n";
	std::cout << "Choix : ";

	int			phaseChoice;

	if (!(std::cin >> phaseChoice))
	{
		clearInput();
		return ;
	}

	clearInput();

	const STRING	filename = promptFilename("Nom du fichier de sortie : ");

	if (filename.empty())
		return ;

	bool			ok = false;

	if (phaseChoice == 1)
		ok = exportPoolsToFile(tournament, filename);
	else
	{
		const Phase*	target = getPhaseByMenuChoice(tournament, phaseChoice);

		if (!target)
		{
			std::cout << "[!] Phase non disponible ou non encore générée.\n";
			return ;
		}

		ok = exportPhaseToTxt(target, filename);
	}

	if (ok)
		std::cout << "\033[1;32m[v] Export réussi dans '" << filename << "'.\033[0m\n";
}

/****************************/
/*  PUBLIC — displayMenu	*/
/****************************/

/**
 * Point d entree du tournoi.
 */
void				TournamentCLI::displayMenu(Tournament& tournament)
{
	PrintUtils::clear();

	while (true)
	{
		printMenu(tournament);

		char		input;

		if (!(std::cin >> input))
		{
			clearInput();
			std::cout << "Saisie invalide.\n";
			continue ;
		}

		if (input == 'q' || input == 'Q')
			return ;

		if (input == 'E' || input == 'e')
		{
			handleExportPhase(tournament);
			continue ;
		}

		if (input == 'T' || input == 't')
		{
			clearInput();

			const STRING			filename = promptFilename("Nom du fichier de sortie : ");

			if (!filename.empty() && exportTournamentToTxt(tournament, filename))
				std::cout << "\033[1;32m[v] Tournoi exporté dans '" << filename << "'.\033[0m\n";

			continue ;
		}

		const int	choice = input - '0';

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
				if (tournament.getHasSixteenth())
					handleEliminationPhase(
						tournament.getSixteenth(),
						[&]() { tournament.generateSixteenths(); },
						"1/16 DE FINALE",
						"1/16 generes avec succes !",
						"Impossible : verifiez que les poules soient terminees."
					);
				break ;

			case 4:
				if (tournament.getHasHeighth())
					handleEliminationPhase(
						tournament.getHeighth(),
						[&]() { tournament.generateHeighths(); },
						"1/8 DE FINALE",
						"1/8 generes avec succes !",
						"Impossible : verifiez que la phase precedente soit terminee."
					);
				break ;

			case 5:
				handleEliminationPhase(
					tournament.getQuarters(),
					[&]() { tournament.generateQuarters(); },
					"QUARTS DE FINALE",
					"Quarts de finale generes avec succes !",
					"Impossible : verifiez que les poules soient terminees."
				);
				break ;

			case 6:
				handleEliminationPhase(
					tournament.getSemis(),
					[&]() { tournament.generateSemis(); },
					"DEMI-FINALES",
					"Demi-finales generees avec succes !",
					"Impossible : verifiez que les quarts soient termines."
				);
				break ;

			case 7:
				handleEliminationPhase(
					tournament.getFinal(),
					[&]() { tournament.generateFinal(); },
					"FINALE",
					"Finale generee avec succes !",
					"Impossible : verifiez que les demi-finales soient terminees."
				);
				break ;

			case 8:
				displayFullBracket(tournament);
				break ;

			case 9:
				displayPodium(tournament);
				break ;

			default:
				std::cout << "Choix non disponible.\n";
				break ;
		}
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
	const Phase*	Q = tournament.getQuarters();
	const Phase*	S = tournament.getSemis();
	const Phase*	F = tournament.getFinal();
	const Phase*	T = tournament.getThirdPlace();

	PrintUtils::clear();

	std::cout << "\n=================== ARBRE DE LA PHASE FINALE ===================\n\n";
	std::cout << "\tQUARTS\t\t\tDEMIS\t\t\t\t\t3e PLACE\tFINALE\t\t\tVAINQUEUR\n\n";

	const STRING	q[4][2] = {
		{ getTeamNameOrPlaceholder(Q, 0, 1), getTeamNameOrPlaceholder(Q, 0, 2) },
		{ getTeamNameOrPlaceholder(Q, 1, 1), getTeamNameOrPlaceholder(Q, 1, 2) },
		{ getTeamNameOrPlaceholder(Q, 2, 1), getTeamNameOrPlaceholder(Q, 2, 2) },
		{ getTeamNameOrPlaceholder(Q, 3, 1), getTeamNameOrPlaceholder(Q, 3, 2) }
	};
	const STRING	s[2][2] = {
		{ getTeamNameOrPlaceholder(S, 0, 1), getTeamNameOrPlaceholder(S, 0, 2) },
		{ getTeamNameOrPlaceholder(S, 1, 1), getTeamNameOrPlaceholder(S, 1, 2) }
	};
	const STRING	f[2]    = {
		getTeamNameOrPlaceholder(F, 0, 1),
		getTeamNameOrPlaceholder(F, 0, 2)
	};
	const STRING	t3[2]   = {
		getTeamNameOrPlaceholder(T, 0, 1),
		getTeamNameOrPlaceholder(T, 0, 2)
	};

	STRING			winner = "A determiner";

	if (F && F->isFinished() && !F->getMatches().empty())
	{
		const Team*	w = F->getMatches()[0]->getWinner();

		if (w)
			winner = w->getName();
	}

	const bool		hasThird = tournament.getSettings().getIsThirdPlaceMatch();

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

/**
 * Affiche le podium final avec couleurs ANSI.
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
		std::cout << "\033[1;33m  La finale n'est pas encore terminee.\033[0m\n";
		return ;
	}

	CVP_TEAM		winners = final->getWinners();
	CVP_TEAM		losers = final->getLosers();

	if (!winners.empty() && winners[0])
		std::cout << "\033[1;33m  1er : " << winners[0]->getName() << "\033[0m\n";

	if (!losers.empty() && losers[0])
		std::cout << "\033[0;37m  2e  : " << losers[0]->getName() << "\033[0m\n";

	if (thirdPlace && thirdPlace->isFinished())
	{
		CVP_TEAM	third = thirdPlace->getWinners();
		CVP_TEAM	fourth = thirdPlace->getLosers();

		if (!third.empty()  && third[0])
			std::cout << "\033[0;33m  3e  : " << third[0]->getName()  << "\033[0m\n";

		if (!fourth.empty() && fourth[0])
			std::cout << "  4e  : " << fourth[0]->getName() << "\n";
	}

	std::cout << "══════════════════════════════════════\n";
}

/****************************/
/*  PUBLIC — Export fichier	*/
/****************************/

/**
 * Exporte une phase dans un fichier .txt via PhaseCLI.
 */
bool				TournamentCLI::exportPhaseToTxt(const Phase* phase, C_STRING filename)
{
	if (!phase)
	{
		std::cerr << "[!] Phase inexistante ou non generee — export annule.\n";
		return (false);
	}

	return (PhaseCLI::exportToTxt(*phase, filename));
}

/**
 * Exporte le tournoi complet en .txt.
 * Sequence fixe : en-tête → poules → phases → palmarès.
 */
bool				TournamentCLI::exportTournamentToTxt(const Tournament& tournament, C_STRING filename)
{
	std::ofstream	out(filename);

	if (!out.is_open())
	{
		std::cerr << "[!] Impossible de creer le fichier : " << filename << "\n";
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

/**
 * Ecrit l en-tete du fichier export (nom + settings essentiels).
 */
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

/**
 * Ecrit le bloc [POULES] : pour chaque poule, matchs + classement.
 */
void				TournamentCLI::writePools(std::ofstream& out, const Tournament& tournament)
{
	const VP_POOL	pools = tournament.getPools();

	out << "============================================================\n";
	out << "\tPHASE DE POULES\n";
	out << "============================================================\n";

	if (pools.empty())
	{
		out << "\tAucune poule enregistree.\n\n";
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

/**
 * Ecrit la liste des matchs d une poule dans out.
 */
void				TournamentCLI::writePoolMatches(std::ofstream& out, const Pool& pool)
{
	out << "\n\t[MATCHS]\n";

	int				idx = 1;

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

/**
 * Ecrit le tableau de classement d une poule dans out.
 */
void				TournamentCLI::writePoolStandings(std::ofstream& out, const Pool& pool)
{
	CVP_TEAM		teams = pool.getTeams();
	size_t			maxLen = 6;

	for (const Team* t : teams)
		if (t->getName().size() > maxLen)
			maxLen = t->getName().size();

	const int		w = static_cast<int>(maxLen) + 2;

	out << "\n\t[CLASSEMENT]\n";
	out << "\t\t" << std::left << std::setw(4) << "#"
		<< std::setw(w) << "Equipe"
		<< std::setw(6) << "Pts"
		<< "Diff\n";
	out << "\t\t" << STRING(4 + w + 12, '-') << "\n";

	for (size_t i = 0; i < teams.size(); ++i)
	{
		const int	diff = teams[i]->getScoreDiff();

		out << "\t\t" << std::left << std::setw(4) << (i + 1)
			<< std::setw(w) << teams[i]->getName()
			<< std::setw(6) << teams[i]->getPoint()
			<< (diff >= 0 ? "+" : "") << diff << "\n";
	}
}

/**
 * Ecrit le bloc d une phase eliminatoire dans out.
 */
void				TournamentCLI::writePhaseBlock(std::ofstream& out, const Phase* phase)
{
	if (!phase)
		return ;

	CVP_MATCH		matches = phase->getMatches();
	const int		nbSets = phase->getNbSetToPlay();

	out << "============================================================\n";
	out << "\t" << phase->getName() << "  (" << nbSets << " set(s) par rencontre)\n";
	out << "============================================================\n";

	if (matches.empty())
	{
		out << "\tAucun match enregistre.\n\n";
		return ;
	}

	int				encounterNum = 1;

	for (size_t i = 0; i < matches.size(); i += static_cast<size_t>(nbSets))
		writeEncounterBlock(out, matches, i, nbSets, encounterNum++);

	if (phase->isFinished())
		writePhaseResults(out, *phase);

	out << "\n";
}

/**
 * Ecrit une rencontre (N sets consecutifs) dans out.
 */
void				TournamentCLI::writeEncounterBlock(std::ofstream& out,
														CVP_MATCH matches,
														const size_t startIdx,
														const int nbSets,
														const int encounterNum)
{
	if (startIdx >= matches.size() || !matches[startIdx])
		return ;

	const Match*	first = matches[startIdx];
	C_STRING		nameA = first->getTeamA() ? first->getTeamA()->getName() : "?";
	C_STRING		nameB = first->getTeamB() ? first->getTeamB()->getName() : "?";

	out << "\n\tRencontre " << encounterNum << " :  " << nameA << "  vs  " << nameB << "\n";
	out << "\t" << STRING(nameA.size() + nameB.size() + 14, '-') << "\n";

	int				winsA = 0;
	int				winsB = 0;

	for (int s = 0; s < nbSets; ++s)
	{
		const size_t				idx = startIdx + static_cast<size_t>(s);

		if (idx >= matches.size() || !matches[idx])
			break ;

		const Match*				m = matches[idx];

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

/**
 * Ecrit le rEcapitulatif qualifiEs/EliminEs d une phase terminee dans out.
 */
void				TournamentCLI::writePhaseResults(std::ofstream& out, const Phase& phase)
{
	CVP_TEAM		winners = phase.getWinners();
	CVP_TEAM		losers = phase.getLosers();

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

/**
 * Ecrit le bloc [PALMARES] dans out.
 */
void				TournamentCLI::writePalmares(std::ofstream& out, const Tournament& tournament)
{
	const Phase*	final = tournament.getFinal();
	const Phase*	thirdPlace = tournament.getThirdPlace();

	out << "============================================================\n";
	out << "\tPALMARES\n";
	out << "============================================================\n";

	if (!final || !final->isFinished())
	{
		out << "\tFinale non terminee — palmares indisponible.\n\n";
		return ;
	}

	CVP_TEAM		winners = final->getWinners();
	CVP_TEAM		losers  = final->getLosers();

	if (!winners.empty() && winners[0])
		out << "\t1. (Or)\t\t" << winners[0]->getName() << "\n";

	if (!losers.empty()  && losers[0])
		out << "\t2. (Argent)\t" << losers[0]->getName()  << "\n";

	if (thirdPlace && thirdPlace->isFinished())
	{
		CVP_TEAM	third = thirdPlace->getWinners();
		CVP_TEAM	fourth = thirdPlace->getLosers();

		if (!third.empty()  && third[0])
			out << "\t3. (Bronze)\t" << third[0]->getName()  << "\n";

		if (!fourth.empty() && fourth[0])
			out << "\t4.\t\t\t" << fourth[0]->getName() << "\n";
	}

	out << "\n";
}
