/**
*	@file Reactor.cpp
// Reactor class
//
// These values are by default. They have to be overwritten in the initialization phase
// when reading the default values from the "initialization.html" file

	@Prephix: CR_
	@author Ramon Molla
	@version 2011-09-11
*/

#include <Player.h>
#include <UGKMeshManager.h>

#include <SITexturesResources.h>
#include <GameCharacters.h>

#define CR_ALPHA_MIN 0.8
#define CR_ALPHA_MAX 1.0

extern CMeshesManager	MeshesManager;

CReactor::CReactor()
{
	alpha		= 0.83f;
	alpha_down	= true;

	Size			= 0.00;
	
	PhysicMode	= UGKPHY_LIGHT;	//Visible and untouchable. No AABB management
	Type		= CHARS_REACTOR;

	UpdateAABB(0.3,0.3,0.0);
	ResetTransformations();
	SetLocalTimers		(CR_MAX_TIMERS);
}

void CReactor::RenderLaser(unsigned int L)
{
	float UGKALG_ModelView[16];
	CTexturesManager * TextMngr = (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF];
	unsigned int i, k, l;

	TextMngr->Textures[CTM_PARTICLE]->SetTexture();

	glColor4f(0.120f, 0.340f, 0.953f, alpha);					// bleu
	glPushMatrix();

	glTranslatef(Players[CurrentPlayer]->Laser[L]->Position.v[XDIM],
		Players[CurrentPlayer]->Laser[L]->Position.v[YDIM],
		Players[CurrentPlayer]->Laser[L]->Position.v[ZDIM]);

	glRotatef(Players[CurrentPlayer]->Laser[L]->Mesh->modelo.rot.x, 1.0f, 0.0f, 0.0f);
	glRotatef(Players[CurrentPlayer]->Laser[L]->Mesh->modelo.rot.y, 0.0f, 1.0f, 0.0f);
	glRotatef(Players[CurrentPlayer]->Laser[L]->Mesh->modelo.rot.z, 0.0f, 0.0f, 1.0f);

	glTranslatef(0.0, 0.03, 0.12);

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX, UGKALG_ModelView);

	// undo all rotations (beware all scaling is lost as well)
	for (k = 0; k<3; k++)
	for (l = 0; l<3; l++) {
		if (k == l)
			UGKALG_ModelView[k * 4 + l] = 1.0;
		else
			UGKALG_ModelView[k * 4 + l] = 0.0;
	}

	// set the modelview with no rotations and scaling
	glLoadMatrixf(UGKALG_ModelView);

	for (i = 0; i <= 2; i++)
	{
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1);	glVertex2f(-.30, +.30);
		glTexCoord2f(0, 0);	glVertex2f(-.30, -.30);
		glTexCoord2f(1, 0);	glVertex2f(+.30, -.30);
		glTexCoord2f(1, 1);	glVertex2f(+.30, +.30);
		glEnd();
	}

	TextMngr->Textures[CTM_PARTICLE4]->SetTexture();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);	glVertex2f(-.60 - Size, +.60 + Size);
	glTexCoord2f(0, 0);	glVertex2f(-.60 - Size, -.60 - Size);
	glTexCoord2f(1, 0);	glVertex2f(+.60 + Size, -.60 - Size);
	glTexCoord2f(1, 1);	glVertex2f(+.60 + Size, +.60 + Size);
	glEnd();

	glPopMatrix();
}

void CReactor::Render ()
{
	float UGKALG_ModelView [16];
	unsigned int i,k,l;
	CTexturesManager * TextMngr = (CTexturesManager *) Directory[CHARS_TEXTURES_MNGR_REF];
	HRT_Timems RenderPeriod = Timer[CR_RND_PERIOD].GetAlarmPeriodms();

	// Pas de lumière sur les réacteurs
	glDisable	(SIGLB_PLAYER_LIGHT);
	glDisable	(GL_LIGHTING);

	// REACTEURS DU PLAYER
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glFrontFace(GL_CCW);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	TextMngr->Textures[CTM_PARTICLE]->SetTexture();
	
	// Reactors alpha
	if (alpha_down)
		if (alpha > CR_ALPHA_MIN)
			 alpha -= 0.005*RenderPeriod;
		else alpha_down = false;	//Grow up alpha. Minimum value reached
	else if (alpha < CR_ALPHA_MAX)
			  alpha += 0.005*RenderPeriod;
		 else alpha_down = true;	//Grow down alpha. Maximum value reached

	glColor4f (0.953f, 0.340f, 0.120f, alpha);					// red

	glPushMatrix();
		Transform();
		//billboarding
		glPushMatrix();
			
			// get the current UGKALG_ModelView matrix
			// get the current UGKALG_ModelView matrix
			glGetFloatv(GL_MODELVIEW_MATRIX, UGKALG_ModelView);

			// undo all rotations (beware all scaling is lost as well). Only translation is maintained
			for (k = 0; k<3; k++)
				for (l = 0; l<3; l++)
					if (k == l)
						UGKALG_ModelView[k * 4 + l] = 1.0;
					else
						UGKALG_ModelView[k * 4 + l] = 0.0;

			// set the UGKALG_ModelView with no rotations and scaling
			glLoadMatrixf(UGKALG_ModelView);

			for (i=0; i<=2; i++)
				{
					glBegin (GL_QUADS);
						glTexCoord2f (0,1);	glVertex2f ( - .30+Size, +.30-Size);
						glTexCoord2f (0,0);	glVertex2f ( - .30+Size, -.30+Size);
						glTexCoord2f (1,0);	glVertex2f ( + .30-Size, -.30+Size);
						glTexCoord2f (1,1);	glVertex2f ( + .30-Size, +.30-Size);
					glEnd ();
				}
		
			TextMngr->Textures[CTM_PARTICLE4]->SetTexture();

			glBegin (GL_QUADS);
				glTexCoord2f (0,1);	glVertex2f ( - .70, +.70);
				glTexCoord2f (0,0);	glVertex2f ( - .70, -.70);
				glTexCoord2f (1,0);	glVertex2f ( + .70, -.70);
				glTexCoord2f (1,1);	glVertex2f ( + .70, +.70);
			glEnd ();

		glPopMatrix();
	glPopMatrix();

	// Enabling everything again, as the beginning
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_BLEND);

	glEnable	(SIGLB_PLAYER_LIGHT);
	glEnable	(GL_LIGHTING);
}
