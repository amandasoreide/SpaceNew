/** /////////////////////////////////////////////////////////////////////////////////////
//
// Particle
//
// 

	@author Ramon Molla
	@version 2011-10-11
*/

#include <Particle.h>
#include <GameCharacters.h>
#include <UGKTexturesManager.h>

using namespace UGK;

/**
 * Init
 * These values are by default. They have to be overwritten in the initialization phase
 * when reading the default values from the "initialization.html" file
 *
 */
void CParticle::Init (void)
{
	SetDefault();
	PhysicMode	= UGKPHY_LIGHT;
	Directory.clear();
	Timer.resize(CP_MAX_TIMERS);

	///No AABB initialization is required since a particle do not have to collide to anything. It is a consequence of a character collision
}

	///Shows the particle on the screen
void CParticle::Render				(void)
{
	float Size;
	Matrix ModelView;
	CTexturesManager * TM;

	TM = (CTexturesManager *) Directory[CHARS_TEXTURES_MNGR_REF];
	// Jeg lagde if-løkken her
	if (IndTexture2D >= 0) {
		TM->Textures[IndTexture2D]->SetTexture();
	}
	
	if (Life <= 0.0f)
	{
		SetDefault();
		return;
	}
	else
	{
		Revive();
		
		// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
		Size = cos(Life / 2) / 1.6; //1.8

		glPushMatrix();
		glTranslatef (Position.v[XDIM], Position.v[YDIM], Position.v[ZDIM]);

		if (UGKALG_3D == RenderMode)
		{
			//Using Bill boarding
			// get the current UGKALG_ModelView matrix
			glGetFloatv(GL_MODELVIEW_MATRIX, ModelView.GLMatrix());

			ModelView.RotScaleReset();

			// set the ModelView with no rotations and scaling
			glLoadMatrixf(ModelView.GLMatrix());
		}
		glColor4f(1.0f,1.0f,1.0f,Life);
		glBegin(GL_TRIANGLE_STRIP);							// Build Quad From A Triangle Strip
			glTexCoord2f(1,1); glVertex3f(+ Size,+ Size,Position.v[ZDIM]);		// Top Right
			glTexCoord2f(0,1); glVertex3f(- Size,+ Size,Position.v[ZDIM]);		// Bottom Right
			glTexCoord2f(1,0); glVertex3f(+ Size,- Size,Position.v[ZDIM]);		// Top Left
			glTexCoord2f(0,0); glVertex3f(- Size,- Size,Position.v[ZDIM]);		// Bottom Left
		glEnd();											// Done Building Triangle Strip
		glColor4f(1.0f,1.0f,1.0f,1.0f);
		
		glPopMatrix();
	}
}