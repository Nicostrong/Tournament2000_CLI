//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>
#include <iostream>
#include <algorithm>

#include "../includes/class/Settings.hpp"
#include "../includes/class/Player.hpp"

#include "../includes/utils/PrintUtils.hpp"

#include "../includes/cli/PlayerCLI.hpp"
#include "../includes/viewer/PlayerViewer.hpp"

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

void				PlayerViewer::displayOne(cPlayer p)
{
	std::cout << "------------------------------------------------\n";
	std::cout << "|  Player\t" << p.getId() << "\n";
	std::cout << "------------------------------------------------\n";
	std::cout << "|  Pseudo       : " << p.getPseudo() << std::endl;
	std::cout << "|  Nom          : " << p.getLastName() << std::endl;
	std::cout << "|  Prenom       : " << p.getFirstName() << std::endl;
	std::cout << "|  Genre        : " << p.getGenderStr() << std::endl;
	std::cout << "|  Elimine      : " << (p.getIsEliminated() ? "Oui" : "Non") << std::endl;
	std::cout << "|  Multi-equipe : " << (p.getIsMultiTeamPlayer() ? "Oui" : "Non") << std::endl;
	std::cout << "------------------------------------------------\n";
}

void				PlayerViewer::displayAll(cvpPlayer participants)
{
	if (participants.empty())
	{
		PrintUtils::addError("Aucun participant enregistre.");
		return;
	}

	std::cout << "\n=============== LISTE DES PARTICIPANTS  (" << participants.size() << ") ===============\n";

	size_t wId = 2;
	size_t wPseudo = 6;
	size_t wNom = 3;
	size_t wPrenom = 6;
	size_t wGenre = 5;
	size_t wElim = 7;
	size_t wMulti = 5;

	for (cpPlayer p : participants)
	{
		wId = std::max(wId, std::to_string(p->getId()).length());
		wPseudo = std::max(wPseudo, p->getPseudo().length());
		wNom = std::max(wNom, p->getLastName().length());
		wPrenom = std::max(wPrenom, p->getFirstName().length());
		wGenre = std::max(wGenre, p->getGenderStr().length());
	}

	wId += 2; wPseudo += 2; wNom += 2; wPrenom += 2; wGenre += 2; wElim += 2; wMulti += 2;

	auto printSeparator = [&]() {
		std::cout << '+' << String(wId, '-')
				  << '+' << String(wPseudo, '-')
				  << '+' << String(wNom, '-')
				  << '+' << String(wPrenom, '-')
				  << '+' << String(wGenre, '-')
				  << '+' << String(wElim, '-')
				  << '+' << String(wMulti, '-') << "+\n";
	};

	printSeparator();
	std::cout << std::format("|{:<{}}|{:<{}}|{:<{}}|{:<{}}|{:<{}}|{:<{}}|{:<{}}|\n",
		" ID", wId,
		" Pseudo", wPseudo,
		" Nom", wNom,
		" Prenom", wPrenom,
		" Genre", wGenre,
		" Elimine", wElim,
		" Multi", wMulti);
	printSeparator();

	for (cpPlayer p : participants)
	{
		String strElim = p->getIsEliminated() ? "Oui" : "Non";
		String strMulti = p->getIsMultiTeamPlayer() ? "Oui" : "Non";

		std::cout << std::format("| {:<{}}| {:<{}}| {:<{}}| {:<{}}| {:<{}}| {:<{}}| {:<{}}|\n",
			p->getId(), wId - 1,
			p->getPseudo(), wPseudo - 1,
			p->getLastName(), wNom - 1,
			p->getFirstName(), wPrenom - 1,
			p->getGenderStr(), wGenre - 1,
			strElim, wElim - 1,
			strMulti, wMulti - 1);
	}
	printSeparator();
	std::cout << std::endl;
}
