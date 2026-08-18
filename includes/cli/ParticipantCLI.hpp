//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#ifndef PARTICIPANTSCLI_HPP
# define PARTICIPANTSCLI_HPP

//	STDLIB
# include <vector>
# include <tuple>
# include <string>

//	INCLUDES
# include "../class/Participant.hpp"
# include "../class/Settings.hpp"

//	TYPEDEF
using				STRING		=	std::string;
using				C_STRING	=	const std::string&;
using				VSTRING		=	std::vector<std::string>;
using				VP_PART		=	std::vector<Participant*>;
using				CVP_PART	=	const std::vector<Participant*>&;
using				CV_PART		=	const std::vector<Participant>&;

//	STATIC VARIABLES

/**
 * Gere la representation de l objet Participants en CLI
 * Gere les menus pour la gestion des participants
 */
class ParticipantCLI
{
	private:

		static VP_PART				partList;

		//	MENU
		static void					menuParticipant(CVP_PART participants, const Settings& settings, bool showMenu);

		//	SUBMENU
		static Participant*			create(CVP_PART participants, const Settings& settings);
		static void					destroy(size_t id, VP_PART& participants);
		static void					modify(size_t id, CVP_PART participants, const Settings& settings);
		static void					displayOne(const Participant& p);
		static void					displayAll(CVP_PART participants);

		//	IMPORT
		static VP_PART				importFromCSV(C_STRING filename);

		//	EXPORT
		static bool					exportToCSV(CVP_PART participants, C_STRING filename);

		//	HANDLER
		static void					handleAddParticipant(VP_PART& participants, const Settings& settings);
		static void					handleModifyParticipant(CVP_PART& participants, const Settings& settings);
		static void					handleDeleteParticipant(VP_PART& participants);
		static void					handleImport(VP_PART& participants, const Settings& settings);
		static void					handleExport(CVP_PART participants);
		static void					handledisplay(CVP_PART participants);
		static void					handleList();
		static void					handleTitle();

		//	HELPER
		static bool					checkPseudo(C_STRING pseudo, CVP_PART participants);
		static Participant*			extractParticipantFromLine(C_STRING line, bool isFirstLine);

	public:

		static void					handleMenuParticipant(VP_PART& participants, const Settings& settings);
};

std::ostream&		operator<<(std::ostream& os, const Participant& p);

#endif
