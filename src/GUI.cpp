/*	Definition of the class Interface

Prefix: CI_

@author Ramon Molla
@version 2011-11
In this file all the functions of interface (display windows) player
*/

#include <GUI.h>
#include <GUIGadget.h>
#include <SIGame.h>
#include <FontsManager.h>
#include <UGKMeshManager.h>
#include <BonusManager.h>	// Header File class Bonus Manager

#include <Player.h>
#include <UGKLog.h>
#include <GameSounds.h>
#include <Application.h>
#include <UGKCamera.h>			// Header File class Camera
#include <Scene.h>

#include <SITexturesResources.h>
#include <UGKLog.h>

#include <GlobalDefs.h>
#include <Background.h>
#include <glext.h>
#include <TextoGL.h>

#define	CI_FadingSpeed			0.0006
#define CI_INITIAL_PANEL_PUSH	5.3
#define CI_SPEED_PANEL_PUSH		0.05

//Amount of pixels from the right margin to wirte real time information
#define TEXT_X_POS (Game->initialWidthW-150)
#define TEXT_Y_POS 50
//Amount of pixels to increment for every line written
#define TEXT_Y_INC 25

#define CI_BELOW_LIFES_X SIGLBD_PG_LEFT
#define CI_BELOW_LIFES_Y -7.5f

#define CI_CHAR_WIDTH	.8f
#define CI_CHAR_HEIGHT  .5f
#define CI_CHAR_ADVANCE .9f	//The amount of X movement to write another different character on the screen

#define CI_SMALLCHAR_WIDTH	 .5f
#define CI_SMALLCHAR_HEIGHT  .3f
#define CI_SMALLCHAR_ADVANCE .4f	//The amount of X movement to write another different character on the screen

#define CI_SCORE_WIDTH	2.0
#define CI_SCORE_HEIGHT	0.6

extern CBonusManager	*BonusManager;
extern CFontsManager	FontsManager;
extern CMeshesManager	MeshesManager;
extern CSoundsManager	SoundsManager;

extern CApplication		Application;
extern CCamera			*Camera;		///<Singleton declaration of the game camera
extern CLog				Log;			///<For logging performance, tracing,...
extern CScene			Scene;			///<Singleton to manage the Scene

extern CBackground		Background;
extern CGUI				GUI;

UGKS_String CGUI_ParserMsgs[CGUI_MAX_MSGS] =
{
	"Invalid command",
	"Type a variable name",
	"Average error 3D.01 (3D transition)"
};

extern CSIGame	*Game; // The game singleton

					   //2D Texts Colors
GLfloat red[3] = { 1.0f, 0.0f, 0.0f };
GLfloat gren[3] = { 0.0f, 1.0f, 0.0f };
GLfloat yellow[3] = { 1.0f, 1.0f, 0.0f };
GLfloat blue[3] = { 0.0f, 0.0f, 1.0f };

CConsole::CConsole()
{
	Line = 0;	// CONSOLE INDEX DE CONSOLE_TEXT[][]

	strcpy(Command[0], UGKS_EMPTY_STRING);
	strcpy(Command[1], UGKS_EMPTY_STRING);

	strcpy(Output[0], UGKS_EMPTY_STRING);
	strcpy(Output[1], UGKS_EMPTY_STRING);

	ViewportPush = -999.0;	// CONSOLE VIEWPORT MINI-SCREEN

	Active = false;	// CONSOLE inactive
	ComeIn = true;		// CONSOLE on screen
	Moving = false;	// CONSOLE not moving
	Start = false;	// CONSOLE not starting
}

/**
* Initializer for the Interfaces
*/
void CGUI::Init()
{
	Fading = 0.0001;
	UGK::Vector origin;

	LoadingBMPPosition = UGKCL_LOADINGBMP_FINAL_POSITION;

	origin.v[XDIM] = SIGLBD_PG_RIGHT + 1.0;
	origin.v[YDIM] = CPL_PLAYER_Y_2D - 0.5;
	Directory.resize(CHARS_MAX_REFERENCES);
	boometer = new CBoometer(origin, 0.5, 3, CTM_HEALTH_BAR, (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF]);

	ResetTransformations();
	RenderMode = CHAR_2D;	//GUIs are always rendered in 2D
}

/**
* Change the Textures Manager
*/
void CGUI::SetTexturesManager(CCharacter* TexturesManager)
{
	Directory[CHARS_TEXTURES_MNGR_REF] = TexturesManager;
	boometer->Directory[CHARS_TEXTURES_MNGR_REF] = TexturesManager;
}

/**
* Change Render mode for the Interfaces
*/
void CGUI::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	switch (Mode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		break;
	case CHAR_2D:
		boometer->Position.v[XDIM] = SIGLBD_PG_RIGHT + 1.0;
		boometer->Position.v[YDIM] = CPL_PLAYER_Y_2D - 0.5;
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		boometer->Position.v[XDIM] = SIGLBD_PG_RIGHT - 1.0;
		boometer->Position.v[YDIM] = CPL_PLAYER_Y_2D - 0.5;
		break;
	default: return;
	}
}

/**
* Show Spectrum 3D
*/
void CGUI::DisplaySpectrum(void)
{
	/*
	static int		choix_anim		= 0;
	float			beat_detection	= 0;
	static float	beat_scale		= 0;
	float			beat_sum		= 0;
	static float	xrot			= 0;
	static float	yrot			= 0;
	static float	zrot			= 0;

	// on récupère le spectrum
	float* spec = FSOUND_DSP_GetSpectrum();

	for (int i=0;i<=70;i++)
	beat_detection += spec[i];
	beat_detection /= 70 - 0.0;

	if (beat_detection > beat_scale + 0.0)
	beat_scale = beat_detection;

	beat_scale -= 0.007;

	char my_string[300]=UGKS_EMPTY_STRING;
	_ltoa(beat_scale, my_string, 10);
	//OutputDebugString(_ltoa(beat_scale, my_string, 10));
	//OutputDebugString(_ltoa(10.0, my_string, 10));
	//fontDrawString (100, 300, "beat float =  %f, beat string = %s", beat_scale,my_string);
	//

	if (CHAR_2D != Game->RenderMode && choix_anim == 0)
	{
	glPushMatrix();

	glLoadIdentity ();
	gluLookAt(	0.0, 0.0, 20.0,										// l'oeil
	0.0, 0.0,  0.0,										// le centre
	0.0, 1.0,  0.0	);									// le haut

	glTranslatef ( 0.0, 0.0, 8 );

	beat_sum = 0.012f + beat_scale * 0.07f;

	MeshesManager.Mesh[MESH2PLAYER].scale = beat_sum;

	// textes
	FontsManager.SetFontSize();fontDrawString (260, Application.Window.Height - FontsManager.FontVerticalSpace*2.9 , "beat detec =  %f", beat_detection);
	FontsManager.SetFontSize();fontDrawString (260, Application.Window.Height - FontsManager.FontVerticalSpace*4.2 , "beat scale =  %f", beat_scale);
	FontsManager.SetFontSize();fontDrawString (260, Application.Window.Height - FontsManager.FontVerticalSpace*5.5 , "beat sum   =  %f", beat_sum);

	// tambourin
	MeshesManager.Mesh[MESH2PLAYER].pos.v[XDIM] =  5.2;
	MeshesManager.Mesh[MESH2PLAYER].pos.v[YDIM] = -3.7;

	MeshesManager.Mesh[MESH2PLAYER].rot.v[XDIM] +=  0.8f;
	MeshesManager.Mesh[MESH2PLAYER].rot.v[YDIM] +=  0.4f;
	MeshesManager.Mesh[MESH2PLAYER].rot.v[ZDIM] +=  0.6f;

	// LUMIERE
	static float ambient[] =			{0.8, 0.8, 0.8, 1.0}; //0.9
	static float diffuse[] =			{0.2, 0.2, 0.2, 1.0}; //0.5
	glLightfv(SIGLB_SHIP_LIGHT, GL_AMBIENT, ambient);
	glLightfv(SIGLB_SHIP_LIGHT, GL_DIFFUSE, diffuse);

	// LUMIERE (MATERIAU)
	static float mat_ambient[] = {0.5, 0.5, 0.5, 1.0}; //0.7 ou 0.8
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	static float mat_diffuse_ships[] = {0.1, 0.1, 0.1, 1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_ships);

	// LUMIERE (POSITION)
	static float ShipsLightPos[] = {1.0, 1.0, 15.0, 0.0};
	glLightfv(SIGLB_SHIP_LIGHT, GL_POSITION, ShipsLightPos);

	MeshesManager.Mesh[MESH2PLAYER].Draw();

	glPopMatrix();
	}
	*/
}

/**
* @fn display_ring_2d_to_3d
* Display changes 2D to 3D textures
* This function allows you to see the changes undergone by the texture to get a bonus
* For this function gives the player and the type of bonus that was
* Bonus Depending on the texture changes from 2D to 3D or not the change is made
* You define which is also sound for such a change
*/
void CGUI::DisplayRing2D23D()
{
	Position = { 0.0f, 0.0f, 0.0f };

	// The bonus has been catched and the transition to 3D view mode is on
	if (Passing2D23D)
	{
		// on gère le tremblement du vaisseau du joueur et du cercle
		static float player_yi_acc = 0.05;

		if (BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health > 2.0f)
			Players[CurrentPlayer]->yi_speed += player_yi_acc;
		else Players[CurrentPlayer]->yi_speed -= player_yi_acc;

		if (Players[CurrentPlayer]->yi_speed >= 0.6)
			Players[CurrentPlayer]->yi_speed = 0.6;

		else if (Players[CurrentPlayer]->yi_speed <= -0.4 && CHAR_2D != Game->RenderMode)
			Players[CurrentPlayer]->yi_speed = -0.4;

		Players[CurrentPlayer]->yi_cos += Players[CurrentPlayer]->yi_speed;

		if (Players[CurrentPlayer]->yi_cos >= 3.14 || Players[CurrentPlayer]->yi_cos <= -3.14)
			Players[CurrentPlayer]->yi_speed *= -1;

		if (BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health >= 1.0f)
			Players[CurrentPlayer]->yi = cosf(Players[CurrentPlayer]->yi_cos * 50) / 40;	// cosf(a * b) / c - d : c=amplitude, d=position haut-bas
		else Players[CurrentPlayer]->yi = cosf(Players[CurrentPlayer]->yi_cos * 50) / 60;	// l'amplitude est réduite vers la fin

		Players[CurrentPlayer]->Mesh->modelo.pos.y = Players[CurrentPlayer]->Position.v[YDIM] + Players[CurrentPlayer]->yi;

		// Affichage du cercle
		static float x, y, z;

		x = Players[CurrentPlayer]->Position.v[XDIM];
		y = Players[CurrentPlayer]->Position.v[YDIM] + Players[CurrentPlayer]->yi;
		z = Players[CurrentPlayer]->Position.v[ZDIM];

		float tx = 0;

		// cercle game 2D
		if (CHAR_2D == Game->RenderMode)
		{
			// ring
			glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
			Position.Set(x, y, 0.01f);
			RenderQuad(CTM_PARTICLE3, 0.55f, 0.55f);
		}

		// cercle game 3D
		else
		{
			/*			// sphere
			glPushMatrix();
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glTranslatef(MeshesManager.Mesh[MESH2PLAYER].pos.v[XDIM] , MeshesManager.Mesh[MESH2PLAYER].pos.v[YDIM], MeshesManager.Mesh[MESH2PLAYER].pos.v[ZDIM]);
			glTranslatef(0, 0.2, 0);
			glRotatef(90, 0, 1, 0);
			static float angle_sphere = 0;
			angle_sphere += 1;
			glRotatef(angle_sphere, 1, 0, 0);
			glBindTexture (GL_TEXTURE_2D, texture[53]);
			//glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_LASER]->Explosion.Health);
			glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D]->Explosion.Health > 0.2 ? 0.2 : Bonus[CBN_BONUS_3D]->Explosion.Health);
			//gluSphere(Game->quadratic,0.73f,2,32); //0.78 0.75
			gluSphere(Game->quadratic,0.78f,32,32);//0.71
			glPopMatrix();
			*/
			// ring
			static float angle_ring = 0;
			if (BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health > 1.5)
				angle_ring += RenderTimer.GetAlarmPeriodms()*1.5; // /1.5
			else if (BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health > 1.0f)
				//angle_ring += Time.ms10 * Bonus[CBN_BONUS_3D]->Explosion.Health/1.5;
				angle_ring += RenderTimer.GetAlarmPeriodms();
			else if (BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health > 0.5)
				angle_ring += RenderTimer.GetAlarmPeriodms()*0.2; // /5
			else
				angle_ring += RenderTimer.GetAlarmPeriodms() *0.125;// /8

			if (angle_ring > 360)
				angle_ring = fmod(angle_ring, 360);
			/*			for (int i=0; i<=5; i++)
			{
			glPushMatrix();
			glDisable(GL_DEPTH_TEST);
			tx = -0.05; //0.08
			glDisable(GL_CULL_FACE);
			glTranslatef(MeshesManager.Mesh[MESH2PLAYER].pos.v[XDIM] , MeshesManager.Mesh[MESH2PLAYER].pos.v[YDIM], MeshesManager.Mesh[MESH2PLAYER].pos.v[ZDIM]);
			glTranslatef(0, 0.2, 0);
			//glRotatef(angle_ring*3, 1, 1.2, 1.3);
			//glRotatef(90, 0, 0, 1);
			if (i < 10)
			glRotatef(angle_ring+i*30, 1, 0, 0); //60
			//glRotatef(angle_ring, 1, 1, 0);
			else if (i == 1)
			//glRotatef(angle_ring/2, -1.1, -1.1, -1.1);
			glRotatef(angle_ring+45, -1, -1, 0);
			else if (i == 2)
			//glRotatef(angle_ring/3, -1.2, 1, 1);
			glRotatef(angle_ring+90, 1, -1, 0);
			else if (i == 3)
			//glRotatef(angle_ring/4, 1, -1.1, -1.1);
			glRotatef(angle_ring+135, -1, 1, 0);

			glBindTexture (GL_TEXTURE_2D, TexturesManager.Textures[CTM_PARTICLE3_3D].Id);//52 43
			//glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D]->Explosion.Health*1);
			//glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D]->Explosion.Health > 0.15 ? 0.15 : Bonus[CBN_BONUS_3D]->Explosion.Health);
			if (Bonus[CBN_BONUS_3D]->Explosion.Health > 2.0f)
			glColor4f (1.0f, 1.0f, 1.0f, 0.3);
			else
			glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D]->Explosion.Health *0.15);
			{
			glBegin (GL_QUADS);
			glTexCoord2f (0,1);glVertex2f (-tx - 1.0f, +tx + 1.0f);//1.2
			glTexCoord2f (1,1);glVertex2f (+tx + 1.0f, +tx + 1.0f);//1.2
			glTexCoord2f (1,0);glVertex2f (+tx + 1.0f, -tx - 1.0f);//0.8
			glTexCoord2f (0,0);glVertex2f (-tx - 1.0f, -tx - 1.0f);//0.8
			glEnd ();
			glEnable(GL_DEPTH_TEST);
			}
			glPopMatrix();
			}*/

			// ring
			glColor4f(1.0f, 1.0f, 1.0f, BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health > 0.4 ? 0.4 : BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			Position.Set(Players[CurrentPlayer]->Mesh->modelo.pos.x, Players[CurrentPlayer]->Mesh->modelo.pos.y + 0.2, Players[CurrentPlayer]->Mesh->modelo.pos.z);
			RenderQuad(CTM_PARTICLE3_3D, 1.03f, 1.03f);
			glEnable(GL_DEPTH_TEST);

			// sphere
			/*			glPushMatrix();
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glTranslatef(MeshesManager.Mesh[MESH2PLAYER].pos.v[XDIM] , MeshesManager.Mesh[MESH2PLAYER].pos.v[YDIM], MeshesManager.Mesh[MESH2PLAYER].pos.v[ZDIM]);
			glTranslatef(0, 0.2, 0);
			glRotatef(90, 0, 1, 0);
			static float angle_sphere = 0;
			angle_sphere += 1;
			glRotatef(angle_sphere, 1, 0, 0);
			//glRotatef(angle_ring, 1, 0, 0);
			glBindTexture (GL_TEXTURE_2D, texture[53]);
			glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D]->Explosion.Health);
			//glColor4f (1.0f, 1.0f, 1.0f, Bonus[CBN_BONUS_3D]->Explosion.Health > 0.4 ? 0.4 : Bonus[CBN_BONUS_3D]->Explosion.Health);
			gluSphere(Game->quadratic,0.78f,32,32); //0.78 0.75
			glPopMatrix();
			glDisable(GL_CULL_FACE);
			*/
		}

		// reset when ending
		//ACHTUNG: NO LO ENTIENDO
		/*if (BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health <= 0.0f)
		{
		BonusManager->Bonus[CBN_BONUS_3D]->Active			= false;
		BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Alive	= false;
		Players[CurrentPlayer]->yi				= 0.00;
		Players[CurrentPlayer]->yi_speed		= 0.01;
		Players[CurrentPlayer]->yi_cos			= 0;

		Game->Sound2D3DPassed = false;
		}*/

		// Going to game 3D
		if (BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health < 2.0 && CHAR_2D == Game->RenderMode)
		{
			try
			{
				Game->ChangeRenderMode(CHAR_3D);
				Game->OutEvent(CSIG_3D_CHANGED);	//v Controlado 6->4  //Turning back from CSIG_PASSING_2_3D state to CSIG_PLAYING state
													//Game->Level = 1;
													//MeshesManager.GetMesh(Players[CurrentPlayer]->Mesh->IndexInList]->modelo.pos.x = Players[CurrentPlayer]->Position.v[XDIM];									// spécifie la position du Player 3D car sinon l'anneau serait affiché au centre lors du passage dans la prochaine boucle.
													//Players[CurrentPlayer]->Speed.Reset();
			}
			catch (...)
			{
				ERR(UGKS_string2LPCWSTR(CGUI_ParserMsgs[CIP_AVERAGE_ERROR]));
			}
		}

		// tiembla la escena
		/*		if (Bonus[CBN_BONUS_3D]->Explosion.Health <= 3.5 && Bonus[CBN_BONUS_3D]->Explosion.Health >=0.5)
		{
		//glRotatef(-Bonus[CBN_BONUS_LASER]->Shake2, -0.3f, 0.5f, 1.0f);

		if (Bonus[CBN_BONUS_3D]->Explosion.Health >= 1.5f)
		{
		Bonus[CBN_BONUS_3D]->Shake2 += Bonus[CBN_BONUS_3D]->Shake1;
		Bonus[CBN_BONUS_3D]->Shake1 *= 1.05;
		}
		else
		{
		Bonus[CBN_BONUS_3D]->Shake2 -= Bonus[CBN_BONUS_3D]->Shake1;
		Bonus[CBN_BONUS_3D]->Shake1 *= 0.99;
		}

		if (Bonus[CBN_BONUS_3D]->Shake1 >= 1.2)
		Bonus[CBN_BONUS_3D]->Shake1 = 1.2;

		else if (Bonus[CBN_BONUS_3D]->Shake1 <= 0.5)
		Bonus[CBN_BONUS_3D]->Shake1 = 0.5;

		Bonus[CBN_BONUS_3D]->Shake2 *= rand()%4*0.1 - 0.2;

		//glRotatef(Bonus[CBN_BONUS_3D]->Shake2, -0.3f, 0.5f, 1.0f);
		}
		*/
		// que disminuye la vida de la "explosión" de Bono
#ifdef DEBUG_PASSAGE_3D
		if (Bonus[CBN_BONUS_LASER]->Explosion.Health - 0.01*RenderTimer.GetAlarmPeriodms() <= 0)			// no se exceda el límite de 0
			Bonus[CBN_BONUS_LASER]->Explosion.Health = 0.0f;						// tenemos 0
		else
			Bonus[CBN_BONUS_LASER]->Explosion.Health -= 0.01*RenderPeriod;
#else
		if (BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health - 0.001*RenderTimer.GetAlarmPeriodms() <= 0)			// no se exceda el límite de 0
			BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health = 0.0f;						// tenemos 0
		else
			BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health -= 0.001*RenderTimer.GetAlarmPeriodms();
#endif
		// su CROSS (2D -> 3D)
		if (!Game->Sound2D3DPassed && BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health <= 2.4 && BonusManager->Bonus[CBN_BONUS_3D]->Explosion.Health > 0.0)//...3.0...
		{
			Game->Sound2D3DPassed = true;

			SoundsManager.Play(CGS_CROSS_SND, UGKSND_MAX_VOLUME);
		}
	}
}

/**
*@fn void CGUI::DisplayScores()
* Display the player score and the higest score
* This function lets you see the punctuation
* These are calculated and then displayed in the coordinates defined
*/
void CGUI::DisplayScores()
{
	CTexturesManager * TextMngr = (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF];

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	//Current score at the left side up of the screen
	Position = { -5.0f, 5.7f, 0.0f };
	RenderQuad(CTM_SCORE, CI_SCORE_WIDTH, CI_SCORE_HEIGHT);

	_itoa(Game->Score, Number, 10);        // integer to string, en base 10

	NumberLength = strlen(Number);
	int long_0 = CGUI_MAX_SCORE_LENGTH - NumberLength, i;

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//Render initial zeroes
	TextMngr->Textures[CTM_NUMBER0]->SetTexture();
	Position = { -6.3f, 5.0f, 0.0f };
	for (i = 0; i< long_0; i++)
	{
		RenderQuad(CI_SMALLCHAR_WIDTH, CI_SMALLCHAR_HEIGHT);
		Position.v[XDIM] += CI_SMALLCHAR_ADVANCE;
	}

	//Render the rest of the numbers
	for (i = long_0; i < CGUI_MAX_SCORE_LENGTH; i++)
	{
		NumberTexture = (int)(Number[i - long_0]) - '0';
		RenderQuad((CTM_Textures)(NumberTexture + CTM_NUMBER0), CI_SMALLCHAR_WIDTH, CI_SMALLCHAR_HEIGHT);
		Position.v[XDIM] += CI_SMALLCHAR_ADVANCE;
	}

	// Highest SCORE at the centre up of the screen
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	Position.Set(-0.1f, 5.7f, 0.0f);
	RenderQuad(CTM_HISCORE, CI_SCORE_WIDTH*1.2, CI_SCORE_HEIGHT);

	_itoa(Game->HiScore, Number, 10);			// integer to string, in base 10

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//Set zeroes on the left
	NumberLength = strlen(Number);
	long_0 = CGUI_MAX_SCORE_LENGTH - NumberLength;
	TextMngr->Textures[CTM_NUMBER0]->SetTexture();
	Position.Set(-1.5f, 5.0f, 0.0f);
	for (i = 0; i< long_0; i++)
	{
		RenderQuad(CI_SMALLCHAR_WIDTH, CI_SMALLCHAR_HEIGHT);
		Position.v[XDIM] += CI_SMALLCHAR_ADVANCE;
	}

	for (i = long_0; i<CGUI_MAX_SCORE_LENGTH; i++)
	{
		NumberTexture = (int)(Number[i - long_0]) - '0';
		RenderQuad((CTM_Textures)(CTM_NUMBER0 + NumberTexture), CI_SMALLCHAR_WIDTH, CI_SMALLCHAR_HEIGHT);
		Position.v[XDIM] += CI_SMALLCHAR_ADVANCE;
	}
}


/**
* @fn void CGUI::DisplayLives()
* Show the amount of lives (ships) the player still has
*/
void CGUI::DisplayLives()
{
	CTexturesManager * TextMngr = (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF];

	//Display the lives on the left down corner
	Position = { CI_BELOW_LIFES_X , CI_BELOW_LIFES_Y, 0.0f };

	//The number of lives remaining
	RenderQuad((CTM_Textures)(CTM_NUMBER0 + Players[CurrentPlayer]->Lives), CI_CHAR_WIDTH, CI_CHAR_HEIGHT);

	// ALPHA TEST + BLEND
	if (!Alpha)
		glEnable(GL_ALPHA_TEST);						// for TGA alpha test

	glAlphaFunc(GL_GREATER, 0.3f);						// for TGA alpha test
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// for anti-aliasing

														// las naves de jugador abajo a la izquierda
	for (int k = 1; k <= Players[CurrentPlayer]->Lives; k++)
	{
		Position.v[XDIM] += CI_CHAR_ADVANCE;
		RenderQuad((CTM_Textures)(Players[CurrentPlayer]->IndTexture2D), CI_CHAR_WIDTH, CI_CHAR_HEIGHT);
	}

	// Numerical lives on the right up corner
	Position.Set(4.55f, 5.7f, 0.0f);
	RenderQuad(CTM_LIVES, CI_SCORE_WIDTH, CI_SCORE_HEIGHT);

	_itoa(Players[CurrentPlayer]->Lives, Number, 10);        // integer to string, en base 10

	NumberLength = strlen(Number);
	int long_0 = 3 - NumberLength,
		i;

	TextMngr->Textures[CTM_NUMBER0]->SetTexture();
	Position.Set(4.0f, 5.0f, 0.0f);
	for (i = 0; i< long_0; i++)
	{
		RenderQuad(CI_SMALLCHAR_WIDTH, CI_SMALLCHAR_HEIGHT);
		Position.v[XDIM] += CI_SMALLCHAR_ADVANCE;
	}

	for (i = long_0; i< 3; i++)
	{
		NumberTexture = (int)(Number[i - long_0]) - '0';
		RenderQuad((CTM_Textures)(CTM_NUMBER0 + NumberTexture), CI_SMALLCHAR_WIDTH, CI_SMALLCHAR_HEIGHT);
		Position.v[XDIM] += CI_SMALLCHAR_ADVANCE;
	}
}

/**
* @fn display_splashscreen
* Show splash screen
* If this option is activated, disables 2D texture
* And is assigned to the splash screen
*/
void CGUI::DisplaySplashScreen()
{
	if (!Game->active_splash)
		return;

	static float splash_alpha = 1;

	//glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA); // doesn't work with ARB_MULTISAMLE
	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, splash_alpha);

	// Dessine le  splash screen
	glBegin(GL_QUADS);
	glVertex3f(-20, -20, 1);// -12 a 12
	glVertex3f(20, -20, 1);
	glVertex3f(20, 20, 1);
	glVertex3f(-20, 20, 1);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor4f(1, 1, 1, 1);

	//splash_alpha -= 0.0025 * Time.ms10;
	splash_alpha -= 0.0025 *RenderTimer.GetAlarmPeriodms();
	if (splash_alpha <= 0)
	{
		splash_alpha = 1.0;
		Game->active_splash = false;
	}
}

/**
* @fn display_console
* Display the console
* This is the window where it will launch and display the game
* Fits according to the resolution of the screen
* In addition to establishing the various parts of the console as panels and separations of the same
*/
void CGUI::DisplayConsole()
{
	Position = { 2.85f, 0.0f, 0.0f };

	// condiciones necesarias
	if (Application.Window.Width != UGKCGC_XGA_HOR_RES || Application.Window.Height != UGKCGC_XGA_VER_RES)
		return;
	if (!Console.Start && !Console.Moving && !Console.Active)
		return;

	glDisable(GL_TEXTURE_2D);

	// colores (con transparencia)
	if (CHAR_2D == Game->RenderMode)
		glColor4f(1.0f, 1.0f, 1.0f, 0.05f);
	else
		glColor4f(1.0f, 1.0f, 1.0f, 0.05f);

	// las diapositivas del panel de izquierda a derecha (de derecha a izquierda)
	static float panel_push;

	if (Console.Moving)
	{
		if (Console.ComeIn)
		{
			if (Console.Start)
				panel_push = CI_INITIAL_PANEL_PUSH;

			panel_push -= CI_SPEED_PANEL_PUSH * RenderTimer.GetAlarmPeriodms();
			if (panel_push < 0)		// If end position is exceeded
				panel_push = 0;		// Set to end position
		}
		else if (!Console.ComeIn)
		{
			if (Console.Start)
				panel_push = 0;
			if (panel_push < CI_INITIAL_PANEL_PUSH)
				panel_push += CI_SPEED_PANEL_PUSH * RenderTimer.GetAlarmPeriodms();
		}
	}
	// panel de visualización
	glPushMatrix();
	glLoadIdentity();
	Camera->LookFrom(panel_push, 0.0, 20.0);
	Camera->LookAt(panel_push, 0.0, 0.0);

	// parte inferior del panel
	if (CHAR_2D != Game->RenderMode)
		glColor4f(1.0f, 1.0f, 1.0f, 0.03f);//0.15 0.05
	else
		glColor4f(0.7f, 0.7f, 1.0f, 0.03f);

	RenderQuad(8.65f, 11.5f);

	// consola de fondo
	glDisable(GL_BLEND);
	switch (Game->RenderMode)
	{
	case CHAR_2D:
		glColor4f(0.04f, 0.04f, 0.03f, 1.0f);//0.05
		break;
	case CHAR_3D:
		glColor4f(0.00f, 0.384f, 0.651f, 0.05f);//0.05
		break;
	case CHAR_LINES3D:
		glColor4f(0.01f, 0.01f, 0.01f, 1.0f);
	}

	Position.v[XDIM] = -8.4f;
	Position.v[YDIM] = 3.6f;
	RenderQuad(2.1f, 1.1f);

	// bordes
	glEnable(GL_BLEND);
	glLineWidth(2.0f);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(3, 0xADAD);

	// bordes de la consola
	switch (Game->RenderMode)
	{
	case CHAR_2D:
		glColor4f(0.5f, 0.5f, 0.5f, 0.2f);//1.0
		break;
	case CHAR_3D:
		glColor4f(0.9f, 0.9f, 0.9f, 0.1f);//1.0
		break;
	case CHAR_LINES3D:
		glColor4f(0.7f, 0.7f, 0.8f, 0.15f);
	}
	glBegin(GL_LINE_LOOP);
	glVertex2f(-10.50, 4.65);
	glVertex2f(-10.50, 2.50);
	glVertex2f(-6.30, 2.50);
	glVertex2f(-6.30, 4.65);
	glEnd();
	// mini-screen bordes (viewport2)
	glBegin(GL_LINE_LOOP);
	glVertex2f(-10.50, -5.58);
	glVertex2f(-10.50, -2.45);
	glVertex2f(-6.30, -2.45);
	glVertex2f(-6.30, -5.58);
	glEnd();

	// borde del panel
	switch (Game->RenderMode)
	{
	case CHAR_2D:
		glColor4f(0.3f, 0.3f, 0.3f, 0.2f);//0.2
		break;
	case CHAR_3D:
		glColor4f(0.9f, 0.9f, 0.9f, 0.05f);//0.05
	}

	glBegin(GL_LINES);
	glVertex2f(-5.80, 11.50);
	glVertex2f(-5.80, -11.50);
	glEnd();

	glPopMatrix();

	float FontVerticalSpace_temp = FontsManager.GetFontVerticalSpace() *1.1;									// mayor espacio

																												// INPUT UTILISATEUR (LA LIGNE DE COMMANDE)
																												// input ligne 1
	char temp[21];
	char line1[21];
	strncpy(temp, Console.Text[Console.Line], CGUI_MAX_CONSOLE_MSG_COLS);
	temp[CGUI_MAX_CONSOLE_MSG_COLS] = '\0';
	strcpy(line1, ">");
	strcat(line1, temp);

	// cursor parpadeante
	double integer;
	double blink = modf((double)timeGetTime() / 1000, &integer);
	if (blink > 0.3)
		if (strlen(Console.Text[Console.Line]) < CGUI_MAX_CONSOLE_MSG_COLS)
			strcat(line1, "_");

	FontsManager.SetFontSize(FontsManager.GetFontSize() - 1);
	FontsManager.DrawFont(30 - Console.ViewportPush, 580 + Console.Line*FontVerticalSpace_temp, line1);
	//fontDrawString(30-ViewportPush, 580+Console.Line*FontVerticalSpace_temp, line1);

	// input ligne 2
	if (strlen(Console.Text[Console.Line]) >= CGUI_MAX_CONSOLE_MSG_COLS)
	{
		char* temp;
		char line2[21];
		temp = &Console.Text[Console.Line][CGUI_MAX_CONSOLE_MSG_COLS];
		strcpy(line2, temp);
		if (blink > 0.3)
			strcat(line2, "_");
		FontsManager.SetFontSize(FontsManager.GetFontSize() - 1);
		FontsManager.DrawFont(30 - Console.ViewportPush, 580 - 1 * FontVerticalSpace_temp, line2);
		//fontDrawString(30-ViewportPush, 580-1*FontVerticalSpace_temp, line2);
	}

	// ÉVALUATION DE LA COMMANDE 1
	if (strlen(Console.Command[0]) > 0)
	{
		// Console.Command (1)
		char temp[45] = UGKS_EMPTY_STRING;
		strcat(temp, "\r");
		strcat(temp, (const char*)Console.Command[0]);
		temp[CGUI_MAX_CONSOLE_MSG_COLS] = '\0';
		if (CHAR_2D == Game->RenderMode)
			FontsManager.SetFontColor(0.7);
		else
			FontsManager.SetFontColor(0.9);

		FontsManager.SetFontSize(FontsManager.GetFontSize() - 1);
		//fontDrawString(30-ViewportPush, 580-2*FontVerticalSpace_temp, temp);
		FontsManager.DrawFont(30 - Console.ViewportPush, 580 - 2 * FontVerticalSpace_temp, temp);

		// Console.Output (1)
		if (CHAR_2D == Game->RenderMode)
			FontsManager.SetFontColor(0.7, 0.5, 0.5);
		else
			FontsManager.SetFontColor(0.952f, 0.937f, 0.804f);

		if (strcmp(Console.Command[0], "HELP") == 0)					// help
			strcpy(Console.Output[0], UGKS_string2charstr(CGUI_ParserMsgs[CGUI_TYPE_VBLE_NAME]));
		else if (strcmp(Console.Command[0], "CAMERAX") == 0)			// variables
			_ltoa(Camera->Position.v[XDIM], Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "CAMERAY") == 0)
			_ltoa(Camera->Position.v[YDIM], Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "CAMERAZ") == 0)
			_ltoa(Camera->Position.v[ZDIM], Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "ANGLEX") == 0)
			_ltoa(Scene.Angle.v[XDIM], Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "ANGLEY") == 0)
			_ltoa(Scene.Angle.v[YDIM], Console.Output[0], 10);
		//else if (strcmp(Console.Command[0], "MUSICTYPE") == 0)
		//	_ltoa(SoundsManager.music_type, Console.Output[0], 10);
		else if (strcmp(Console.Command[0], "MUSICENCOURS") == 0)
			strncpy(Console.Output[0], Game->CurrentMusic.c_str(), 39);
		else if (strcmp(Console.Command[0], "MUSICAFFICHEE") == 0)
			strncpy(Console.Output[0], Game->music_affichee.c_str(), 39);
		else if (strcmp(Console.Command[0], "STREAMNAME") == 0)
			strncpy(Console.Output[0], Game->stream_name.c_str(), 39);
		else if (strcmp(Console.Command[0], "IDMYMUSIC") == 0)
			_itoa(Game->MusicConsoleOutput, Console.Output[0], 10);
		else		// invalid command
			strcpy(Console.Output[0], UGKS_string2charstr(CGUI_ParserMsgs[CGUI_TYPE_VBLE_NAME]));
		FontsManager.SetFontSize(FontsManager.GetFontSize() - 1);
		//fontDrawString(30-ViewportPush, 580-3*FontVerticalSpace_temp, Console.Output);
		FontsManager.DrawFont(30 - Console.ViewportPush, 580 - 3 * FontVerticalSpace_temp, Console.Output[0]);
	}

	// ÉVALUATION DE LA COMMANDE 2
	if (strlen(Console.Command[1]) > 0)
	{
		// Console.Command[1]
		char temp[45] = UGKS_EMPTY_STRING;
		strcat(temp, "\r");
		strcat(temp, Console.Command[1]);
		temp[CGUI_MAX_CONSOLE_MSG_COLS] = '\0';
		if (CHAR_2D == Game->RenderMode)
			FontsManager.SetFontColor(0.7);
		else
			FontsManager.SetFontColor(0.9);
		FontsManager.SetFontSize(FontsManager.GetFontSize() - 1);
		//fontDrawString(30-ViewportPush, 580-4*FontVerticalSpace_temp, temp);
		FontsManager.DrawFont(30 - Console.ViewportPush, 580 - 4 * FontVerticalSpace_temp, temp);

		// Console.Output[1]
		if (CHAR_2D == Game->RenderMode)
			FontsManager.SetFontColor(0.7, 0.5, 0.5);
		else
			FontsManager.SetFontColor(0.952f, 0.937f, 0.804f);

		if (strcmp(Console.Command[1], "CAMERAX") == 0)				// variables
			_ltoa(Camera->Position.v[XDIM], Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "CAMERAY") == 0)
			_ltoa(Camera->Position.v[YDIM], Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "CAMERAZ") == 0)
			_ltoa(Camera->Position.v[ZDIM], Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "ANGLEX") == 0)
			_ltoa(Scene.Angle.v[XDIM], Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "ANGLEY") == 0)
			_ltoa(Scene.Angle.v[YDIM], Console.Output[1], 10);
		//else if (strcmp(Console.Command[1], "MUSICTYPE") == 0)
		//	_ltoa(SoundsManager.music_type, Console.Output[1], 10);
		else if (strcmp(Console.Command[1], "MUSICENCOURS") == 0)
			strncpy(Console.Output[1], Game->CurrentMusic.c_str(), 39);
		else if (strcmp(Console.Command[1], "MUSICAFFICHEE") == 0)
			strncpy(Console.Output[1], Game->music_affichee.c_str(), 39);
		else if (strcmp(Console.Command[1], "IDMYMUSIC") == 0)
			_itoa(Game->MusicConsoleOutput, Console.Output[1], 10);

		FontsManager.SetFontSize(FontsManager.GetFontSize() - 1);
		FontsManager.DrawFont(30 - Console.ViewportPush, 580 - 5 * FontVerticalSpace_temp, Console.Output[1]);
		//fontDrawString(30-ViewportPush, 580-5*FontVerticalSpace_temp, Console.Output[1]);
	}

	glEnable(GL_TEXTURE_2D);

}

/**
* @fn display_console_mini_screen
* Display a Small console
* This window is generated when it is played in full screen
* Where is recalculated the position of the window besides the rearrangement of the textures
*/
void CGUI::display_console_mini_screen()
{
	// Necessary conditions
	if (Application.Window.Width != UGKCGC_XGA_HOR_RES || Application.Window.Height != UGKCGC_XGA_VER_RES)
		return;

	if (Console.Start || Console.Moving || Console.Active)
	{
		// activación: se desliza de izquierda a derecha
		if (Console.ComeIn && Console.Moving)
		{
			if (Console.Start)
			{
				Console.ViewportPush = 244.8;	//244.8 = (26+193+25.8))
				Console.Start = false;
			}
			if (Console.ViewportPush - (2.311 *RenderTimer.GetAlarmPeriodms()) > 0)		// no deberá exceder la posición de destino 2.311 = ((26+193+26.0)/53)/2
				Console.ViewportPush -= 2.311 *RenderTimer.GetAlarmPeriodms();			//2.311 = ((26+193+26.0)/53)/2
			else
			{
				Console.Moving = false;
				Console.Active = true;
				Console.ComeIn = false;
				Console.ViewportPush = 0;
			}
		}
		// désactivation : il coulisse de droite à gauche
		else if (!Console.ComeIn && Console.Moving)
		{
			if (Console.Start)
			{
				Console.ViewportPush = 0.0;
				Console.Start = false;
			}
			if (Console.ViewportPush < 244.8) //244.8 = (26+193+25.8))
				Console.ViewportPush += 2.311*RenderTimer.GetAlarmPeriodms();	//2.311 = ((26+193+26.0)/53)/2
			else
			{
				Console.Moving = false;
				Console.Active = false;
				Console.ComeIn = true;
			}
		}

		// affichage du mini-screen
		glViewport(26 - ceil(Console.ViewportPush), 126, 193, 191 * 3 / 4);
		CGL_ResetProjection();
		gluPerspective(45.0, (GLfloat)(UGKCGC_XGA_HOR_RES) / (GLfloat)(UGKCGC_XGA_VER_RES), 0.1f, 500.0); // Set Up Perspective Mode To Fit 1/4 The Screen (Size Of A Viewport)
		CGL_ResetModelView();												// Reset The Modelview Matrix
		Camera->SetLook();

		glClear(GL_DEPTH_BUFFER_BIT);

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		// mini-écran : fond noir sans transparence (couche 1)
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);	glVertex3f(-12, 12, -.15f);
		glTexCoord2f(1, 0);	glVertex3f(-12, -12, -.15f);
		glTexCoord2f(1, 1);	glVertex3f(12, -12, -.15f);
		glTexCoord2f(0, 1);	glVertex3f(12, 12, -.15f);
		glEnd();

		// mini-écran : fond gris avec transparence (couche 2)
		glEnable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 0.0f);//0.15 0.05
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);	glVertex3f(-12, 12, -.15f);
		glTexCoord2f(1, 0);	glVertex3f(-12, -12, -.15f);
		glTexCoord2f(1, 1);	glVertex3f(12, -12, -.15f);
		glTexCoord2f(0, 1);	glVertex3f(12, 12, -.15f);
		glEnd();

		glRotatef(Scene.Angle.v[XDIM], 1.0, 0.0, 0.0);
		glRotatef(Scene.Angle.v[YDIM], 0.0, 1.0, 0.0);

		glEnable(GL_TEXTURE_2D);

		//Background.Render();

		DisplayLives();		// Amount of remianing lives
		DisplayScores();	// Score and Highest score on the screen

							// PARAMETRES 3D (DEBUT)
		if (CHAR_2D != Game->RenderMode)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
		}
		// LUMIERE DES SHIPS (DEBUT)
		if (CHAR_2D != Game->RenderMode)
		{
			glEnable(GL_LIGHTING);
			glEnable(SIGLB_SHIP_LIGHT);
		}

		if (CHAR_2D != Game->RenderMode)
		{
			glDisable(SIGLB_SHIP_LIGHT);
			glDisable(GL_LIGHTING);

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glDisable(GL_CULL_FACE);
		}
	}
}

/**
* @fn display_text_frames
* Display text frames
* This function is introduced to the window frames
* Pear this depends on the size of the window frames will inputting the coordinates of the window
*/
void CGUI::DisplayTextFrames()
{
	CTexturesManager * TextMngr = (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF];

	// si la résolution est non standard on n'affiche pas les cadres
	if ((Application.Window.Width != UGKCGC_UXGA_HOR_RES || Application.Window.Height != UGKCGC_UXGA_VER_RES)
		&& (Application.Window.Width != UGKCGC_SXGA_HOR_RES || Application.Window.Height != UGKCGC_SXGA_VER_RES43)
		&& (Application.Window.Width != UGKCGC_XGA_HOR_RES || Application.Window.Height != UGKCGC_XGA_VER_RES)
		&& (Application.Window.Width != UGKCGC_SVGA_HOR_RES || Application.Window.Height != UGKCGC_SVGA_VER_RES)
		&& (Application.Window.Width != UGKCGC_VGA_HOR_RES || Application.Window.Height != UGKCGC_VGA_VER_RES))
		return;

	glPushMatrix();
	glLoadIdentity();
	Camera->LookFrom(0.0, 0.0, 20.0);
	Camera->LookAt(0.0, 0.0, 0.0);

	glDisable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// CADRE HAUT-DROITE  ¯|
	float lw = 0;								// lang Width : must be resized depending on the language
	if (LANG_FRENCH == Application.Window.Lang)
		lw = 0.1;
	else if (LANG_SPANISH == Application.Window.Lang)
		lw = 0.2;
	float lx = 0;									// lang x

	Position.v[XDIM] = 8.7f + lx + lw;
	Position.v[YDIM] = 6.75f;
	Position.v[ZDIM] = 0.0f;

	RenderQuad(CTM_CADRE_CENTRE, 1.8f, 1.05f);

	// CADRE BAS-GAUCHE  |_
	if (LANG_FRENCH == Application.Window.Lang)
		lw = 0.18;
	else if (LANG_SPANISH == Application.Window.Lang)
		lw = -0.25;
	else   lw = 0;

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);	glVertex2f(-8.7f - 1.80f, -8.4f + 1.13f);
	glTexCoord2f(0, 0);	glVertex2f(-8.7f - 1.80f, -8.4f + 0.30f);
	glTexCoord2f(1, 0);	glVertex2f(-8.7f + 1.25f + lw, -8.4f + 0.30f);
	glTexCoord2f(1, 1);	glVertex2f(-8.7f + 1.25f + lw, -8.4f + 1.13f);
	glEnd();

	// CADRE HAUT-GAUCHE  |¯
	TextMngr->Textures[CTM_CADRE_CENTRE_LONG]->SetTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);	glVertex2f(-8.7f - 1.8f, +6.6f + 1.30f);
	glTexCoord2f(0, 0);	glVertex2f(-8.7f - 1.8f, +6.6f + 0.20f);
	glTexCoord2f(1, 0);	glVertex2f(-8.7f + 5.4f, +6.6f + 0.20f);
	glTexCoord2f(1, 1);	glVertex2f(-8.7f + 5.4f, +6.6f + 1.30f);
	glEnd();

	glPopMatrix();
}

/**
* @fn display_Health
* Show the player's health on the screen
* This function displays the player on screen health
* Also being showing the life disminusicion
*/
void CGUI::DisplayHealth()
{
	boometer->Render(Players[CurrentPlayer]->Health / Players[CurrentPlayer]->MaxHealth);
}

/**
* @fn DisplayPressKey
* Display the buttons to press
* This function displays the button is pressed
*/
void CGUI::DisplayPressKey()
{

	Game->ReshapeGL();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	Position.v[XDIM] = 0.0f;
	Position.v[YDIM] = 1.43f;
	Position.v[ZDIM] = SIGLBD_DEFAULT_Z_POSITION;

	RenderQuad(CTM_PRESSKEY, 3.9f, 0.78f);
}

/**
* @fn DisplayPaused
* Show display paused
* This function will pause the game window
*/
void CGUI::DisplayPaused()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	Position.v[XDIM] = 0.0f;
	Position.v[YDIM] = 1.43f;
	Position.v[ZDIM] = SIGLBD_DEFAULT_Z_POSITION;

	RenderQuad(CTM_PAUSED, 3.9f, 0.78f);
}

/**
* @fn FadingInWon
* Show the Fading animation presentation
* The actual score

*/
void CGUI::FadingInWon(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// efface les buffers

																	// diminution du volume de la musique de la dialog box 'About'
#ifdef UGKSND_FMOD375
	FMUSIC_SetMasterVolume(SoundsManager.FadingModule, (int)((1 - Fading)*UGKSND_MAX_VOLUME));
#elif UGKSND_FMODEx
#elif UGKSND_OAL
#elif UGKSND_SDL
#endif

																	// Showing the Loading bmp on the screen
	LoadingBMPPosition = 0.3 + sqrt(sqrt(1 / Fading));
	//Game->DisplayLoadingBMP();

	if (Fading < 1.0)
		if (RenderTimer.IsSounding())
		{
			RenderTimer.ResetAlarm();
			Fading += CI_FadingSpeed * RenderTimer.GetAlarmPeriodms();
		}
		else
		{
#ifdef UGKSND_FMOD375
			FMUSIC_StopSong(SoundsManager.FadingModule);
			FMUSIC_FreeSong(SoundsManager.FadingModule);
#elif UGKSND_FMODEx
#elif UGKSND_OAL
#elif UGKSND_SDL
#endif

			//Passing from CSIG_CONFIGURE state to CSIG_FADING_IN state
			Game->OutEvent(CSIG_FADED);		//v Controlado 11->9, 13->10, 12->14
		}

	glFlush();															// flush the GL rendering pipeline
}


/**
* @fn CSIGame::DisplayLoadingBMP
* Show the Image "Loading"
* Starts the colors in 1.0f
* After Despues une la textura 2D con el Manegador de Texturas
* After the image is displayed at the coordinates given loaded
*/
void CGUI::DisplayLoadingBMP()
{
	glColor4f(0.0f, 0.0f, 0.0f, Fading);

	Position.v[XDIM] = 0.1f*LoadingBMPPosition;
	Position.v[YDIM] = 1.1f*LoadingBMPPosition;
	Position.v[ZDIM] = SIGLBD_DEFAULT_Z_POSITION;

	RenderQuad(CTM_LOADING, 5.0f*LoadingBMPPosition, 4.0f*LoadingBMPPosition);
}

/**
* @fn FadingOutLoading
* View the Fading out of charge
* This is done to Fading is loaded when a game is completed loaded bar
* For this you use the buffers
*/
void CGUI::FadingOutLoading(void)
{
	CTexturesManager * TextMngr = (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF];

	//static float Fading = 1.0f;											// Maximum lighting
	static float rotation_angle_total = 0;

	Application.Window.CreateRenderingContext();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Erase graphic buffers

																	// rotation of the loading image 
	rotation_angle_total = (pow((float)1 - Fading, (float)1.3)) * 90;

	glLoadIdentity();
	Camera->LookFrom(0.0, 0.0, 10.0);
	Camera->LookAt(0.f, 0.f, 0.f);

	glRotatef(rotation_angle_total, 0.0f, 1.0f, 1.0f);

	//Diminish the lighting of the screen
	glColor4f(1.0f, 1.0f, 1.0f, Fading);
	if (Fading <= 0.0)
		Game->AI->SetState(CSIG_WAIT_KEY);
	else
	{
		Fading -= 0.03 * RenderTimer.GetAlarmPeriodms();
		FontsManager.SetFontSize(8);						// Size of the logs text
		if (Application.Window.Width >= UGKCGC_SVGA_HOR_RES)// logs added if resolution >= 800x600
			for (int i = 0; i <= Log.LogIndex; i++)			// for every line of the log
			{
				FontsManager.SetFontSize();
				if (Application.Window.Width >= UGKCGC_XGA_HOR_RES)
					FontsManager.SetFontColor(Fading);
				else	FontsManager.SetFontColor(Fading - 0.2);
				FontsManager.DrawFont(10, Application.Window.Height - 30 - (FontsManager.GetFontSize() + 2)*i, "%s", Log.LogStrings[i]);
			}

		// Putting image "loading..." on the screen
		float j = 0.3 + sqrt(Fading);
		glColor4f(1.0f, 1.0f, 1.0f, Fading);
		TextMngr->Textures[CTM_LOADING]->SetTexture();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-3.0*j, 0.10*j, .05f);
		glTexCoord2f(1, 0); glVertex3f(3.0*j, 0.10*j, .05f);
		glTexCoord2f(1, 1); glVertex3f(3.0*j, 1.30*j, .05f);
		glTexCoord2f(0, 1); glVertex3f(-3.0*j, 1.30*j, .05f);
		glEnd();

		glFlush();															// flush the GL rendering pipeline
	}
}


/**
* @fn FadingInAnimation
* Show the Fading animation presentation
* The animation begins to move Logo Space Invader, while the discoloration fades
* For this you use the buffers, which also decreases other music from the animation

*/
int FadingInAnimation()
//DWORD WINAPI FadingInAnimation(LPVOID lpParam)
{
	GUI.Fading = 0.000025f;
	GUI.RenderTimer.ResetAlarm();

	Application.Window.SetRenderingContext();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Application.Window.Reshape(45.0f);

	Camera->SetLook();
	GUI.Position.v[ZDIM] = Camera->zoom_total;
	GUI.Rotation = Scene.Angle;
	GUI.Rotation.v[XDIM] = 180;
	glDisable(GL_BLEND);

	//Enable lighting
	glEnable(GL_LIGHTING);

	glDisable(SIGLB_PLAYER_LIGHT);
	glEnable(SIGLB_SHIP_LIGHT);
	glDisable(SIGLB_SHIP2_LIGHT);

	while (GUI.Fading < 1.0)
		if (GUI.RenderTimer.IsSounding())
		{
			GUI.RenderTimer.ResetAlarm();
			GUI.Fading += CI_FadingSpeed * GUI.RenderTimer.GetAlarmPeriodms();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear color and depth buffers

																// Reduce the volume of the music
#ifdef UGKSND_FMOD375
			FMUSIC_SetMasterVolume(SoundsManager.FadingModule, (int)((1 - Fading)*UGKSND_MAX_VOLUME));
#elif UGKSND_FMODEx
#elif UGKSND_OAL
#elif UGKSND_SDL
#endif																// Showing the Loading bmp on the screen
			GUI.LoadingBMPPosition = 0.3 + sqrt(sqrt(1.0f / GUI.Fading));
			GUI.DisplayLoadingBMP();
			Application.Window.Refresh();
		}

#ifdef UGKSND_FMOD375
	FMUSIC_StopSong(SoundsManager.FadingModule);
	FMUSIC_FreeSong(SoundsManager.FadingModule);
#elif UGKSND_FMODEx
#elif UGKSND_OAL
#elif UGKSND_SDL
#endif

	//Passing from CSIG_CONFIGURE state to CSIG_FADING_IN state
	Game->OutEvent(CSIG_LOADING);//v Controlado 1->2

	glFlush();															// flush the GL rendering pipeline

	return NULL;
}

///Renders the whole GUI after all the game scene is rendered
void CGUI::Render(void)
{
	unsigned int posX, posY;
	cTextoGL txtFPS, txtRND, txtUPD, txtIDL, txtSUM, txtCP, txtRTDSKMM;
	char Text[50];

	Blending = glIsEnabled(GL_BLEND),
		Alpha = glIsEnabled(GL_ALPHA_TEST);

	if (!Blending)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Display Game Stats
	DisplayStats();
	// Display Player Health
	DisplayHealth();
	// Number of lives
	DisplayLives();
	// Score, Hiscore, Lives
	DisplayScores();

	// BACK TO NON ALPHA TEST + PREVIOUS BLEND
	if (!Alpha)		glDisable(GL_ALPHA_TEST);
	if (!Blending)	glDisable(GL_BLEND);
}

///Renders the statistics on the right upper corner
void CGUI::DisplayStats(void)
{
	unsigned int posX, posY;
	cTextoGL txtFPS, txtRND, txtUPD, txtIDL, txtSUM, txtCP, txtRTDSKMM;
	char Text[50];

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	posX = TEXT_X_POS;
	posY = TEXT_Y_POS;
	sprintf(Text, "FPS %.2f", TimerManager.fps);
	txtFPS.text2D_draw(posX, posY, Text, Game->initialWidthW, Game->initialHeightW, yellow);
	posY += TEXT_Y_INC;
	sprintf(Text, "Rnd %.2f%%", TimerManager.pct[SIGLBT_RENDER_TIMING]);
	txtRND.text2D_draw(posX, posY, Text, Game->initialWidthW, Game->initialHeightW, yellow);
	posY += TEXT_Y_INC;
	sprintf(Text, "Upd %.2f%%", TimerManager.pct[SIGLBT_UPDATE_TIMING]);
	txtUPD.text2D_draw(posX, posY, Text, Game->initialWidthW, Game->initialHeightW, yellow);
	posY += TEXT_Y_INC;
	sprintf(Text, "Idl %.2f%%", TimerManager.pct[SIGLBT_IDLE_TIMING]);
	txtIDL.text2D_draw(posX, posY, Text, Game->initialWidthW, Game->initialHeightW, yellow);

	//Draw Info Texts
	if (Game->DiscreteSimulation)
	{
		posY += TEXT_Y_INC;
		sprintf(Text, "Rtdsk %.2f%%", TimerManager.pct[SIGLBT_RTDESK_TIMING]);
		txtIDL.text2D_draw(posX, posY, Text, Game->initialWidthW, Game->initialHeightW, yellow);
	}

	posY += TEXT_Y_INC;
	sprintf(Text, "Suma %% %.2f", TimerManager.pctSuma);
	txtSUM.text2D_draw(posX, posY, Text, Game->initialWidthW, Game->initialHeightW, yellow);
}

