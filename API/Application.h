/**	Definition of the class Application

*	Prefix: CA_

*	@author Ramon Molla
*	@version 2011-09
*/

#ifndef CA_APPLICATION
#define CA_APPLICATION

#include <Network.h>
#include <UGKLanguage.h>

#define CA_DEFAULT_LANGUAGE_FILE "default"
#define CA_DEFAULT_TMP_LANGUAGE_FILE "defaultTemp.html"

#define CA_OPENGL
//#define CA_DIRECTX

#ifdef	UGKOS_WINDOWS
	#ifdef CA_OPENGL
		#include <UGKGLWindow.h>
	#elif	defined(CA_DIRECTX)
		#include <UGKDXWindow.h>
	#endif
#elif	defined(UGKOS_LINUX)
	#include <UGKGLWindow.h>
#elif	defined(UGKOS_OSX)
	#include <UGKGLWindow.h>
#elif	defined(UGKOS_ANDROID)
	#include <UGKGLWindow.h>
#endif

typedef enum
{
	CA_APP_STOP,
	CA_APP_INIT,
	CA_APP_CREATE,
	CA_APP_START,
	CA_APP_PAUSE,
	CA_APP_END,
	CA_APP_MAX_STATES
} CA_MAIN_APP_STATES;

/**
*	CApplication

*	Manages all the information relative to the general game application.
*	It is implemented as a singleton. This is not the game singleton
*/
class CApplication
{
public:
	//Atributes
	UGKS_String		LanguageFolder;
	UGKS_String		Name;		///<Application Name

	UGK::CLanguage *Language;	///< Language that holds all the text to be showed to the user, logs,...
	CGLWindow		Window;		///<Singleton to manage the application Window

	#ifdef	UGKOS_WINDOWS
	//Windows specific Attributes
	HINSTANCE		hInstance;	///<Application Instance
	#elif	defined(UGKOS_LINUX)
	#elif	defined(UGKOS_OSX)
	#elif	defined(UGKOS_ANDROID)
	#endif

	NET_BITRATE_TYPE	Bitrate;	///<Network BITRATE 
	bool				Stream;		///<MUSIC STREAMING

	//AI FSM state support
	CA_MAIN_APP_STATES CurrentState;

	//Methods
	void Init (void);

	CApplication(){Init(); Language = new UGK::CLanguage();}	///<Constructor of the class
	~CApplication() { if (Language) delete Language; }

	void Terminate();

	int GetOptions(bool Stream);
	bool Update();

	bool LoadCurrentLanguage ();
};

#endif
