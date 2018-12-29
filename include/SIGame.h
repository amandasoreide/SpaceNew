/**
*	Definition of the Class Space Invaders Game
*	General class to support all the attribute specific for the Sàce Invaders Game
*	Prefix: CSIG_

*	@author Ramon Molla
*	@version 2012-10
*/

#ifndef CSIG_CLASS_GAME
#define CSIG_CLASS_GAME

#include <Game.h>
#include <SIMessage.h>
#include <UGKSoundSystem.h>
#include <GameCharacters.h>
#include <GlobalDefs.h>
#include <UGKAudioHardware.h>

#define CSIG_INIT_INITIAL	  0	//Number in the array or elements that holds the configuration by default
								//defined in the "initialization.html" file during the initialization phase
#define CSIG_INIT_CURRENT	  0	//Position of the current element used as default Player, super ship,...

#define CSIG_NAVY_INITIAL_Y_POSITION	 2.0f

#define CSIG_INTERMISSION_TEXT_BLINKS 3

#define CSIG_VERSION "1.0.0"			//Version number for the current implementation of the Space Invaders video game
#define CSIG_TITTLE  "Space Invaders"	//Tittle name for the current implementation of the Space Invaders video game

#define CSIG_SAVED_GAME  "SavedGame"

typedef enum {
	CSIG_INITIALIZATION_FILE,	//Predefined initialization game file
	CSIG_KEYBOARD_FILE,			//Keyboard map description file
	CSIG_DEVICES_FILE,			//Devices file
	CSIG_ULEVEL_FILE,			//User Level files predefined name or root name

	CSIG_GAME_AI_FILE,			//Artifical Intelligence file description for the singleton Game
	CSIG_CIRCLESHIP_AI_FILE,	//Artifical Intelligence file description for the circle ships
	CSIG_SUPPLYSHIP_AI_FILE,	//Artifical Intelligence file description for the supply ships
	CSIG_SHIP_AI_FILE,			//Artifical Intelligence file description for the common ships

	CSIG_MAX_PREDEF_FILES
} CSIG_PREDEF_FILES;

/** \typedef CN_EXTRA_TIMERS
*	Types of different local timing managed by the NAVY
*/
typedef enum {
	CSIG_UPD_PERIOD,	//Update period
	CSIG_RND_PERIOD,	//Rendering period
	CSIG_MAX_TIMERS
} CSIG_EXTRA_TIMERS;

typedef enum {
	SIAI_GAME_DEFAULT,
	SIAI_SHIP_DEFAULT,
	SIAI_SUPPLYSHIP_DEFAULT,
	SIAI_CIRCLESHIP_DEFAULT,
	SIAI_BONUS_DEFAULT,
	SIAI_MAX_AI
} SIAI_AI_TYPE;

//Game States
typedef enum	
{
	CSIG_INITIAL,			///<The game is in the initialization phase
	CSIG_FADING_IN,			///<The game is appearing from black while is reading the level information and updating internal data structures
	CSIG_FADING_OUT2PLAY,	///<Fading out from loading to play
	CSIG_PLAYING,			///<The Player is playing now the game
	CSIG_SAVING,			///<The game is saved for posterior playing	
	CSIG_PASSING_2_3D,
	CSIG_INTERMISSION,		///<When the general game is moving because a bonus has been catched, the player has lost a life, the game,...
	CSIG_LOST_LIFE,			///<The Player has lost one life
	CSIG_WON,				///<The Player has won the game
	CSIG_LOST_GAME,			///<The Player has lost the game	
	CSIG_FADING_OUT2WON,	///<The game is Fading to black while releasing all data internal structures
	CSIG_FADING_OUT2END,	///<The game is Fading to black while releasing all data internal structures
	CSIG_FADING_OUT2LOST,	///<The game is Fading to black while releasing all data internal structures
	CSIG_PAUSED,			///<The game stays on hold
	CSIG_WAIT_KEY,			///<The game waits till the player presses any key.
	CSIG_END,				///<Releasing the videogame until new game session
	CSIG_MAXSTATE			///<Amount of different states in the game
} CSIG_GAME_STATE;

///Different transitions for the FSMs that control the Game behaviour
typedef enum {
	CSIG_DEFAULT=0,			///For management purpouses only
	CSIG_STARTING,			///The game is just born and initialized. Now it is time for interfacing with the player
	CSIG_LOADING,			///Loading level to play
	CSIG_FADING_OUT,		///Finishing the loading stage before atarting the match
	CSIG_LOADED,			///The current level has just being loaded. Go to play
	CSIG_LIFE_LOST,			///One life has been lost. A general camera moving has to be performed
	CSIG_PLAY,				///Go on playing
	CSIG_SAVE,				///Save the game
	CSIG_SAVED,				///The game has been saved and goes back to playing
	CSIG_CHANGING_2_3D,		///When a CBN_BONUS_3D Bonus is touched, a change to 3D view mode is performed
	CSIG_3D_CHANGED,		///The transition to 3D view mode is finished. Now, turn back to CSIG_PLAYING state
	CD_INTERMISS_FINISHED,	///The intermission has finished
	CSIG_FADING2WON,		///The current level is going to being won
	CSIG_FADING2END,		///The game is going to end
	CSIG_FADING2LOST,		///The current level is going to being lost
	CSIG_FADED,				///The current level has just been won or lost or the game has just ended
	CSIG_NEXT_LEVEL,		///The level has been won and now a new level has to be loaded
	CSIG_UPDATE,			///Event given to any state to evolve/update itself
	CSIG_PAUSE,				///The player asks the game to pause
	CSIG_CONTINUE,			///The player asks the game to continue
	CGS_KEYPRESSED,			///The player pressed a key to start a new game or level.
	CSIG_2END,				//v
	CSIG_MAXTRANSITION		///For management purpouses only
} CSIG_GAME_TRANSITIONS;	///Generic character possible states that can be any character by default

extern UGKS_String CSIG_PredefinedFiles[CSIG_MAX_PREDEF_FILES];

/**
*	\class CSIGame
* Game singleton that manages middleware specific for this specific game.
* It behaves also as a character because it has its own behaviour.
*/
class CSIGame: public CGame
{
	void InitGraphics	();
	void LoadGame		(UGKS_String  filename, UGKS_String  folder);

public:

		//Music Management
	UGKS_String 		CurrentMusic,
						music_affichee,
						stream_name,
						stream_name_user;

	CHAR_RENDER_MODE	NextRenderMode;

	//Presentation attributes
	bool			Text_2D;			// 2D TEXT AFFICHE ?
	bool			Spectrum;			// SPECTRUM (BEAT DETECTION)?
	bool			Antialiasing;		///< Using antialiasing for rendering the GUI gadgets

	bool			BouncingMode;
	bool			Paused;
	bool			LevelReaded;				// Level reading on
	bool			ReadLevel;					// Activate/Deactivate, reading on from parser
	bool			OnlyOneLevel;				// Activated, Game is executed only with the current level
	CSIG_GAME_ENDING_TYPES WayToEnd;

	int				NextHtmlInd2Load;			// With OnlyOneLevel active, indicate the html file index to load next level.
	UGKS_String 	LevelFileName;
	vector <UGKS_String>	HtmlFiles;
	bool			StopAfterOneLevel;
	unsigned int	SSxLEV;						// Amount of Supply Ships per level...

	GLUquadricObj*	quadratic;		// Storage For Our Quadratic Objects
	UGKAudioHardware soundcard;
	CSoundSystem	SoundSystem;


	//Control 
	bool			EndByTime,
					EndByFrame,
					DiscreteSimulation;
	int				initialWidthW;
	int				initialHeightW;


	//Legacy attributes to refactor
	#ifdef UGKSND_FMOD375
	#elif CSND_FMODEx
	#elif CSND_OAL
	#elif UGKSND_SDL
		Mix_Music *music;
	#endif

	bool			AboutDlgBoxMODWorking;	// Music For About-DialogBox
	int				MusicConsoleOutput;
	bool			Sound2D3DPassed;		// Sound passing from 2D to 3D

	///Auxiliar attributes for transition management
	float	intermede_angle_total;	// INTERMEDE ANGLE TOTAL pour l'animation de l'Game->intermede
	bool	active_splash;			// SPLASH SCREEN activé après l'intermède
	
	RTDESK_TIME		NextSimulation;

	RTDESK_CMsg *msg,								//RTDESK Message Time
				*msgResetTest;						//RTDESK Reset Test Message
	cMsgGame	*GenericMsg[UMSG_GENERIC_MSG_MAX_TYPE];
	
	float	DefaultUpdPeriod[CHARS_MAX_CHARTYPE],
			DefaultRndPeriod[CHARS_MAX_CHARTYPE];

//methods
	//Constructor
	 CSIGame ();
	~CSIGame ();

	void Init(void);
	bool Initialize (void);			// Any GL Init Code & User Initialization Goes Here
	void AssignSounds2Objects(void);	///< Assign global sounds to specific objects as they will need them during the playing
	void Deinitialize (void);
	void ResetObjects(void);

	//Loading methods for different aspects of the game
	void LoadInitialization(void);	///< Method for load the initialization file
	void LoadDevice(void);			///< Method for load input devices 
	void LoadKeyboard(void);		///< Method for load keyboard html

	void LoadLevel(int lev);
	void LoadLevelFile(int HtmlFileInd);

	void Loading();

	void LaunchInitialEvents();
	void AssignRTDESK();
	void AssignRTDESK2Navy();

	void InitObjectReferences();	//Initialize all the references to external objects for all the potential objects or objects managers in the game
	void InitTimers(void);			//Initialize all the timers for all the potential objects or objects managers in the game
	void RunMainLoop();
	void PromoteLevel();	
	void Display();
	void Snapshot(UGKS_String filename);

	void Save(UGKS_String fileName); /// Saves the game to a file - Rodrigo Aldecoa
	void EndAccCounting(unsigned int indTmr);

	void SetEndingCriteria(bool ByTime, bool ByFrame);
	void SetSimulationType(CG_SIMULATION_MODE SimulationType);

	///Shows the whole game on the screen
	///These are not virtual since this class does not come from class Character
	void Render(void);
	void ReshapeGL(void);

	///Change the way the whole game is going to be rendered on the screen
	void ChangeRenderMode(CHAR_RENDER_MODE);

	//AI Methods
	bool OutEvent(CSIG_GAME_TRANSITIONS EventName);

	///Artificial Intelligence character initialization
	void AssignAI2defaultObjects();
	void AI_Init		(SIAI_AI_TYPE Type);	
	///Moving the Supply Ship
	void AI_Move		(float ms10);				

	void StartCameraMov();
	/**
	* @fn bool CSIGame::PlayerWon()
	* Check for the conditions let the player to win the match
	*/
	bool PlayerWon();

	void SetDefaultDirectories();

	void Update ();			//Update the world. Similar to render() but updating
	void DiscreteUpdate ();	//Discrete Simulation Update
	
	void ReceiveMessage(RTDESK_CMsg *pMsg);
};
#endif