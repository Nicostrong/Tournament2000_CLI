//
// Created by Nicolas Fordoxcel on 15/07/2026.
//

#ifndef COLOR_HPP
# define COLOR_HPP

namespace	Color
{
	// ==================================================
	// REINITIALISATION
	// ==================================================
	constexpr const char* RESET			=	"\033[0m";

	// ==================================================
	// EFFETS ET STYLES DE TEXTE
	// ==================================================
	constexpr const char* BOLD			=	"\033[1m"; // Gras / Intense
	constexpr const char* DIM			=	"\033[2m"; // Sombre / Attenue
	constexpr const char* ITALIC		=	"\033[3m"; // Italique
	constexpr const char* UNDERLINE		=	"\033[4m"; // Souligne
	constexpr const char* BLINK			=	"\033[5m"; // Clignotement lent
	constexpr const char* FAST_BLINK	=	"\033[6m"; // Clignotement rapide
	constexpr const char* REVERSE		=	"\033[7m"; // Inverse couleur texte et fond
	constexpr const char* HIDDEN		=	"\033[8m"; // Masqué (texte invisible)
	constexpr const char* STRIKE		=	"\033[9m"; // Texte barre

	// ==================================================
	// COULEURS DU TEXTE
	// ==================================================
	constexpr const char* BLACK			=	"\033[30m";
	constexpr const char* RED			=	"\033[31m";
	constexpr const char* GREEN			=	"\033[32m";
	constexpr const char* YELLOW		=	"\033[33m";
	constexpr const char* BLUE			=	"\033[34m";
	constexpr const char* MAGENTA		=	"\033[35m";
	constexpr const char* CYAN			=	"\033[36m";
	constexpr const char* WHITE			=	"\033[37m";

	// ==================================================
	// COULEURS DU TEXTE EN GRAS / INTENSES
	// ==================================================
	constexpr const char* BRED			=	"\033[1;31m";
	constexpr const char* BGREEN		=	"\033[1;32m";
	constexpr const char* BYELLOW		=	"\033[1;33m";
	constexpr const char* BBLUE			=	"\033[1;34m";
	constexpr const char* BMAGENTA		=	"\033[1;35m";
	constexpr const char* BCYAN			=	"\033[1;36m";
	constexpr const char* BWHITE		=	"\033[1;37m";

	// ==================================================
	// COULEURS D'ARRIERE-PLAN
	// ==================================================
	constexpr const char* BG_BLACK		=	"\033[40m";
	constexpr const char* BG_RED		=	"\033[41m";
	constexpr const char* BG_GREEN		=	"\033[42m";
	constexpr const char* BG_YELLOW		=	"\033[43m";
	constexpr const char* BG_BLUE		=	"\033[44m";
	constexpr const char* BG_MAGENTA	=	"\033[45m";
	constexpr const char* BG_CYAN		=	"\033[46m";
	constexpr const char* BG_WHITE		=	"\033[47m";
}

#endif
