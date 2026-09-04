//
// Created by Nicolas Fordoxcel on 21/08/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <format>

#include "../includes/utils/PrintUtils.hpp"
#include "../includes/utils/SettingsChecker.hpp"

#include "../includes/Constantes.hpp"
#include "../includes/Errors.hpp"

#include "../includes/class/Settings.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

using				cBool			=	const bool;

using				cSet            =   const Settings&;

/****************************************************************************************************/
/*	CONSTRUCTOR / DESTRUCTOR																		*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	GETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	SETTER																							*/
/****************************************************************************************************/

/****************************************************************************************************/
/*	PRIVATE METHOD																					*/
/****************************************************************************************************/

bool				SettingsChecker::addErrorIf(cBool condition, cString message, vString& errors)
{
	if (condition)
	{
		errors.push_back(message);
		return (true);
	}
	return (false);
}

void                SettingsChecker::checkLogicalTournament(cSet settings, vString& errors)
{
    cInt totalTeamsRequired = settings.getNbPools() * settings.getNbPlayerByPool();
    cInt playersPerTeam = settings.getIsDouble() ? 2 : 1;
    cInt totalPlayersRequired = totalTeamsRequired * playersPerTeam;

    if (settings.getNbPlayers() > totalPlayersRequired)
    {
        addErrorIf(true, std::format("Nombre de joueurs trop eleve ({}) pour la structure actuelle ({} poules de {} {} = {} joueurs max).",
            settings.getNbPlayers(), settings.getNbPools(), settings.getNbPlayerByPool(),
            (settings.getIsDouble() ? "equipes" : "joueurs"), totalPlayersRequired), errors);
    }
    else if (settings.getNbPlayers() < totalPlayersRequired && !settings.getAllowMultiTeamPlayers())
    {
        addErrorIf(true, std::format("Pas assez de joueurs ({}). La structure necessite exactement {} joueurs (ou activez le mode multi-equipes).",
            settings.getNbPlayers(), totalPlayersRequired), errors);
    }
    else if (settings.getNbPlayers() < totalPlayersRequired && settings.getAllowMultiTeamPlayers())
    {
        cInt missingPlayers = totalPlayersRequired - settings.getNbPlayers();
        if (missingPlayers > NBPLAYERINMULTITEAMMAX)
        {
            addErrorIf(true, std::format("Ecart de joueurs trop important ({} manquants).\n\tLe mode multi-equipes tolere au maximum {} joueurs manquants.",
                missingPlayers, NBPLAYERINMULTITEAMMAX), errors);
        }
    }
}

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

bool                SettingsChecker::isPlayerCountAllowed(cInt nbPlayers, cBool isDouble)
{
    const auto& allowedList = isDouble ? allowedNbPlayersDouble : allowedNbPlayersSimple;

    return (std::find(allowedList.begin(), allowedList.end(), nbPlayers) != allowedList.end());
}

bool                SettingsChecker::isPoolMathConsistent(cInt nbPlayers, cBool isDouble, cInt nbPools, cInt teamsPerPool)
{
    cInt playersPerTeam = isDouble ? 2 : 1;
    cInt totalPlayersRequired = nbPools * teamsPerPool * playersPerTeam;
    return (nbPlayers == totalPlayersRequired);
}

bool                SettingsChecker::isValid(Settings& settings, vString& errors)
{
    errors.clear();

    addErrorIf(settings.getName().empty(), E_NAME, errors);
    addErrorIf(!isPlayerCountAllowed(settings.getNbPlayers(), settings.getIsDouble()), E_NBPLAYER, errors);
    addErrorIf(!isInList(settings.getNbPlayerByPool(), allowedNbPlayersOrTeamsPerPools), E_NBPLAYERPERPOOL, errors);
    addErrorIf(!isInList(settings.getNbPools(), allowedNbPools), E_NBPOOL, errors);
    addErrorIf(settings.getNbBadmintonCourt() <= 0, E_NBTERRAIN, errors);
    addErrorIf(settings.getScoreMin() <= 0, E_SCOREMIN, errors);
    addErrorIf(settings.getScoreMax() <= settings.getScoreMin(), E_SCOREMAX, errors);
    addErrorIf(settings.getDiffPointsToWin() <= 0, E_DIFFSCORE, errors);
    addErrorIf(!isInList(settings.getNbSetPlayedPools(), allowedNbSetToPlay), E_NBSETPOOL, errors);
    addErrorIf(!isInList(settings.getNbSetPlayedSixteenth(), allowedNbSetToPlay), E_NBSETSIXTEENTH, errors);
    addErrorIf(!isInList(settings.getNbSetPlayedHeigth(), allowedNbSetToPlay), E_NBSETHEIGHT, errors);
    addErrorIf(!isInList(settings.getNbSetPlayedQuarters(), allowedNbSetToPlay), E_NBSETQUARTER, errors);
    addErrorIf(!isInList(settings.getNbSetPlayedSemis(), allowedNbSetToPlay), E_NBSETSEMI, errors);
    addErrorIf(!isInList(settings.getNbSetPlayedFinal(), allowedNbSetToPlay), E_NBSETFINAL, errors);

    if (settings.getIsThirdPlaceMatch())
        addErrorIf(!isInList(settings.getNbSetPlayedThirdPlace(), allowedNbSetToPlay), E_NBSETTHIRD, errors);

    checkLogicalTournament(settings, errors);

    cBool valid = errors.empty();
    settings.setIsValid(valid);
    return (valid);
}

bool                SettingsChecker::validateAll(const Settings& settings)
{
    vString dummyErrors;
    Settings tempSettings = settings;

    return (isValid(tempSettings, dummyErrors));
}

bool                SettingsChecker::canAccommodate(cSet settings, cInt actualPlayers)
{
    return ((settings.getNbPlayers() - actualPlayers) <= NBPLAYERINMULTITEAMMAX);
}
