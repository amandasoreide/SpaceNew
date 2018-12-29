// SI_2013.cpp: define el punto de entrada de la aplicación.
//

#pragma once

#include <UGKLanguages_ISO639.h> //Header File For ISO639 & Languages
#include <UGKGLWindow.h>
#include <SIGame.h>
#include <Application.h>
#include <SIKeyboard.h>
#include <Player.h>
#include <Ship.h>

#include <Resource.h> //Header File For ISO639 & Languages
#include <TextResources.h>	// Header File For the internationalization of the text resources 
#include <GlobalDefs.h>
#include <GlobalTiming.h>

#include <UGKCharactersPool.h>
#include <SICharactersFactory.h>
#include <QuadtreeRoot.h>

#include <Scene.h>
#include <iostream>
#include <stdlib.h>
#include <atlstr.h>
#include <CommCtrl.h>
#include <fstream>

#define WM_TOGGLEFULLSCREEN (WM_USER+1)		// Application Message For Toggling Fullscreen
#define MAX_LOADSTRING 100

extern CSIGame				*Game;
extern CApplication			Application;
extern CSIKeyboard			SIKeyboard;		///<Singleton to manage the application keyboard
extern CGraphicCard			GraphicsCard;	///<Singleton to manage the application Graphics Card
extern CScene				Scene;			//Scene
extern CSICharactersFactory	*CharacterFactory;
extern CCharactersPool		*CharacterPool;   ///< Pool of CCharacters

extern RTDESK_CEngine		*RTDESK_Engine;
extern CMsgFactory			MsgFactory;

extern BOOL	MainProgramLooping;

// THREADS
unsigned long	SIDLG_h_thread;	// Handle Of The Loading Thread

// Variables globales:
UGKS_String aux;
stringstream	ss;
string			strg;
HANDLE			hFind = INVALID_HANDLE_VALUE;

/*---------------------------------------------
ADD: Charco Aguirre, Jorge
Variable local of Language for Combobox
-----------------------------------------------*/
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

map<int, std::string> language_combo;
HINSTANCE hInstance_tmp;
HWND hWndComboBox;			//Combobox that contain the language of game
string codeHTML;			//Contain code the chosen language
//----------------------------------------------

ofstream fout;

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


DWORD  SetUpHCI () 
{
	HMENU hMenubar	= CreateMenu();
	HMENU hGame		= CreateMenu();
	HMENU hLoad		= CreateMenu();
	HMENU hConfiguration = CreateMenu();
	HMENU hHelp		= CreateMenu();

	CString		AuxCString;
	UGKS_String AuxString;

	//Main level load
	AuxCString = UGKS_string2CString(SIGLB_GetText(TR_MENU_WINDOW_GAME));
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hGame, AuxCString);					//Game
	AuxCString = UGKS_string2CString(SIGLB_GetText(TR_MENU_WINDOW_LEVEL));
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hLoad, AuxCString);					//Level
	//Add for Windows Configuration	
	AuxCString = UGKS_string2CString(SIGLB_GetText(TR_MENU_WINDOW_CONFIGURATION));
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hConfiguration, AuxCString);			//Configuration
	//*****************************
	AuxCString = UGKS_string2CString(SIGLB_GetText(TR_MENU_WINDOW_HELP));
	AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hHelp, AuxCString);					//Help

	//Second level options
	//Game options
	AuxCString = UGKS_string2CString(SIGLB_GetText(TR_MENU_WINDOW_START_GAME));
	AppendMenu(hGame, MF_STRING, ID_Start, AuxCString);								//"Start Game"
	AuxCString = UGKS_string2CString(SIGLB_GetText(TR_MENU_WINDOW_EXIT_GAME));
	AppendMenu(hGame, MF_STRING, ID_Exit, AuxCString);
	//Language options
	AuxCString = UGKS_string2CString(SIGLB_GetText(TR_TAG_LANGUAGE));
	AppendMenu(hConfiguration, MF_STRING, ID_Language, AuxCString);					//Language

	//Reading possible levels in Levels\UserLevels
	WIN32_FIND_DATA ffd;

	UGKS_GenerateFullPathName(AuxString, CSIG_PredefinedFiles[CSIG_ULEVEL_FILE], "*");
	AuxString += UGK_HTML_EXT;
	AuxCString = UGKS_string2CString(AuxString);
	hFind = FindFirstFile(AuxCString, &ffd);

	if (INVALID_HANDLE_VALUE != hFind)
	{
		do
		{
			aux = UGKS_CString2string(ffd.cFileName);
			Game->HtmlFiles.push_back(aux);
		} while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);
	}

	if(Game->HtmlFiles.size() > MAX_LEVELS_HTML)
		for(int i=0; i!=MAX_LEVELS_HTML; i++){
			AuxCString = UGKS_string2CString(Game->HtmlFiles[i]);
			AppendMenu(hLoad, MF_BYPOSITION, i, AuxCString);
		}
	else
		for(int i=0; i!=Game->HtmlFiles.size(); i++){
			AuxCString = UGKS_string2CString(Game->HtmlFiles[i]);
			AppendMenu(hLoad, MF_BYPOSITION, i, AuxCString);
		}
    
	AuxCString = UGKS_string2CString(SIGLB_GetText(TR_MENU_WINDOW_ABOUT));
	AppendMenu(hHelp, MF_STRING, IDM_ABOUT, AuxCString);
	
	SetMenu(Application.Window.hWnd, hMenubar);

	/*------------------------------------------------------------
	Create ComboBox of Language and Buttom
	ADD: Charco Aguirre, Jorge
	------------------------------------------------------------*/
	int xpos = 10;            // Horizontal position of the window.
	int ypos = 10;            // Vertical position of the window.
	int nwidth = 400;         // Width of the window
	int nheight = 300;        // Height of the window
	
	AuxCString = UGKS_string2CString(SIGLB_GetText(TR_TAG_ACCEPT));

	hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		xpos, ypos, nwidth, nheight, Application.Window.hWnd, HMENU(ID_LISTLANGUAGE), HINST_THISCOMPONENT,NULL);
	
	HWND hWndButtom = CreateWindow(WC_BUTTON, AuxCString,
		WS_VISIBLE | WS_CHILD, 230, ypos, 125, 25, Application.Window.hWnd, HMENU(ID_ACCEPTLANGUAGE), HINST_THISCOMPONENT, NULL);
		
	/**
	  *Initialize File of Language
	  *Call function of UGK for Load ISO639 and read directory of language for fill combobox
	  *
	*/
	ISO639 Iso_Language;
	if (Iso_Language.loadISO())
	{
		Iso_Language.readDirectory();
		language_combo = Iso_Language.getLANGUAGECOMBO();		
		unsigned int cont = 0;
		while (cont<language_combo.size())
		{
			auto search = language_combo.find(cont);						//Find ISO of file Language inside global variable
			if (search != language_combo.end())
			{
				/*---------------------------------------------------------
				Convert language of game to TCHAR for combobox
				--------------------------------------------------------*/
				string item = search->second;
				int c = MultiByteToWideChar(0, 0, item.c_str(), -1, NULL, 0);
				TCHAR *tmp = new TCHAR[c];
				MultiByteToWideChar(0, 0, item.c_str(), -1, tmp, c);
				//----------------------------------------------------------
				SendMessage(hWndComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)tmp);
				cont = cont + 1;
			}
		}
	}

	Application.Window.Show();
	
	return NULL;
}

/** Function Update_Language
* Update the default.html file using a temporary file , it is done because when you start
* the program the file is locked default.html
* Then copy the temporary default.html file, the temporary file is deleted so that the copy 
* is not performed each time the program starts.
*/
/*-------------------------
ADD: Charco Aguirre, Jorge
Update file of language  
-------------------------*/
void Update_Language()
{
	WIN32_FIND_DATA findFileData;
	HANDLE          hFind;
	char dir[MAX_PATH];
	char path[MAX_PATH] = "\\languages\\default_tmp.html";
	DWORD cchCurDir = MAX_PATH;

	GetCurrentDirectoryA(cchCurDir, dir);
	strcat_s(dir, _countof(dir), path);				// Join path + folder that contain file of configuration
	/*---------------------------------------------------------
	Convert PATH of directory to LPCWSTR for read de folder
	--------------------------------------------------------*/
	int c = MultiByteToWideChar(0, 0, dir, -1, NULL, 0);
	wchar_t *tmp = new wchar_t[c];
	MultiByteToWideChar(0, 0, dir, -1, tmp, c);
	LPCWSTR dir1 = tmp;
	/*---------------------------------------
	Get file of the folder language
	-----------------------------------------*/
	hFind = FindFirstFile(dir1, &findFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		UGKS_String SourceLang, DefaultLang;
		UGKS_GenerateFullPathName(SourceLang, Application.LanguageFolder, CA_DEFAULT_TMP_LANGUAGE_FILE);
		UGKS_GenerateFullPathName(DefaultLang, Application.LanguageFolder, CA_DEFAULT_LANGUAGE_FILE);
		DefaultLang += ".html";
		CopyFileA(SourceLang.c_str(), DefaultLang.c_str(), FALSE);
		DeleteFileA(SourceLang.c_str());
	}
}

void LoadRTDESK() {

	RTDESK_TIME Aux;

	if (NULL == RTDESK_Engine)
		 RTDESK_Engine = new RTDESK_CEngine();

	if (RTDESK_Engine->StartUp())
		 printf("RTDESK_StartUpEngine initialization OK\n");
	else printf("RTDESK_StartUpEngine initialization fail\n");

	//Para permitir que los mensajes con tiempo 0 se ejecuten directamente sin encolarse pongo 1LL.
	Aux = RTDESK_SLACK_TIME;
	RTDESK_Engine->SetStoreSlackTime(Aux);	//StoreSlackTime es longlong -> Aux representa los ticks de CPU de Slack Time

	//Configuration of the Pool Manager
	RTDESK_Engine->SetMsgFactory(&MsgFactory);
	RTDESK_Engine->SetMaxMsgTypes(UMSG_MSG_MAX_TYPE);
}

void Initialize()
{	
#ifdef CHAR_USE_QUADTREE
	QuadtreeRoot	*QTRoot;

	//Collision structures initialization
	QTRoot = new QuadtreeRoot(CHARS_MAX_CHARTYPE);
	QTRoot->setQuadtree(SIGLBD_PG_BOTTOM, SIGLBD_PG_LEFT, SIGLBD_PG_WIDTH, SIGLBD_PG_HEIGHT, 6);
	
	CharacterFactory->SetQTRoot(QTRoot);
#endif

	Update_Language();

	//Initialize the factory of characters using the default characters
	CharacterFactory->InitDict(CHARS_Tags, CHARS_MAX_CHARTYPE);

	//Initialize Characters Pool
	CharacterPool = new CCharactersPool (CharacterFactory);
	CharacterPool->setSubTypes(CHARS_SHIP,  CS_MAXTYPE);
	CharacterPool->setSubTypes(CHARS_SHOOT, CSH_MAX_SHOOTS_TYPES);

	//Initialize the discrete simulator engine in order to create the dispatchers and so on
	LoadRTDESK();

	Game = (CSIGame*)CharacterFactory->createNewCharacter(CHARS_GAME, UGKOBJM_NO_SUBTYPE);
}

void DeInitialize()
{
	RTDESK_Engine->Reset();
	TimerManager.DestroyAllTimers();
}

#ifdef	UGKOS_WINDOWS
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
#elif	defined(UGKOS_LINUX)
#elif	defined(UGKOS_OSX)
#elif	defined(UGKOS_ANDROID)
#endif
	
	UGKS_String		temp;
	bool			firstTime=true;
		
	hInstance_tmp = hInstance;
	Application.LanguageFolder = "pepe";
	CharacterFactory = new CSICharactersFactory();
	Application.LanguageFolder = "pepe";
	Initialize();
	
	// Fill Out Application and main window data
	Application.Name = CSIG_TITTLE;		// Application Class Name
	Application.hInstance = hInstance;	// Saves the Application Instance instance id into the corresponding attribute in the application singleton

	//Update Application Window attributes
	Application.Window.setWindowMetadata(Application.Name, Game->initialWidthW, Game->initialHeightW, 16, true, true);
	Application.Window.SetName(Application.Name);
	Application.Window.SetTitle(Application.Name);
	Application.Window.hInstance=	Application.hInstance;
	Application.Window.SetId(IDI_SI_2013, IDI_SMALL, IDS_APP_TITLE);
	Application.Window.SetWndProc(WndProc);
	Application.Window.SetCmdShow(nCmdShow);
		
	//Initializing game singleton general parameters
	if (!Game->Initialize())
	{// Main Game initialization failure
		Application.Terminate();
		Application.CurrentState = CA_APP_END;
	}

	//Use language values loaded in Game->Initialize. So, please, execute after game initialization
	SetUpHCI();

	// Load Device for DialogBox
	Game->LoadDevice();

	//**************************************************************//

	// Bucle principal de mensajes:	
	while (GetMessage(&msg, NULL, 0, 0) && MainProgramLooping)									// Loop Until WM_QUIT Is Received
	{	
		//AFD that represents the behavior of the main Application
		switch (Application.CurrentState)
		{
		case CA_APP_INIT:
				temp = Game->GetTitle();
				temp += " [Start]";
				OutputDebugString(UGKS_string2LPCWSTR(temp));

				// Get the resolution for "Quick FullScreen" mode
				if (Application.Window.quickFullScreen == TRUE && Application.Window.FullScreen == FALSE)// If Quick Fullscreen, we catch the screen resolution
				{
					Application.Window.Width	= GraphicsCard.DeskTop.Width;
					Application.Window.Height	= GraphicsCard.DeskTop.Height;
				}

				Application.CurrentState = CA_APP_CREATE;
			break;
		case CA_APP_CREATE:
				if (Application.Window.IsCreated())		// Was Window Creation Successful?	
				{	// At This Point We Should Have A Window That Is Setup To Render OpenGL
					// Start The Message Pump. Initialization was a success
					MainProgramLooping = TRUE;
					Game->AI->SetState(CSIG_INITIAL);
					Application.CurrentState = CA_APP_START;
					TimerManager.GetTimer(SIGLBT_STEP_APP_TIMING)->InitCounting();		//Start step application timer
					TimerManager.GetTimer(SIGLBT_IDLE_TIMING)->InitCounting();		//Start step application timer
				}
				else									// If Window Creation Failed
				{
					// Error Creating Window
					MessageBox(HWND_DESKTOP, UGKS_string2LPCWSTR(SIGLB_GetText(TR_BASECODE_ERROR_CREATING_OPENGL)), LPCWSTR("Error"), MB_OK | MB_ICONEXCLAMATION);
					Application.CurrentState = CA_APP_STOP;
				}
			break;
		case CA_APP_START:
				while (MainProgramLooping && !Game->StopAfterOneLevel)								// While The Message Pump Is Active
				{
					MainProgramLooping = Application.Update();

					if(Game->Paused){		
						Application.CurrentState = CA_APP_PAUSE;
						break;
					}
					#ifdef DEF_EXP_CSV_FILE
					switch (Game->WayToEnd)
					{
					case CSIG_END_BY_TIME:
						if(DEF_MAX_TICKS < TimerManager.GetTimer(SIGLBT_APP_TIMING)->TicksUntilNow()){
							Game->Snapshot(".\\Resultados\\Snapshot.bmp");
							Game->SetEndingCriteria(true, false);
							MainProgramLooping = FALSE;
						}
						break;
					case CSIG_END_BY_FRAME:
						if (TimerManager.AllnFrames >= DEF_END_IN_FRAME){
							Game->Snapshot(".\\Resultados\\Snapshot.bmp");
							Game->SetEndingCriteria(false, true);
							MainProgramLooping = FALSE;
						}
						break;
					}
					#endif

					Game->RunMainLoop();	
				}
				if(!Game->Paused)
					Application.CurrentState = CA_APP_END;
			break;
		
		case CA_APP_END:
			#ifdef DEF_EXP_CSV_FILE
			if (Game->EndByTime || Game->EndByFrame)
			{
				TimerManager.EndAccCounting(SIGLBT_IDLE_TIMING);

				double msTotApl = TimerManager.GetTimer(SIGLBT_APP_TIMING)->GetLastPeriodms();
				double msTotIdl = TimerManager.GetTimer(SIGLBT_IDLE_TIMING)->GetLastPeriodms();

				double medFps = TimerManager.fpsAcum / TimerManager.fpsContTotal;

				ss.str(UGKS_EMPTY_STRING);
				ss << "Total: " << msTotApl << "Toal Discrt: " << TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING)->GetLastPeriodms();
				ss << " Update: " << TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->GetLastPeriodms();
				ss << " Render: " << TimerManager.GetTimer(SIGLBT_RENDER_TIMING)->GetLastPeriodms() << " Idle: " << msTotIdl;
				ss << " %Idle: " << (msTotIdl * 100) / msTotApl << " Avrg FPS: " << medFps << ";" << TimerManager.AllnFrames << UGKS_NEW_LINE_STRING;

				if(Game->DiscreteSimulation) fout.open(".\\Resultados\\ResumenD.csv", std::ios_base::app);
				else fout.open(".\\Resultados\\ResumenC.csv", std::ios_base::app);
				fout << ss.str() << std::endl;
				fout.flush();
				fout.close();

				TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING)->ResetAccumulate();
				TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->ResetAccumulate();
				TimerManager.GetTimer(SIGLBT_RENDER_TIMING)->ResetAccumulate();
				TimerManager.GetTimer(SIGLBT_IDLE_TIMING)->ResetAccumulate();		
				
				#ifdef DEF_LEC_PARTIAL
				FILE *fsalida;
				fsalida = fopen("DLecRnd.csv", "a+");
				for(int lec=0; lec<indTotRnd; lec++){
					fprintf(fsalida, "%d;%.6f\n", lec, muestrasTotRnd[lec]);
				}
				fclose(fsalida);

				fsalida = fopen("DLecUpd.csv", "a+");
				for(int lec=0; lec<indTotUpd; lec++){
					fprintf(fsalida, "%d;%.6f\n", lec, muestrasTotUpd[lec]);
				}
				fclose(fsalida);

				fsalida = fopen("DLecIdl.csv", "a+");
				for(int lec=0; lec<indTotIdl; lec++){
					fprintf(fsalida, "%d;%.6f\n", lec, muestrasTotIdl[lec]);
				}
				fclose(fsalida);

				fsalida = fopen("DLecRtd.csv", "a+");
				for(int lec=0; lec<indTotRtd; lec++){
					fprintf(fsalida, "%d;%.6f\n", lec, muestrasTotRtd[lec]);
				}
				fclose(fsalida);
				#endif
			}
			#endif

			// Application Is Finished
			Game->ResetObjects();
			Game->Deinitialize();											// User Defined DeInitialization

			DeInitialize();

			#ifdef CHW_USE_KINECT
			KinectManager.cleanupExit();									// Clean kinect stuff
			#elif  defined(CHW_USE_WIIMOTE)
			//Disconnect the wiimotes
			wiiuse_cleanup(wiimotes, CWM_MAX_WIIMOTES);
			#endif
			Application.Window.Destroy();									///Destroy The Active Window
			UnregisterClass(UGKS_string2LPCWSTR(Application.Name), Application.hInstance);		// UnRegister Window Class
			Application.CurrentState = CA_APP_STOP;
			break;
		case CA_APP_PAUSE:
			SIKeyboard.Update();
			if(!Game->Paused)
				Application.CurrentState = CA_APP_START;
			break;
		case CA_APP_STOP:
		default:
			if (!Application.Window.TranslateAcc(&msg))
				if (msg.message != WM_QUIT)
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			break;
		}
	}
	return (int) msg.wParam;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: procesar mensajes de la ventana principal.
//
//  WM_COMMAND	: procesar el menú de aplicación
//  WM_PAINT	: pintar la ventana principal
//  WM_DESTROY	: enviar un mensaje de finalización y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	CString AuxString, AuxString1;

	switch (message)
	{
	case WM_COMMAND:

		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Menu selections analize:
		//All options between 0 <-> 50 are reserved to load individual levels.
		if(wmId>=MIN_LEVELS_HTML && wmId < MAX_LEVELS_HTML)
		{
			Game->NextHtmlInd2Load = wmId;
			Game->OnlyOneLevel = true;
			Application.CurrentState = CA_APP_INIT;
		}
		else{
			UGKS_String SourceLang, DefaultLang;
			
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(Application.hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case ID_Exit:
				//DestroyWindow(hWnd);
				MainProgramLooping = false;
				break;
			case ID_Start:
				Game->OnlyOneLevel = false;
				Application.CurrentState = CA_APP_INIT;
				break;
			case ID_Language:
				AuxString = UGKS_string2CString(SIGLB_GetText(TR_TAG_MSG_LANGUAGE));
				AuxString1 = UGKS_string2CString(SIGLB_GetText(TR_TAG_MESSAGES));
				MessageBox(HWND_DESKTOP, AuxString, AuxString1, MB_OK | MB_ICONINFORMATION);
				break;

			case ID_ACCEPTLANGUAGE:
				/*-----------------------------------------------------------
				ADD: Charco Aguirre, Jorge (Create file temporal of language)
				-----------------------------------------------------------*/
				UGKS_GenerateFullPathName(SourceLang, Application.LanguageFolder, codeHTML);
				UGKS_GenerateFullPathName(DefaultLang, Application.LanguageFolder, CA_DEFAULT_TMP_LANGUAGE_FILE);
				CopyFileA(SourceLang.c_str(), DefaultLang.c_str(), FALSE);			
				AuxString = UGKS_string2CString(SIGLB_GetText(TR_TAG_CHANGE_LANGUAGE));
				AuxString1 = UGKS_string2CString(SIGLB_GetText(TR_TAG_MESSAGES));
				MessageBox(HWND_DESKTOP, AuxString, AuxString1, MB_OK | MB_ICONINFORMATION);
				break;

			case ID_LISTLANGUAGE:
				/*------------------------------------------------
				ADD: Charco Aguirre, Jorge (Select Change ComboBox)
				------------------------------------------------*/
				if (HIWORD(wParam) == CBN_SELCHANGE)
				{
					int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
					TCHAR  ListItem[256];
					(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);
					std::wstring tmp(ListItem);
					std::string arr_s(tmp.begin(), tmp.end());
					string language_choose = arr_s;
					codeHTML = arr_s.substr(1, 3) + ".html";					
				}
				//------------------------------------
				break;

			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: agregar código de dibujo aquí...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	////////////////////////////////////////////////////////////////////////////////
	case WM_CREATE:													// Window Creation
		{
		CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);		// Store Window Structure Pointer
		// Get The OS Window Context
		LPVOID OSWindow = creation->lpCreateParams;
		SetWindowLong(hWnd, GWL_USERDATA, (LONG)(OSWindow));
		}
		return 0;													// Return
	case WM_CLOSE:													// Closing The Window
		Application.Terminate();									// Terminate The Application
		Application.CurrentState = CA_APP_END;
		return 0;													// Return

	case WM_ENTERSIZEMOVE:
		if (CSIG_INITIAL == Game->AI->GetState())											// pendant le chargement on baisse la priorité
			SetThreadPriority((void*)SIDLG_h_thread, THREAD_PRIORITY_LOWEST);
		return 0;
	case WM_EXITSIZEMOVE:		
			SetThreadPriority((void*)SIDLG_h_thread, THREAD_PRIORITY_NORMAL );
		return 0;

	case WM_ERASEBKGND:
		return true;

	case WM_SIZE:													// Size Action Has Taken Place
		switch (wParam)												// Evaluate Size Action
		{
		case SIZE_MINIMIZED:										// Was Window Minimized?
			//BOX("minimized")
			Application.Window.Visible = FALSE;						// Set Visible To False
			return 0;												// Return

		case SIZE_MAXIMIZED:										// Was Window Maximized?
			Application.Window.Visible = TRUE;						// Set Visible To True
			Application.Window.Width = LOWORD(lParam);
			Application.Window.Height = HIWORD(lParam);
			Game->ReshapeGL();										// Reshape Window - LoWord=Width, HiWord=Height
			return 0;												// Return

		case SIZE_RESTORED:											// Was Window Restored?
			Application.Window.Visible = TRUE;						// Set Visible To True
			Application.Window.Width = LOWORD(lParam);
			Application.Window.Height = HIWORD(lParam);
			Game->ReshapeGL();							// Reshape Window - LoWord=Width, HiWord=Height
			return 0;									// Return
		}
		break;											// Break
	case WM_KEYDOWN:
		if (!SIKeyboard.ActionIsActive((char)wParam))
		{
			// Update Keyboard Buffers For Keys Pressed
			SIKeyboard.SetActiveKey(wParam);				// Set The Selected Key (wParam) To True			
			///Now the program calls to method Update in general, not only updates a key if not all
			SIKeyboard.Update();
		}
		break;											// Break
	case WM_KEYUP:										// Update K	eyboard Buffers For Keys Released
		SIKeyboard.ResetActiveKey(wParam);
		SIKeyboard.Update();
		break;											// Break

	case WM_LBUTTONDOWN:
			Scene.Moving = true;						// We are moving the scene
			Scene.Start.v[XDIM] = LOWORD(lParam);		// mouse x
			Scene.Start.v[YDIM] = HIWORD(lParam);		// mouse y
		break;

	case WM_LBUTTONUP:
			Scene.Moving		= false;						// Stop moving the scene
		break;

	case WM_MOUSEMOVE:
		/// Scene rotation
		if (Scene.Moving)
		{
			Scene.Angle.v[XDIM] += (HIWORD(lParam) - Scene.Start.v[XDIM]) / 2;			// souris axe x (/2 pour diminuer la vitesse)
			Scene.Angle.v[YDIM] += (LOWORD(lParam) - Scene.Start.v[YDIM]) / 2;			// souris axe y (/2 pour diminuer la vitesse)
		
			Scene.Start.v[XDIM] = HIWORD(lParam);					// mouse x
			Scene.Start.v[YDIM] = LOWORD(lParam);					// mouse y
		}
		break;

	case WM_TOGGLEFULLSCREEN:									// Toggle FullScreen Mode On/Off
		Application.Window.FullScreen = !Application.Window.FullScreen;
		PostMessage(hWnd, WM_QUIT, 0, 0);
		break;													// Break
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Controlador de mensajes del cuadro Acerca de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
