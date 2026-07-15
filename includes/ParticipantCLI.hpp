//
// Created by Nicolas Fordoxcel on 14/06/2026.
//

#ifndef PARTICIPANTSCLI_HPP
# define PARTICIPANTSCLI_HPP

//	STDLIB
# include <cctype>

//	INCLUDES
# include "./Participant.hpp"

//	TYPEDEF
typedef std::string					STRING;

//	STATIC VARIABLES

class ParticipantCLI
{
	public:

		static Participant			create();
		static void					print(const Participant& participant);
		static STRING				trim(STRING s);
};

#endif
