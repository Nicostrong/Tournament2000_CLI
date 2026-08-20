//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

/****************************************************************************************************/
/*	INCLUDES																						*/
/****************************************************************************************************/

#include <algorithm>
#include <cctype>

#include "../includes/utils/FormatUtils.hpp"

/****************************************************************************************************/
/*	TYPEDEF																							*/
/****************************************************************************************************/

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

/****************************************************************************************************/
/*	PUBLIC METHOD																					*/
/****************************************************************************************************/

/**
 * trim retire touts les espaces en debut et fin de string.
 * @param s string a modifier
 */
void	    		FormatUtils::trim(String& s)
{
    const size_t start = s.find_first_not_of(" \t\n\r\f\v");

    if (start == String::npos)
    {
        s.clear();
        return;
    }

    const size_t end = s.find_last_not_of(" \t\n\r\f\v");

    s.erase(end + 1);
    s.erase(0, start);
}

/**
 * capitalize met la premiere lettre d une string en majuscule et passe
 * touts les autres caracteres en minuscule.
 * @param s string a modifier
 */
void				FormatUtils::capitalize(String& s)
{
    if (s.empty())
        return;
    
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    s[0] = std::toupper(s[0]);
}

/**
 * toUpper transforme touts les caracteres d une string en majuscule.
 * avant la modification, la string est trimee.
 * @param s string a modifier
 */
void				FormatUtils::toUpper(String& s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
}

/**
 * toLower transforme la string en mettant touts les caracteres en minuscule.
 * @param s string a modifier
 */
void				FormatUtils::toLower(String& s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

/**
 * trimAndCapitalize trim une string puis met la premiere lettre en majuscule et passe
 * toutes les autres en minuscule.
 * @param s string a modifier
 */
void				FormatUtils::trimAndCapitalize(String& s)
{
    trim(s);

    if (s.empty())
        return;

    capitalize(s);
}

/**
 * trimAndPutToUpper trim une string puis transforme touts les caracteres en majuscule.
 * @param s string a modifier
 */
void				FormatUtils::trimAndPutToUpper(String& s)
{
    trim(s);

    if (s.empty())
        return;

    toUpper(s);
}

/**
 * trimAndPutToLower trim une string puis transforme touts les caracteres en minuscule
 * @param s string a modifier
 */
void				FormatUtils::trimAndPutToLower(String& s)
{
    trim(s);

    if (s.empty())
        return;

    toLower(s);
}