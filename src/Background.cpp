/**
*	CBackground
*	Defines some methods of the class of a CBackground

*	Prefix: CBG_

*	@author 
*	@version 2014-05
*/

#ifndef CBG_BACKGROUND
#define CBG_BACKGROUND

#include <Background.h>
#include <SITexturesResources.h>
#include <UGKTexturesManager.h>
#include <SIGame.h>

extern CSIGame			*Game; // The game singleton
extern CTexturesManager	TexturesManager;

#define CBG_BG_WIDTH		16.0f
#define CBG_BG_LENGTH		16.0f
#define CBG_BG_HEIGHT		 0.1f
#define CBG_BG_CUBE_SIZE	12.0f

using namespace UGK;

CBackground::CBackground()
{
	ResetTransformations();

	UpdateAABB(CBG_BG_WIDTH, CBG_BG_LENGTH, CBG_BG_HEIGHT);

	RenderMode = CHAR_2D;

	IndTexture2D						= CTM_SKYBOX_BCKG;
	Type = CHARS_BACKGROUND;
	Directory.resize(CHARS_MAX_REFERENCES);
	Directory[CHARS_TEXTURES_MNGR_REF]	= (CCharacter *) &TexturesManager;
}

/**
* @fn void CBackground::Render(float levelRatio)
* Display the bottom of the window
* This function allows you to display the bottom of the window depending on whether the background object is in 2D or 3D mode
* Also depends on the level
* @param bool viewport2 represents the state in 3D
*/
void CBackground::Render(bool Rotate)
{
	Position.Set(0.0f, 0.0f, -SIGLBD_DEFAULT_Z_POSITION);	// .05 in order to avoid z-fighting
	Rotation.v[YDIM] = 180.0f;
	Rotation.v[ZDIM] = 180.0f;

	switch (RenderMode)
	{
		case CHAR_2D:
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			switch (Game->Level)
			{
			case 0:
			case 1:
				IndTexture2D = CTM_SPACE_BCKG;
				UpdateAABB(15.0f, 15.0f, 0.0f);
				CGUIGadget::Render();
				break;
			case 2:	// planet
				IndTexture2D = CTM_PLANET_BCKG;
				Position.v[ZDIM] += 5.0f;
				UpdateAABB(10.0f, 5.0f, 0.0f);
				CGUIGadget::Render();
				break;
			case 3:	// earth-moon
				Position.v[XDIM] += 2.0f;
				Position.v[ZDIM] += -15.0f;
				IndTexture2D = CTM_EARTH_MOON_BCKG;
				UpdateAABB(13.46f, 10.0f, 0.0f);
				CGUIGadget::Render();
				break;
			case 4:	// galaxy1
				IndTexture2D = CTM_GALAXY_BCKG;
				UpdateAABB (8.0f, 7.36f, 0.0f);
				CGUIGadget::Render();
				break;
			case 5: // skybox
				Position.v[ZDIM] = 51.0f;
				IndTexture2D = CTM_SKYBOX_BCKG;
				UpdateAABB(CBG_BG_CUBE_SIZE, CBG_BG_CUBE_SIZE, CBG_BG_CUBE_SIZE);
				DisplayCube();
				break;
			default:
				break;
			}
			// texture of the bottom stick
			IndTexture2D = CTM_BAND;
			break;
		case CHAR_3D:
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CW);

			if (Rotate)
			{
				glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
				Mesh->modelo.rot.z -= 0.07 * Game->DefaultUpdPeriod[CHARS_GAME];
			}
			else
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f); 

			Mesh->modelo.rot.x = 45;
			Mesh->modelo.rot.y = 90;

			Mesh->modelo.Draw();

			glDisable(GL_CULL_FACE);

			// texture of the bottom stick
			IndTexture2D = CTM_3D_BAND;
			break;
		default:
			break;
	}
	// Draw the bottom stick
	Position.Set(0.0f, -7.0f, -SIGLBD_DEFAULT_Z_POSITION+0.01);	// -.04 in order to avoid z-fighting

	UpdateAABB(12.0f, 0.1f, 0.0f);
	Rotation.v[YDIM] = 180.0f;
	Rotation.v[ZDIM] = 0.0f;
	CGUIGadget::Render();
}
#endif