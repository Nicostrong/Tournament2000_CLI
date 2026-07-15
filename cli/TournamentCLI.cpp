//
// Created by Nicolas Fordoxcel on 15/06/2026.
//

//	STDLIB
#include <iostream>

//	INCLUDES
#include "../includes/TournamentCLI.hpp"
#include "../includes/MatchCLI.hpp"
#include "../includes/PoolCLI.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef std::vector<Pool*>			V_POOL;
typedef std::vector<Match*>			V_MATCH;

//	STATIC VARIABLES

/********************/
/*	PRIVATE METHOD	*/
/********************/

void								TournamentCLI::handlePhase(Phase* phase, C_STRING phaseName)
{
	if (!phase)
	{
		std::cout << "Cette phase n'est pas encore active.\n";
		return ;
	}
	
	handleMatchList(phase->getMatches(), phaseName);
}

void								TournamentCLI::handleMatchList(const V_MATCH& matches, C_STRING title)
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

STRING								TournamentCLI::getTeamNameOrPlaceholder(Phase* phase, size_t matchIdx, int teamPos)
{
	if (!phase)
		return ("A determiner");

	const V_MATCH& matches = phase->getMatches();

	if (matchIdx >= matches.size() || !matches[matchIdx])
		return ("A determiner");

	Team* t = (teamPos == 1) ? matches[matchIdx]->getTeamA() : matches[matchIdx]->getTeamB();
	
	if (t) 
		return (t->getName());
		
	return ("A determiner");
}

void								TournamentCLI::clearInput()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/********************/
/*	PUBLIC METHOD	*/
/********************/

void								TournamentCLI::displayMenu(Tournament& tournament)
{
	int	choice = 0;

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
		std::cout << "0. Retour au menu principal\n";
		std::cout << "====================================================\n";
		std::cout << "Votre choix : ";

		if (!(std::cin >> choice))
		{
			clearInput();
			std::cout << "Saisie invalide. Veuillez entrer un nombre.\n";
			continue;
		}

		if (choice == 0)
			break;

		switch (choice)
		{
			case 1:
			{
				const V_POOL& pools = tournament.getPools();
				
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
				
				size_t pIdx;

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

void								TournamentCLI::displayFullBracket(Tournament& tournament)
{
	Phase*	Q = tournament.getQuarters();
	Phase*	S = tournament.getSemis();
	Phase*	F = tournament.getFinal();

	std::cout << "\n=================== ARBRE DE LA PHASE FINALE ===================\n\n";

	STRING	q1_1 = getTeamNameOrPlaceholder(Q, 0, 1);
	STRING	q1_2 = getTeamNameOrPlaceholder(Q, 0, 2);
	STRING	q2_1 = getTeamNameOrPlaceholder(Q, 1, 1);
	STRING	q2_2 = getTeamNameOrPlaceholder(Q, 1, 2);
	STRING	q3_1 = getTeamNameOrPlaceholder(Q, 2, 1);
	STRING	q3_2 = getTeamNameOrPlaceholder(Q, 2, 2);
	STRING	q4_1 = getTeamNameOrPlaceholder(Q, 3, 1);
	STRING	q4_2 = getTeamNameOrPlaceholder(Q, 3, 2);

	STRING	s1_1 = getTeamNameOrPlaceholder(S, 0, 1);
	STRING	s1_2 = getTeamNameOrPlaceholder(S, 0, 2);
	STRING	s2_1 = getTeamNameOrPlaceholder(S, 1, 1);
	STRING	s2_2 = getTeamNameOrPlaceholder(S, 1, 2);

	STRING	f1_1 = getTeamNameOrPlaceholder(F, 0, 1);
	STRING	f1_2 = getTeamNameOrPlaceholder(F, 0, 2);

	STRING	winner = "A determiner";

	if (F && F->isFinished() && !F->getMatches().empty())
	{
		if (F->getMatches()[0]->isFinished())
		{
			Team* w = F->getMatches()[0]->getWinner();

			if (w)
				winner = w->getName();
		}
	}

	std::cout << "	QUARTS DE FINALE			DEMI-FINALES				FINALE				VAINQUEUR\n\n";
	std::cout << "	" << q1_1 << " ---\n";
	std::cout << "						|---> " << s1_1 << " ---\n";
	std::cout << "	" << q1_2 << " ---\n";
	std::cout << "                                              |---> " << f1_1 << " ---\n";
	std::cout << "	" << q2_1 << " ---\n";
	std::cout << "						|---> " << s1_2 << " ---\n";
	std::cout << "	" << q2_2 << " ---\n";
	std::cout << "																	|---> " << winner << "\n";
	std::cout << "	" << q3_1 << " ---\n";
	std::cout << "						|---> " << s2_1 << " ---\n";
	std::cout << "	" << q3_2 << " ---\n";
	std::cout << "												|---> " << f1_2 << " ---\n";
	std::cout << "	" << q4_1 << " ---\n";
	std::cout << "						|---> " << s2_2 << " ---\n";
	std::cout << "	" << q4_2 << " ---\n";
	std::cout << "==============================================================================\n";
}