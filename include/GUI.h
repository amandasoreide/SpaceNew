/**
*	@file GUI.h
*	Definition of the class Interface

*	Prefix: CGUI_

*	@author Ramon Molla
*	@version 2011-11
*/

#include <Boometer.h>

#ifndef CGUI_INTERFACE //Se borrar la macros interface
#define CGUI_INTERFACE //Se define la macros interface

#define CGUI_MAX_COMMAND			 2
#define CGUI_MAX_CONSOLE_LENGTH		40
#define CGUI_MAX_CONSOLE_LINES		50
#define CGUI_MAX_CONSOLE_COLS		50

#define CGUI_MAX_CONSOLE_MSG_COLS	20
#define CGUI_MAX_NUMBERS			10
#define CGUI_MAX_SCORE_LENGTH		 6

typedef enum {
	CGUI_INVALID_COMMAND,
	CGUI_TYPE_VBLE_NAME,
	CIP_AVERAGE_ERROR,
	CGUI_MAX_MSGS
} CGUI_LogMsg;

// CONSOLE Class
class CConsole
{
public:

	bool	Active,			///< Is Active
			ComeIn,			///< On screen
			Moving,			///< Is moving
			Start,			///< Is starting
			Passing2D23D;	///< Flag that signals the state of moving from 2D rendering mode to 3D one

	char	Text[CGUI_MAX_CONSOLE_LINES][CGUI_MAX_CONSOLE_COLS];	// CONSOLE TEXT
	int		Line;			// CONSOLE LINE INDEX FOR CONSOLE_TEXT[][]

	char	Command			[CGUI_MAX_COMMAND][CGUI_MAX_CONSOLE_LENGTH];	// CONSOLE 1ERE COMMANDE
	char	Output			[CGUI_MAX_COMMAND][CGUI_MAX_CONSOLE_LENGTH];	// CONSOLE 1ERE SORTIE
	float	ViewportPush;	// CONSOLE VIEWPORT MINI-SCREEN

	CConsole();
};

/**   
   * @class cInterface
 
	* This class defines the Graphical User Interface for the game states, where is the constructor
    * Define the GUI to display the screens of the game such as pause, the player's score, lives, buttons
	
	@author Ramon Molla
	@version 2018-02
*/

class CGUI:public CGUIGadget
{
public:
	//Attributes
	float		Fading,				///<Interface Fading percentage
				LoadingBMPPosition;	///<Unary percentage of the position of the element to put on the screen. May affect any dimension
	CConsole	Console;
	bool		Passing2D23D,		///< Flag that alerts from moving from global 2D rendering scene mode to a 3D one
				Blending,
				Alpha;
	char		Number[CGUI_MAX_NUMBERS] = UGKS_EMPTY_STRING;
	int			NumberLength,
				NumberTexture;

	cHRTimer	RenderTimer;

	//Constructor
	CGUI() {Init();}

	//Methods
	void Init ();

	///Shows the user interface on the screen
	void Render(void);

	void DisplayRing2D23D				();					//mostrar conversion de 2D a #D
	void DisplayScores					();					//Mostrar score de jugadors
	void DisplayLives					();					//Show the amount of lives the player has: donw on the left and on the up-right corner
	void DisplayConsole					();					//mostrar la pantalla de consola
	void display_console_mini_screen	();					//Mostrar pequeña pantalla de consola
	void DisplayTextFrames				();					//mostrar marcos de texto 
	void DisplayHealth					();					//Mostrar salud de nave
	void DisplaySpectrum				();					//mostrar spectrum
	void DisplayLoadingBMP				();
	void DisplayStats					();
	void DisplaySplashScreen			(); //pantalla de inicio
	void DisplayPressKey				(); //mostrar botones
	void DisplayPaused					(); ///<Show a "pause" string on the screen while in pause mode

	void FadingInWon		(void);		///< Animation while Fading in the Score
	void FadingOutLoading	(void);		///< Animation while Fading out the SI logo

	void ChangeRenderMode	(CHAR_RENDER_MODE Mode);
	void SetTexturesManager	(CCharacter* TexturesManager);

	//TODO: Add a scene graph for gadgets in GUI.
private:
	CBoometer *boometer;
};
int FadingInAnimation();
//DWORD WINAPI FadingInAnimation(LPVOID lpParam);		///< Animation while Fading in the SI logo

#endif
