//
// Created by Nicolas Fordoxcel on 09/07/2026.
//

//	STDLIB
#include <algorithm>
#include <cctype>

//	INCLUDES
#include "../includes/utils/FormatUtils.hpp"

//	TYPEDEF
typedef std::string					STRING;
typedef const std::string&			C_STRING;

//	STATIC VARIABLES

/****************/
/*	CANONICAL	*/
/****************/

/************/
/*	GETTER	*/
/************/

/************/
/*	SETTER	*/
/************/

/********************/
/*	PRIVATE METHOD	*/
/********************/

/********************/
/*	PUBLIC METHOD	*/
/********************/

STRING								FormatUtils::trim(C_STRING s)
{
    auto							start = s.begin();

    while (start != s.end() && std::isspace(static_cast<unsigned char>(*start)))
        start++;

    auto							end = s.end();

    do
    {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(static_cast<unsigned char>(*end)));

    return (std::string(start, end + 1));
}

STRING								FormatUtils::capitalize(STRING s)
{
    if (s.empty())
        return (s);
    
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    s[0] = std::toupper(s[0]);
    
	return (s);
}

STRING								FormatUtils::toUpper(STRING s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);

    return (s);
}