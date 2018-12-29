/**
*	Definition of the class game
*	General class to support all the attribute general to all the potential games produced in the future.
*	Prefix: CG_

*	@author Ramon Molla
*	@version 2012-10
*/

#ifndef CG_CLASS_GAME
#define CG_CLASS_GAME

#include <UGKcharacter.h>
#include <RTDeskEngine.h>

typedef enum {
	CG_AI_DIR,			//Artifical Intelligence directory
	CG_CONFIG_DIR,		//General configuration directory
	CG_LANGUAGE_DIR,	//See Application.LanguageFolder 
	CG_LEVEL_DIR,		//Levels directory
	CG_USER_LEVEL_DIR,	//User levels directory
	CG_MESH_DIR,		//Meshes directory
	CG_MUSIC_DIR,		//Music directory
	CG_SOUNDS_DIR,		//Sounds (special effects) directory
	CG_TEXTURE_DIR,		//Textures directory
	CG_DEVICES_DIR,		//Devices directory
	CG_MAX_DIR
} CG_DIRECTORIES;

//Simulation Type

typedef enum {
	CG_CONTINUOUS_SIMUL,
	CG_DISCRETE_SIMUL,
	CG_MAX_SIMULATION_MODE	//For management purpouses only. Always at the end of the enumeration
} CG_SIMULATION_MODE;

using namespace UGK;

/**
*	\class CGame
* Game singleton that manages middleware specific for this specific game.
* It behaves also as a character because it has its own behaivor.
*/
class CGame: public CCharacter 
{
protected:
	//Game general attributes
	UGKS_String Version;		///< Game VERSION
	UGKS_String Title;			///< Game TITLE

public:
	///Level control
	int	Level;			///<Current LEVEL of the game

	//These two attributes have to be managed by the score manager and initialized by the initialization parser
	//More scores and names have to be stored also when finished a game session
	unsigned int	Score;			///<Current score
	unsigned int	HiScore;		///<Current highest score

	///Game directories where to store game general information: meshes, sounds, ai,...
	UGKS_String Directories	[CG_MAX_DIR];

	///Devices
	std::vector<string>	DevicesIO;

//methods
	//Constructor
	CGame (){Reset();};
	~CGame ();

	void Reset();

	virtual void Save (UGKS_String  fileName); /// Saves the game to a file - Rodrigo Aldecoa

	//Setters y Getters
	/**
	@fn SetVersion
	Assigns a game version to the Game in order to let the game to open a correct/understandable version of the initialization or game files
	@param UGKS_String V Version to be set to the Game
	*/
	inline void SetVersion(UGKS_String V) {Version = V;}

	/**
	@fn SetTitle
	Assigns a game title
	@param UGKS_String T Title to be set
	*/
	inline void SetTitle(UGKS_String T) {Title = T;}

	inline UGKS_String GetVersion()	{return Version;};
	inline UGKS_String GetTitle()	{return Title;};
};

#endif