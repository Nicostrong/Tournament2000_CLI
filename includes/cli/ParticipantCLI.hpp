//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#ifndef PARTICIPANTSCLI_HPP
# define PARTICIPANTSCLI_HPP

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include "../Global.hpp"

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
class ParticipantCLI
{
	private:

		static vpPart				partList;

		//	MENU
		static void					menuParticipant(cvpPart participants, cSet settings, bool showMenu);

		//	SUBMENU
		static Participant*			create(cvpPart participants, cSet settings);
		static void					destroy(size_t id, vpPart& participants);
		static void					modify(size_t id, cvpPart participants, cSet settings);
		static void					displayOne(cPart p);
		static void					displayAll(cvpPart participants);

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

	public:

		static void					handleMenuParticipant(vpPart& participants, cSet settings);
};

std::ostream&		operator<<(std::ostream& os, cPart p);

#endif
