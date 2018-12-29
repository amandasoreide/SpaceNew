/**	
*	@file GlobalDefs.h

Global definitions and types

*	Prefix: SIGLBD_

*	@author Ramon Molla
*	@version 2015-11
*/


#ifndef SIGLB_GLOBALD_H
#define SIGLB_GLOBALD_H

#ifdef  _MSC_VER
	#pragma warning (disable:4305 4244 4005)	// Disable warnings on precision truncations/conversions
												// 4305 : truncation
												// 4244 : conversion, possible loss of data
#endif

	//Avoids warnings of deprecated functions
	//#define _CRT_SECURE_NO_WARNINGS

	#define MAXSSxLEV 10	//Maximun amount of Supply Ships per level
	#define SIGLB_MAX_MUSIC_NAME_LENGTH	1024

	// DEBUG TOOLS
	#define DEBUG_MORTAL					// DEBUG
	#define DEBUG_MUSIC						// DEBUG
	#define DEBUG_BUNKERS					// DEBUG
	#define DEBUG_FPS						// DEBUG

	//PLAYING GROUND BOUNDARIES. For frustrum culling,  player, navy,... movements
	//It is suppossed that the center of the universe is at (0.0, 0.0) just in the middle of the screen
	#define SIGLBD_PG_BOTTOM	-10.0f
	#define SIGLBD_PG_CEILING	 13.0f
	#define SIGLBD_PG_RIGHT		 08.0f
	#define SIGLBD_PG_LEFT		-07.5f
	#define SIGLBD_PG_WIDTH		 15.5f
	#define SIGLBD_PG_HEIGHT	 23.0f

	//Limits for Ships Movement with Bounding Mode if it is Active.
	#define SIGLBD_PG_SHIPS_BOTTOM	-3.5f
	#define SIGLBD_PG_SHIPS_CEILING	 8.0f

	#define SIGLBD_DEFAULT_Z_POSITION	0.05f

	#define SIGLBD_MAX_UPDATETIME	40.0		//ms. None of the objects should update over this period of time.
	#define SIGLBD_MAX_RENDERTIME	16.66667	//ms. Belongs to 60fps

	//Macro that eases the writing for a string in the language selected by the user
#define SIGLB_GetText(Name) Application.Language->GetText(Name)

///Only define this macro if debug log messages are activated
//#define SIGLB_DEBUG
#ifdef	SIGLB_DEBUG
#define SIGLB_AddLog(s) Log.Add(s)
#else
#define SIGLB_AddLog(s)
#endif

typedef enum
{
	CSIG_NO_END,
	CSIG_END_BY_TIME,
	CSIG_END_BY_FRAME,
	CSIG_END_MAX_TYPES
} CSIG_GAME_ENDING_TYPES;

typedef enum
{
	SIGLB_NO_ERROR,
	SIGLB_WINDOW_CREATION_ERROR
} SIGLB_END_ERRORS;

typedef struct {
	int r;
	int g;
	int b;
	int a;
} C3DS_Color4f;

#define SIGLB_PLAYER_LIGHT GL_LIGHT0
#define SIGLB_SHIP_LIGHT   GL_LIGHT1
#define SIGLB_SHIP2_LIGHT  GL_LIGHT2

#endif
