//
// Created by Nicolas Fordoxcel on 13/06/2026.
//

#ifndef PARTICIPANT_HPP
# define PARTICIPANT_HPP

//	STDLIB
# include <string>
# include <vector>

//	INCLUDES
# include "./Pool.hpp"
# include "./Team.hpp"
# include "./Match.hpp"
# include "./Phase.hpp"
# include "./Settings.hpp"
# include "./Tournament.hpp"
# include "./Participant.hpp"

//	TYPEDEF
using				string		=	std::string;
using				cString		=	const std::string&;

using				cPool		=	const Pool&;
using				cTeam		=	const Team&;
using				cMatch		=	const Match&;
using				cPhase		=	const Phase&;
using				cSet		=	const Settings&;
using				cPart		=	const Participant&;

using				vpPool		=	std::vector<Pool*>;
using				vpTeam		=	std::vector<Team*>;
using				vpMatch		=	std::vector<Match*>;
using				vpPhase		=	std::vector<Phase*>;
using				vpPart		=	std::vector<Participant*>;

using				cvpPool		=	const std::vector<Pool*>&;
using				cvpTeam		=	const std::vector<Team*>&;
using				cvpMatch	=	const std::vector<Match*>&;
using				cvpPhase	=	const std::vector<Phase*>&;
using				cvpPart		=	const std::vector<Participant*>&;		cString		=	const std::string&;

//	STATIC VARIABLES

/**
 * la classe Participant represente un joueur humain.
 */
class Participant
{
	public:

		enum Gender { MALE, FEMALE, MIXED };

	private:

		static int					_idCounter;
		size_t						_id;
		string						_pseudo;
		string						_lastName;
		string						_firstName;
		Gender						_gender;
		bool						_isEliminated;
		bool						_isMultiTeamPlayer;

	public:

		//	CANONICAL
		Participant() = delete;
		Participant(cString pseudo, cString lastName, cString firstName, Gender gender);
		Participant(const Participant& p) = delete;
		Participant&				operator=(const Participant& p) = delete;
		~Participant() = default;

		//	GETTER
		[[nodiscard]]
		size_t						getId() const;
		[[nodiscard]]
		cString					getPseudo() const;
		[[nodiscard]]
		cString					getFirstName() const;
		[[nodiscard]]
		cString					getLastName() const;
		[[nodiscard]]
		string						getGenderStr() const;
		[[nodiscard]]
		Gender						getGenderInt() const;
		[[nodiscard]]
		bool						getIsEliminated() const;
		[[nodiscard]]
		bool						getIsMultiTeamPlayer() const;

		//	SETTER
		void						setPseudo(cString value);
		void						setFirstName(cString value);
		void						setLastName(cString value);
		void						setGender(Gender value);
		void						setIsEliminated(bool value);
		void						setIsMultiTeamPlayer(bool value);

		//	METHOD

};

#endif