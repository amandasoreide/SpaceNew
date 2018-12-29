/** 
*	Global variables and singletons are declared here

*	Prefix: No prefix by default

*	@author Ramon Molla
*	@version 2012-08
*/

#define WIN32_LEAN_AND_MEAN				// Compile faster (excludes automatic processing of some rarely-used headers)
#include <Global.h>						// Header File For General Parameters of the Game

#include <Navy.h>			// Header File class Navy-> Ships and supply ships that fight in the enemy navy
#include <Player.h>
#include <UGKCameraManager.h>

#ifndef CDS_FULLSCREEN					// CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4				// Compilers. By Defining It This Way,
#endif									// We Can Avoid Errors

using namespace UGK;

CSITimeAcc TimerManager;

/**
*	\defgroup Managers Managers Singletons
*	@{
*/
CAIManager					AIManager;			///<For managing the Artificial Intelligence of any kind of character
CTextureAnimationManager	AnimationsManager;
CBonusManager*				BonusManager;
CShootsManager*				ShootsManager;
CFontsManager				FontsManager;
CMeshesManager				MeshesManager;
CScoresManager				ScoresManager;
CSoundsManager				SoundsManager;
CTexturesManager			TexturesManager;
CCameraManager				CameraManager;
SISceneGraph				SceneGraph;

///Amount of players simultaneously available when playing
vector<CPlayer*>	Players;
int					CurrentPlayer = CPL_NO_PLAYER;	///<Initially, the player by default is the one before the first one

#ifdef CHW_USE_WIIMOTE
CWiimoteManager	WiimoteManager;
#endif

/**
*	\defgroup GSingletons General game singletons 
*
*	@{
*/
CApplication	Application;
CNavy			*Navy;			///<Singleton to save the general configuration of the enemy Navy
CGUI			GUI;			///<Manages all the windows, msgs,... for the player
CCamera			*Camera;		///<Singleton declaration of the game camera
CScene			Scene;			///<Singleton to manage the Scene
CBackground		Background;
CSICharactersFactory	*CharacterFactory;			///<Singleton declaration of the character factory that creates as many given characters as demanded

//RT-DESK
RTDESK_CEngine *RTDESK_Engine = NULL;
CMsgFactory		MsgFactory;

/**
*	\defgroup GSingletons HW game singletons 
*
*	@{
*/
CSIKeyboard		SIKeyboard;		///<Singleton to manage the application keyboard
CGraphicCard	GraphicsCard;	///<Singleton to manage the application Graphics Card

BOOL	MainProgramLooping;