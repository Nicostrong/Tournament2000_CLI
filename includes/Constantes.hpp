//
// Created by Nicolas Fordoxcel on 02/07/2026.
//

#pragma once

//	Valeurs acceptables
//
//	NBPOOL							[4, 8, 16]
//	NBPLAYERSORTEAMSPERPOOL			[3, 4, 5]
//	NBPLAYERPERPOOL					[12, 16, 20, 24, 32, 40, 48, 64, 80]
//	NBPLAYERSSIMPLE					[12, 16, 20, 24, 32, 40, 48, 64, 80]
//	NBPLAYERSDOUBLE					[24, 32, 40, 48, 64, 80, 96, 128, 160]

//	Default settings
# define TOURNAMENTNAME				"Tournoi"

# define NBPLAYER					12 //ISDOUBLE ? (NBPOOL * NBPLAYERPERPOOL * 2) : (NBPOOL * NBPLAYERPERPOOL)
# define NBPOOL						4
# define NBTERRAIN					5
# define SCOREMIN					21
# define SCOREMAX					30
# define ECART						2
# define NBPLAYERPERPOOL			3
# define NBSETSIXTEENTH				1
# define NBSETHEIGTH				1
# define NBSETQUARTER				1
# define NBSETSEMI					1
# define NBSETTHIRD					1
# define NBSETFINAL					3

# define ISDOUBLE					false
# define ISMIXED					false
# define PLAYERMULTITEAM			false
# define PLAYTHIRDPLACE				false
# define ISVALIDE					false

# define GENDER						Gender::MIXED

//	Game settings
# define NBTERRAINMIN				1
# define NBTERRAINMAX				12
# define SCOREMINTOWIN				5
# define SCOREMAXTOWIN				30
# define ECARTMIN					1
# define ECARTMAX					5

//	Players setting
# define MINPLAYERS					12
# define MAXPLAYERS					96
# define NBPLAYERINMULTITEAMMAX		4

//	POOL
# define NBPOOLMIN					4
# define NBPOOLMAX					12
# define NBPLAYERPERPOOLMIN			(MINPLAYERS / NBPOOLMIN)
# define NBPLAYERPERPOOLMAX			(MAXPLAYERS / NBPOOLMAX)

//	SET
# define NBSETPOOLMIN				1
# define NBSETPOOLMAX				3
# define NBSETSIXTEENTHMIN			1
# define NBSETSIXTEENTHMAX			3
# define NBSETHEIGTHMIN				1
# define NBSETHEIGTHMAX				3
# define NBSETQUARTERMIN			1
# define NBSETQUARTERMAX			3
# define NBSETSEMIMIN				1
# define NBSETSEMIMAX				3
# define NBSETTHIRDMIN				1
# define NBSETTHIRDMAX				3
# define NBSETFINALMIN				1
# define NBSETFINALMAX				5