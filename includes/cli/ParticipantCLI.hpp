//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#ifndef PARTICIPANTSCLI_HPP
# define PARTICIPANTSCLI_HPP

//	STDLIB
# include <vector>

//	INCLUDES
# include "../class/Participant.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;
typedef std::vector<Participant>	V_PART;

//	STATIC VARIABLES

class ParticipantCLI
{
	public:

		static Participant			create();
		static void					print(const Participant& participant);
		static STRING				trim(STRING s);
		static V_PART				importFromCSV(C_STRING filename);
		static bool					exportToCSV(const V_PART& participants, C_STRING filename);
};

#endif
