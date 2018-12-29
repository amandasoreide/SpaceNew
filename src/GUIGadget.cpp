/*	Definition of the class Interface

	Prefix: CI_

	@author Ramon Molla
	@version 2011-11
	In this file all the functions of interface (display windows) player
*/

#include <GUIGadget.h>
#include <FontsManager.h>

#include <Player.h>
#include <Background.h>
#include <UGKLog.h>
#include <GameSounds.h>
#include <Application.h>
#include <UGKCamera.h>			// Header File class Camera
#include <Scene.h>

#include <SITexturesResources.h>
#include <UGKLog.h>

#include <GlobalDefs.h>
#include <glext.h>
#include <TextoGL.h>

extern CFontsManager	FontsManager;
extern CSoundsManager	SoundsManager;

extern CApplication		Application;
extern CCamera			*Camera;		///<Singleton declaration of the game camera
extern CLog				Log;			///<For logging performance, tracing,...
extern CScene			Scene;			///<Singleton to manage the Scene

extern CBackground		Background;

//2D Texts Colors
GLfloat red[3] = { 1.0f, 0.0f, 0.0f };
GLfloat gren[3] = { 0.0f, 1.0f, 0.0f };
GLfloat yellow[3] = { 1.0f, 1.0f, 0.0f };
GLfloat blue[3] = { 0.0f, 0.0f, 1.0f };

void CGUIGadget::RenderQuad(int T, float HalfWidth, float HalfHeight)
{
	CTexturesManager * TextMngr = (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF];

	TextMngr->Textures[T]->SetTexture();
	UpdateAABB(HalfWidth, HalfHeight, 0.0f);
	Render2D();
}

void CGUIGadget::RenderQuad(float HalfWidth, float HalfHeight)
{
	UpdateAABB(HalfWidth, HalfHeight, 0.0f);
	Render2D();
}

/**   
   * Initializer for the Interfaces  
*/
void CGUIGadget::Init ()
{
	Fading = 0.0001;
	UGK::Vector origin;
	
	LoadingBMPPosition = UGKCL_LOADINGBMP_FINAL_POSITION;

	origin.v[XDIM] = SIGLBD_PG_RIGHT + 1.0;
	origin.v[YDIM] = CPL_PLAYER_Y_2D - 0.5;

	ResetTransformations();
	RenderMode = CHAR_2D;	//GUIs are always rendered in 2D
}

///Renders the gadget
void CGUIGadget::Render(void)
{
	CTexturesManager * TextMngr = (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF];

	Blending = glIsEnabled(GL_BLEND),
	Alpha	 = glIsEnabled(GL_ALPHA_TEST);

	if (!Blending)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	TextMngr->Textures[IndTexture2D]->SetTexture();
	Render2D();

	// BACK TO NON ALPHA TEST + PREVIOUS BLEND
	if (!Alpha)		glDisable(GL_ALPHA_TEST);
	if (!Blending)	glDisable(GL_BLEND);
}

/**
* @fn DisplayCube()
* Display the Cube
* This function lets you draw a textured cube in the window. Use the same texture for all the sides
*/
void CGUIGadget::DisplayCube() //Show a cube in the space
{
	CTexturesManager * TextMngr = (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF];

	glPushMatrix();

	glTranslatef(Position.v[XDIM], Position.v[YDIM], Position.v[ZDIM]);

	//glEnable (GL_TEXTURE_2D);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the texture...
	TextMngr->Textures[IndTexture2D]->SetTexture();

	// Render the six faces...
	// Front Face
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
	glEnd();

	// Back Face
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
	glEnd();

	// Top Face
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
	glEnd();

	// Bottom Face
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
	glEnd();

	// Right face
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMAX][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
	glEnd();

	// Left Face
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMIN][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMAX][ZDIM].Value);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(CharAABB.AABB[CHAR_BBMIN][XDIM].Value, CharAABB.AABB[CHAR_BBMAX][YDIM].Value, CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);
	glEnd();

	glPopMatrix();
}

/**
* @fn DisplayCube
* Display the Cube
* @param texture_id int represents the number of texture
* @param float size representing the size of the cube
* This function lets you draw a cube texture in the window
*/
void CGUIGadget::DisplayCube(float size)
{
	UpdateAABB(size, size, size);


}