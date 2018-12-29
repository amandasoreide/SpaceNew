/** 
*	Class Keyboard

*	Class prefix: CK_

*	@version 2016
*/

#include <BonusManager.h>	// Header File class Bonus Manager
#include <ShootsManager.h>	/// Header File class Manager for the shoots

#include <SIKeyboard.h>
#include <SIGame.h>
#include <Player.h>
#include <UGKCamera.h>			// Header File class Camera

extern CCamera			*Camera;			///<Singleton declaration of the game camera
extern CSIGame			*Game;				///<Singleton declaration to save the general configuration of the game
extern CShootsManager	*ShootsManager,
						*ShootsManager;
extern CBonusManager	*BonusManager,
						*BonusManager;
extern RTDESK_CEngine	*RTDESK_Engine;

/*______________________________________________________________________________________

CONTROLS
========

[LEFT], [RIGHT]       -> Displacements
[UP]		           -> Shoots
[ESC]                 -> Game Quit
[+], [-]              -> Zoom in/out respectively TODO
[SPACE]               -> Change music
[OEM7]                -> Console
[0]                   -> Console text
[F1]                  -> Change game from 2D to 3D and viceversa
[F2]                  -> Sets or resets the music spectrum
[F3]                  -> Lasers
[F4]                  -> Timer for monitoring purpouses
[F5]                  -> Player passes from mortal mode to inmortal mode and viceversa
[F12]                 -> Change game to full screen
[A]                   -> Anti-aliasing on/off
[B]                   -> Change background
[C]                   -> Alternates between continuous mode and discrete mode
[H]                   -> Help
[L]                   -> Change Level of playing
[M]                   -> Music on/off (Mute)
[P]                   -> Player pause
[R]                   -> Reinitialize camera : angle, zoom, position,...
[S]                   -> Saves the current state of the game
[T]                   -> Change text to 2D
[V]                   -> Vertical Synchronization on/off
[W]                   -> Camera inc fov
[X]                   -> Camera dec fov
(MOUSE)               -> Rotation of the scene

TODO:wheel,joystick,win-tab key,weapons, block-accel
______________________________________________________________________________________*/

///Now CSIK_ActionKeys and CSIK_ActionsDelay can be implemented in the hmtl parametrizer, and the parser UGKKeyboardParser complete these lists 


///Amount of time (ms) a pressed key lasts to send a new command to the game
float	CSIK_ActionsDelay	[CSIK_MAX_KBD_ACTION] = {
	0,		//CSIK_NO_ACTION			No action has been detected. For instance, an incorrect key pressed

	500,	//CSIK_GAME_END				Finishes the game
	200,	//CSIK_GAME_2D_3D			Changes between graphical mode 2D to 3D and viceversa
	0,		//CSIK_GAME_BACKGROUND
	200,	//CSIK_GAME_LEVEL
	200,	//CSIK_GAME_ANTIALIASING	Sets or resets the antialiasing mode
	200,	//CSIK_GAME_VSYNC
	0,		//CSIK_GAME_FULL_SCREEN
	1000,	//CSIK_MODE_GAME			Alternates between continuous mode and discrete mode
	0,		//CSIK_SAVE_GAME			Saves the current state of the game 

	200,	//CSIK_PLAYER_INMORTAL		Player passes from mortal mode to inmortal mode and viceversa
	50,		//CSIK_PLAYER_MOVE_RIGHT	Player ship is move rightwards
	50,		//CSIK_PLAYER_MOVE_LEFT		Player ship is move leftwards
	200,	//CSIK_PLAYER_SHOOT			Laser fire. ms between two consecutive shoots
	100,	//CSIK_PLAYER_LASERS

	0,		//CSIK_CAMERA_INC_FOV	
	0,		//CSIK_CAMERA_DEC_FOV,
	0,		//CSIK_CAMERA_RESHAPE
	300,	//CSIK_CAMERA_ZOOM_IN
	300,	//CSIK_CAMERA_ZOOM_OUT

	200,	//CSIK_TIMER				For monitoring purpouses
	300,	//CSIK_CONSOLE
	300,	//CSIK_CONSOLE_TEXT

	200,	//CSIK_MUSIC_MUTE			Activates or mutes the music during the playing
	200,	//CSIK_NEXT_MUSIC			Changes the music for the next one
	200,	//CSIK_SPECTRUM				Sets or resets the music spectrum

	200,	//CSIK_TEXT2D

	1000,	//CSIK_PLAYER_PAUSE
	200		//CSIK_HELP
};

/**
*	KEYBOARD ACTION NME
*	Actions the keyboard can send to the game loop
*	Represent the string to identify every keyborad input
*	These lexemes may be seen at the file configuration\Keyboard.html
*/

UGKS_String CSIK_ActionLexemes[CSIK_MAX_KBD_ACTION] = {
	"NO_ACTION",			///<No action has been detected. For instance, an incorrect key pressed

	/**
	*	\defgroup GActions Game actions
	*	@{
	*	General game commands
	*/
	"GAME_END",			///<Finishes the game
	"GAME_2D_3D",		///<Changes between graphical mode 2D to 3D and viceversa
	"GAME_BACKGROUND",
	"GAME_LEVEL",
	"GAME_ANTIALIASING",	///<Sets or resets the antialiasing mode
	"GAME_VSYNC",			///<Vertical synchronization
	"GAME_FULL_SCREEN",	///<Toggles between a full screen or a window
	"MODE_GAME",
	"SAVE_GAME",			///<Saves the current state of the game 
	///@}

	/**
	*	\defgroup GPlayer Player actions
	*	@{
	*/
	"PLAYER_INMORTAL",	///<Player passes from mortal mode to inmortal mode and viceversa
	"PLAYER_MOVE_RIGHT",	///<Player ship is move rightwards
	"PLAYER_MOVE_LEFT",	///<Player ship is move leftwards
	"PLAYER_SHOOT",		///<Laser fire
	"PLAYER_LASERS",
	///@}

	/**
	*	\defgroup GCamera Camera actions
	*	@{
	*/
	"CAMERA_INC_FOV",
	"CAMERA_DEC_FOV",
	"CAMERA_RESHAPE",	///<Resets the camera at its original position
	"CAMERA_ZOOM_IN",
	"CAMERA_ZOOM_OUT",
	///@}

	"TIMER",				///<For monitoring purpouses
	"CONSOLE",
	"CONSOLE_TEXT",

	/**
	*	\defgroup GMusic Music actions
	*	@{
	*/
	"MUSIC_MUTE",		///<Activates or mutes the music during the playing
	"NEXT_MUSIC",		///<Changes the music for the next one
	"SPECTRUM",			///<Sets or resets the music spectrum
	///@}

	"TEXT2D",

	"PLAYER_PAUSE",
	"HELP"
};

//Key map associated to the game
unsigned char	CSIK_ActionKeys	[CSIK_MAX_KBD_ACTION] = {
	NULL,		//CSIK_NO_ACTION			No action has been detected. For instance, an incorrect key pressed

	VK_ESCAPE,	//CSIK_GAME_END				Finishes the game
	VK_F1,		//CSIK_GAME_2D_3D			Changes between graphical mode 2D to 3D and viceversa
	'B',		//CSIK_GAME_BACKGROUND
	'L',		//CSIK_GAME_LEVEL
	'A',		//CSIK_GAME_ANTIALIASING	Sets or resets the antialiasing mode
	'V',		//CSIK_GAME_VSYNC
	VK_F12,		//CSIK_GAME_FULL_SCREEN
	'C',		//CSIK_MODE_GAME			Alternates between continuous mode and discrete mode
	'S',		//CSIK_SAVE_GAME			Saves the current state of the game 

	VK_F5,		//CSIK_PLAYER_INMORTAL		Player passes from mortal mode to inmortal mode and viceversa
	VK_RIGHT,	//CSIK_PLAYER_MOVE_RIGHT	Player ship is move rightwards
	VK_LEFT,	//CSIK_PLAYER_MOVE_LEFT		Player ship is move leftwards
	VK_UP,		//CSIK_PLAYER_SHOOT			Laser fire
	VK_F3,		//CSIK_PLAYER_LASERS

	'W',		//CSIK_CAMERA_INC_FOV	
	'X',		//CSIK_CAMERA_DEC_FOV,
	'R',		//CSIK_CAMERA_RESHAPE
	VK_ADD,		//CSIK_CAMERA_ZOOM_IN
	VK_SUBTRACT,//CSIK_CAMERA_ZOOM_OUT

	VK_F4,		//CSIK_TIMER				For monitoring purpouses
	VK_OEM_7,	//CSIK_CONSOLE
	0,			//CSIK_CONSOLE_TEXT			Virtual event

	'M',		//CSIK_MUSIC_MUTE			Activates or mutes the music during the playing
	VK_SPACE,	//CSIK_NEXT_MUSIC			Changes the music for the next one
	VK_F2,		//CSIK_SPECTRUM				Sets or resets the music spectrum
	
	'T',		//CSIK_TEXT2D

	'P',		//CSIK_PLAYER_PAUSE
	'H'			//CSIK_HELP
};

/**
   * @fn void CSIKeyboard::Update(const CK_TOKEN Token)
   * Executes the actions associated to a token
   * @Param char Token that implements the active token
*/
void CSIKeyboard::ExecuteAction(const CK_ACTION Action, bool On)	///<Release a new key from the active list of key pressed
{
	static int shootCounter = 0;

	switch (Action)
	{
	case CSIK_PLAYER_SHOOT:
			Players[CurrentPlayer]->ManageEvent(CPL_SHOOT, On);
   		break;
	case CSIK_PLAYER_MOVE_LEFT:
			Players[CurrentPlayer]->ManageEvent(CPL_MOVE_LEFT, On);
		break;
	case CSIK_PLAYER_MOVE_RIGHT:
			Players[CurrentPlayer]->ManageEvent(CPL_MOVE_RIGHT, On);
		break;
	case CSIK_GAME_END:
		Game->AI->SetState(CSIG_FADING_OUT2END);// OutEvent(CSIG_FADING2END);		//v Controlado 4->12
		break;
	case CSIK_SAVE_GAME:
		Game->OutEvent(CSIG_SAVE);				//v Controlado 4->5
		break;
	case CSIK_PLAYER_PAUSE:
		Game->Paused = !Game->Paused;
		break;
	case CSIK_MODE_GAME:
			//Invert the simulation type on the fly
			if (Game->DiscreteSimulation)
					Game->SetSimulationType(CG_CONTINUOUS_SIMUL);
			else {
				Game->SetSimulationType(CG_DISCRETE_SIMUL);
				//Set simulation up to date for starting just in the next loop
				RTDESK_Engine->SynchSim2RealTime(0.0);
			}
			for(unsigned int i=0; i<CBN_MAX_BONUSES; i++)
				BonusManager->Bonus[i]->SetDefault();

			ShootsManager->ReleaseAllShoots();
				
		break;
	case CSIK_CAMERA_ZOOM_IN:
		if (HRTimerManager->GetTimer(CSIK_CAMERA_ZOOM_IN)->IsSounding())
		{
			HRTimerManager->GetTimer(CSIK_CAMERA_ZOOM_IN)->ResetAlarm();
			Camera->zoom =  0.02f * RenderPeriod;
			Camera->zoom_total += Camera->zoom;
		}
		break;
	case CSIK_CAMERA_ZOOM_OUT:
		if (HRTimerManager->GetTimer(CSIK_CAMERA_ZOOM_OUT)->IsSounding())
		{
			HRTimerManager->GetTimer(CSIK_CAMERA_ZOOM_OUT)->ResetAlarm();
			Camera->zoom = -0.02f * RenderPeriod;
			Camera->zoom_total += Camera->zoom;
		}
		break;
	}
}