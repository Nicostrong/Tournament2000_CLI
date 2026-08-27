//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#pragma once

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

# include <string>
# include <vector>

/****************************************************************************************************/
/*	CLASSES																							*/
/****************************************************************************************************/

class				Settings;
class				Participant;

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				String			=	std::string;
using				cString			=	const std::string&;

using				pSet			=	Settings*;
using				cSet			=	const Settings&;
using				cpSet			=	const Settings*;

using				pPart			=	Participant*;
using				cPart			=	const Participant&;
using				cpPart			=	const Participant*;
using				vpPart			=	std::vector<Participant*>;
using				cvpPart			=	const std::vector<Participant*>&;

/****************************************************************************************************/
/*	STATIC VARIABLES																				*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	CLASS																							*/
/****************************************************************************************************/

/**
 * Gere la representation de l objet Participants en CLI
 * Gere les menus pour la gestion des participants
 */
class				ParticipantCLI
{
	private:

		static vpPart				_partList;
		static bool					_canAdd;
		static bool					_hasParticipants;
		static bool					_enoughPlayers;

		//	MENU
		static void					menuParticipant(cvpPart participants, cSet settings, bool showMenu);
		static void					executeChoice(int choice, vpPart& participants, cSet settings);

		//	SUBMENU
		static Participant*			create(cvpPart participants, cSet settings);
		static void					destroy(size_t id, vpPart& participants);
		static void					modify(size_t id, cvpPart participants, cSet settings);

		//	IMPORT
		static vpPart				importFromCSV(cString filename);

		//	EXPORT
		static bool					exportToCSV(cvpPart participants, cString filename);

		//	HANDLER
		static void					handleAddParticipant(vpPart& participants, cSet settings);
		static void					handleModifyParticipant(cvpPart participants, cSet settings);
		static void					handleDeleteParticipant(vpPart& participants);
		static void					handleImport(vpPart& participants, cSet settings);
		static void					handleExport(cvpPart participants);
		static void					handledisplay(cvpPart participants);
		static void					handleList();
		static void					handleTitle();

		//	HELPER
		static bool					checkPseudo(cString pseudo, cvpPart participants);
		static Participant*			extractParticipantFromLine(cString line, bool isFirstLine);
		static void					updateState(cvpPart participants, cSet settings);
		static Gender				askGender(cSet settings, int currentGenderInt = -1);

	public:

		static void					handleMenuParticipant(vpPart& participants, cSet settings);

};
