/**	Declaration of the class Application

	Prefix: CA_

	@author Ramon Molla
	@version 2014-07
*/

#include <Application.h>
#include <TextResources.h>
#include <SIGame.h>

/**   
   @fn void CApplication::Init (void)
   Initializes the application
 */

void CApplication::Init (void)
{
	#ifdef	UGKOS_WINDOWS
		Window.DeleteName();
		Name = UGKS_EMPTY_STRING;
		Window.hInstance	= hInstance;

		Bitrate				= NET_BR_DSL;	// MUSIC BITRATE 
	#elif	UGKOS_LINUX
	#elif	UGKOS_OSX
	#elif	UGKOS_ANDROID
	#endif

	CurrentState = CA_APP_STOP;
}

/**   
   @fn int CApplication::Update ()
   Performs the main loop application
*/
bool CApplication::Update ()
{
	#ifdef	UGKOS_WINDOWS
	
	MSG msg;

	//Check For Window Messages
	while (PeekMessage(&msg, Window.hWnd, 0, 0, PM_REMOVE))
		// Check For WM_QUIT Message
		if (msg.message == WM_QUIT)						// Is The Message A WM_QUIT Message?
			return false;	// Otherwise (If Message Is WM_QUIT)
					// Terminate The Message Pump
		else DispatchMessage(&msg);						// If Not, Dispatch The Message
														// to WindowProc()
	// when there Are No Messages
	if (FALSE == Window.Visible)					// If Window Is Not Visible
		WaitMessage();								// Application Is Minimized Wait For A Message (paused)
	return true;
	#elif	UGKOS_LINUX
	#elif	UGKOS_OSX
	#elif	UGKOS_ANDROID
	#endif
}

/**   
   @fn void CApplication::Terminate()
   TERMINATE APPLICATION
*/
void CApplication::Terminate()					// Terminate The Application
{
	#ifdef	UGKOS_WINDOWS
		PostMessage(Window.hWnd, WM_QUIT, 0, 0);	// Send A WM_QUIT Message
	#elif	UGKOS_LINUX
	#elif	UGKOS_OSX
	#elif	UGKOS_ANDROID
	#endif
}

/**   
   @fn void CApplication::Init (void)
   Assigns a game version to the Game in order to let the game to open a correct/understandable version of the initialization or game files
   @param char V Version to be set to the Game
*/
int CApplication::GetOptions(bool Stream)
{
	
#ifdef	UGKOS_WINDOWS
	HKEY key;
	DWORD disposition;
	DWORD type, size;

	// check key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, UGKS_string2LPCWSTR(Name), 0, KEY_READ, &key) == ERROR_SUCCESS)
	{	
		
		size = sizeof(bool);
		RegQueryValueEx(key, LPCTSTR("Stream"),	NULL, &type, (LPBYTE)&Stream,	&size);

		// read int values
		type = REG_DWORD;
		size = sizeof(DWORD);
		RegQueryValueEx(key, LPCTSTR("Bitrate"), NULL, &type, (LPBYTE)&Bitrate, &size);

		return 1;
	}
	else
	{
		// create key
		RegCreateKeyEx(HKEY_LOCAL_MACHINE, UGKS_string2LPCWSTR(Name), 0, UGKS_EMPTY_LPWSTR, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disposition);
		return 0;
	}
	RegCloseKey(key);

	#elif	UGKOS_LINUX
	#elif	UGKOS_OSX
	#elif	UGKOS_ANDROID
	#endif
}

/**   
	* @fn bool CApplication::LoadCurrentLanguage ()
	* Construct the default file languages (log and lang)
	* Assign files and load language
 */
bool CApplication::LoadCurrentLanguage ()
{
	Language->SetSize(TR_MAX_TR);
	return Language->Load(CA_DEFAULT_LANGUAGE_FILE, LanguageFolder, CSIG_TITTLE, CSIG_VERSION);
};
