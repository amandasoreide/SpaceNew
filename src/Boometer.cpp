/**
*	CBoometer
*	Defines some methods of the class of a boometer

*	@author 
*	@version 2014-05
*/

#include <Boometer.h>
#include <SITexturesResources.h>
#include <UGKTexturesManager.h>
#include <GameCharacters.h>

using namespace UGK;

CBoometer::CBoometer(UGK::Vector orig, float h, float w, CHAR_TEXTUREID textRef, CTexturesManager * TextureMngr)
{
	Position = orig;

	UpdateAABB(h, w, CHAR_AABB_THICKNESS_DEFAULT);

	IndTexture2D					= textRef;
	Directory.resize(CHARS_MAX_REFERENCES);
	Directory[CHARS_TEXTURES_MNGR_REF] = (CCharacter *) TextureMngr;

	Type			= CHARS_BOOMETER;
	AABBRelPos		= CHAR_AABB_LL;		//AABB origin is set at the lower left corner
}

void CBoometer::Render(float levelRatio)
{
	//Disable all test
	GLboolean switchBlend	= glIsEnabled(GL_BLEND);
	GLboolean switchZ		= glIsEnabled(GL_DEPTH_TEST);

	if(switchBlend)	glDisable(GL_BLEND);
	if(switchZ)		glDisable(GL_DEPTH_TEST);
	
	glPushMatrix();

	float innerWidth = (CharAABB.AABB[CHAR_BBSIZE][XDIM].Value*levelRatio);
	
	//drawing frame of boometer
	glBegin(GL_LINE_STRIP);
		glColor3f( 0.906f, 0.906f, 0.094f);	// golden color
		glVertex2f(Position.v[XDIM], Position.v[YDIM]);//vertex 1
		glVertex2f(Position.v[XDIM] + CharAABB.AABB[CHAR_BBSIZE][XDIM].Value, Position.v[YDIM]);//vertex 2
		glVertex2f(Position.v[XDIM] + CharAABB.AABB[CHAR_BBSIZE][XDIM].Value, Position.v[YDIM] + CharAABB.AABB[CHAR_BBSIZE][YDIM].Value); //vertex 3
		glVertex2f(Position.v[XDIM], Position.v[YDIM] + CharAABB.AABB[CHAR_BBSIZE][YDIM].Value); //vertex 4
		glVertex2f(Position.v[XDIM], Position.v[YDIM]);//vertex 1, closing the frame
	glEnd();

	//drawing inner rectangle of boometer
	if(levelRatio >0)
	{
		((CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF])->Textures[IndTexture2D]->SetTexture();

		glBegin(GL_QUADS);
			glTexCoord2f (0, 0);				glVertex2f(Position.v[XDIM], Position.v[YDIM]);//vértice 1
			glTexCoord2f (levelRatio, 0);		glVertex2f(Position.v[XDIM] + innerWidth, Position.v[YDIM]);//vértice 2
			glTexCoord2f (levelRatio, 0.95);	glVertex2f(Position.v[XDIM] + innerWidth, Position.v[YDIM] + CharAABB.AABB[CHAR_BBSIZE][YDIM].Value); //vértice 3
			glTexCoord2f (0, 0.9);				glVertex2f(Position.v[XDIM], Position.v[YDIM] + CharAABB.AABB[CHAR_BBSIZE][YDIM].Value); //vértice 4
		glEnd();
	}

	glPopMatrix();

	//Enable if they were enabled
	if(switchBlend)	glEnable(GL_BLEND);
	if(switchZ)		glEnable(GL_DEPTH_TEST);
}