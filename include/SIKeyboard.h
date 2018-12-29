/**	Definition of the class keyboard for Space Invaders

*	@file SIKeyboard.h
*	Prefix: CSIK_

*	@author Ramon Molla
*	@version 2013-10
*/

#ifndef CSIK_KEYBOARD 
#define CSIK_KEYBOARD 

#include <UGKKeyboard.h>

using namespace UGK;

/**
*	\typedef CSIK_KEYBOARD_ACTION
*	Actions the keyboard can send to the game loop
*	Tokens assigned to every keyborad input 
*/

typedef enum {
	CSIK_NO_ACTION = 0,			///<No action has been detected. For instance, an incorrect key pressed

	/**
	*	\defgroup GActions Game actions
	*	@{
	*	General game commands
	*/
	CSIK_GAME_END,			///<Finishes the game
	CSIK_GAME_2D_3D,		///<Changes between graphical mode 2D to 3D and viceversa
	CSIG_GAME_BACKGROUND,
	CSIK_GAME_LEVEL,
	CSIK_GAME_ANTIALIASING,	///<Sets or resets the antialiasing mode
	CSIK_GAME_VSYNC,		///<Vertical synchronization
	CSIK_GAME_FULL_SCREEN,	///<Toggles between a full screen or a window
	CSIK_MODE_GAME,
	CSIK_SAVE_GAME,			///<Saves the current state of the game 
	///@}

	/**
	*	\defgroup GPlayer Player actions
	*	@{
	*/
	CSIK_PLAYER_INMORTAL,	///<Player passes from mortal mode to inmortal mode and viceversa
	CSIK_PLAYER_MOVE_RIGHT,	///<Player ship is move rightwards
	CSIK_PLAYER_MOVE_LEFT,	///<Player ship is move leftwards
	CSIK_PLAYER_SHOOT,		///<Laser fire
	CSIK_PLAYER_LASERS,
	///@}

	/**
	*	\defgroup GCamera Camera actions
	*	@{
	*/
	CSIK_CAMERA_INC_FOV,
	CSIK_CAMERA_DEC_FOV,
	CSIK_CAMERA_RESHAPE,	///<Resets the camera at its original position
	CSIK_CAMERA_ZOOM_IN,
	CSIK_CAMERA_ZOOM_OUT,
	///@}

	CSIK_TIMER,				///<For monitoring purpouses
	CSIK_CONSOLE,
	CSIK_CONSOLE_TEXT,

	/**
	*	\defgroup GMusic Music actions
	*	@{
	*/
	CSIK_MUSIC_MUTE,		///<Activates or mutes the music during the playing
	CSIK_NEXT_MUSIC,		///<Changes the music for the next one
	CSIK_SPECTRUM,			///<Sets or resets the music spectrum
	///@}

	CSIK_TEXT2D,

	CSIK_PLAYER_PAUSE,
	CSIK_HELP,

	CSIK_MAX_KBD_ACTION		///<For accounting purpouses only
} CSIK_KEYBOARD_ACTION;

/**   
	* Space Invaders Keyboard class
*/
class CSIKeyboard: public CKeyboard
{
private:
	
public:
	float	RenderPeriod;
	void	ExecuteAction(const CK_ACTION Token, bool On);	//Virtual method to be implemented in this program
};

extern float			CSIK_ActionsDelay	[CSIK_MAX_KBD_ACTION];
extern unsigned char	CSIK_ActionKeys		[CSIK_MAX_KBD_ACTION];

#endif
