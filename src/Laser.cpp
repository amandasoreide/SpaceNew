/**

* Laser class

* These values are by default. They have to be overwritten in the initializatio phase
* when reading the default values from the "initialization.html" file

*	@author Ramon Molla
*	@version 2011-11
*/

#include <Player.h>  /// Call the library functions defined in the Laser
#include <Laser.h>
#include <GameCharacters.h>
#include <SITexturesResources.h>
#include <GlobalDefs.h>
#include <SIMessage.h>

/**   
   * Constructor for Laser
*/
CLaser::CLaser()
{
	SetLocalTimers(CL_MAX_TIMERS);

	Mesh = NULL;

#ifdef CHAR_USE_AABB
	InitializeAABB();
#endif

	Init();
};		//Constructor

/**   
	* @fn bool CLaser::Init ()
	* Start the Laser
    * It starts after the laser has been taken in the course of the game
    * Also starts life variables, number of fire, explosion
    * The position and speed of the laser with respect to the ship
*/
bool CLaser::Init ()
{
	Type			=	CHARS_LASER;		//se asigna el tipo de bonus que en este caso es el laser
	Health			=	CL_MAX_HEALTH;		//Cantidad de vida del arma
	Hit_duration	=	CL_MAX_MAX_HIT_DUR; //la cantidad maxima de disparos que se puede realizar con el laser
	
	SetDefault();							//The very first time, when created at the beginning of the game, this device is not available
	Passing2D23D	=	false;
	Billboard		=	false;

	Timer[CL_RND_PERIOD].ms2Ticks(SIGLBD_MAX_RENDERTIME);
	Explosion.Init(SIGLBD_MAX_RENDERTIME);

	RenderMode				= CHAR_3D;
#ifndef CHAR_USE_AABB
	UpdateAABB(0.20, 0.25f, 0.0f);			///Update by default the AABB relative to local coordinates
#endif
	Position.Set(0.0f, 0.0f, SIGLBD_DEFAULT_Z_POSITION);	//Implicitly updates by default the AABB relative to local coordinates and collision detector also

	yi				=	0.00;  // movimiento de la nave en la coordenada y
	yi_speed		=	0.01f; // velocidad de la nave en la coordenada y
	yi_cos			=	0;     // variable coseno de nave en la coordenada y
	zi				=	0.00;  // flotamiento de la nave coordenada z para el modo 3D
	zi_speed		=	0.01f; // velocidad de la nave coordenada z para el modo 3D

	Immortal = false;

	return true;
}

/**   
	* @fn AI_Explode
	* Artificial Intelligence Explosion of Laser
    * Explosion starts class Laser type
    * After Killing Laser Artificial Intelligence
*/
void CLaser::AI_Explode (void)
{
	Explosion.Init(Timer[CL_RND_PERIOD].GetAlarmPeriodms());
	Explosion.Activate();

	RTDESK_CMsg *Msg = GetMsgToFill(UMSG_MSG_BASIC_TYPE);
	SendMsg(Msg, BonusManager, RTDESKT_INMEDIATELY);

	AI_Die(); //se mata a la IA del Laser
}	

/**   
	* @fn Collided
	* Shock
    * We define this function for a collision when a Character
    * @ Param CCharacter CollidedChar Shock represents the type Character
    * Depending on the type of shock that has produced 
	* (or firing of the enemy ship) is given a specific sound and global variables changes and game states
*/
void CLaser::Collided (CCharacter *CollidedChar)
{

	if (CHARS_COLLISION_TABLE[CHARS_LASER][CollidedChar->Type])
		switch (CollidedChar->Type) //Crash type
		{
		case CHARS_SHOOT:
			switch (((CShoot*)CollidedChar)->SubType) //Enemy missil impact
			{
			case CSH_SHIP:				///<Shoot sent by a enemy ship. This kind of shoot has to be always after all player's shootings
			case CSH_SUPPLY_SHIP:		///<Shoot sent by a enemy Supply Ship. This kind of shoot has to be always after all player's shootings
			case CSH_CIRCLE_SHIP:		///<Shoot sent by a enemy Circle Ship. This kind of shoot has to be always after all player's shootings
				break;
			default:
				return;
			}
		case CHARS_SHIP: //Ship crash
			if (Alive() && !Immortal)
			{
	#ifdef DEBUG_MORTAL
				// Sound effect for the laser explosion
				Sound[CPL_EXPLOSION_SND]->Play(UGKSND_VOLUME_80);
				if (Passing2D23D)	return;		// Laser is protected by the Bonus upgrade explosion
				AI_Explode();
	#endif
			}
			break;
		}
}

/**   
	* @fn Render
    * It defines all the actions to do when you get or lose a laser weapon
    * Also as changing 2D, 3D Laser with the Character or ship in the game
*/
void CLaser::Render (void)
{	
	if (!Alive())
		return;

	///At this version, lasers can only be displayed when in 3D mode
	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		return;
	case CHAR_2D:
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		if (Alive())
		{
			Mesh->modelo.pos.v = Position.v;
			Mesh->modelo.rot.x =  -270.0f;
			Mesh->modelo.rot.z =  Player->Mesh->modelo.rot.z;
			//Mesh->modelo.scale = 0.15f;
			
			Mesh->modelo.Draw();
		}
		break;
	default: return;
	}

	if (Explosion.Alive())
		Explosion.Render();
	else if (Explosion.Active())
		{
			RTDESK_CMsg *Msg = GetMsgToFill(UMSG_MSG_BASIC_TYPE);
			SendMsg(Msg, Directory[CHARS_GAME_REF], RTDESKT_INMEDIATELY);
			SetDefault();
		}
}

/**   
	* @fn RenderExplosion
	* Define all actions that must be made when the laser operates
    * Modification, reconstruction of Texture, particles of a Character in 2D or 3D
    * @return the Explosion of a character by its particles
*/
void CLaser::RenderExplosion(void)
{
	Matrix ModelView;
	float V, Angle;
	CParticle * Particle;


	HRT_Timems RenderPeriod = Timer[CL_RND_PERIOD].GetAlarmPeriodms();

	((CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF])->Textures[CTM_PARTICLE2]->SetTexture();

	if (Explosion.Alive() && UGKALG_3D == RenderMode)
	{
		Explosion.SetDefault();
		for (unsigned int loop=0; loop<Explosion.active_part.size(); loop++)				// Loop Through All The Particles
		{
			//Check for particles alive. If no one alive, explosion is finished
			if (Explosion.active_part[loop]->Health > 0.0f)
			{
				Explosion.Revive();

				float x = Explosion.active_part[loop]->Position.v[XDIM];		// Grab Our Particle X Position
				float y = Explosion.active_part[loop]->Position.v[YDIM];		// Grab Our Particle Y Position
				//float z = Explosion.active_part[loop]->Position.v[ZDIM];		// Grab Our Particle Z Position
				float z = SIGLBD_DEFAULT_Z_POSITION;

				// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Health
				glColor4f(1.0f,1.0f,1.0f,Explosion.active_part[loop]->Health);

				float size = cos(Explosion.active_part[loop]->Health / 2) / 1.6; //1.8

				if (UGKALG_3D == RenderMode)
				{
					size *= 1.2;
					// BILLBOARDING (particles are perpendicular to the camera)
					// --------------------------------------------------------------
					if (Billboard)	//&& !viewport2
					{
						glPushMatrix();
						glTranslatef ( x, y, 0 );

						// get the current UGKALG_ModelView matrix
						//glGetFloatv(GL_MODELVIEW_MATRIX , ModelView.GLMatrix());

						ModelView.NeutralModelView();

						glBegin(GL_TRIANGLE_STRIP);							// Build Quad From A Triangle Strip
						glTexCoord2f(1,1); glVertex3f(+ size,+ size,z);		// Top Right
						glTexCoord2f(0,1); glVertex3f(- size,+ size,z);		// Bottom Right
						glTexCoord2f(1,0); glVertex3f(+ size,- size,z);		// Top Left
						glTexCoord2f(0,0); glVertex3f(- size,- size,z);		// Bottom Left
						glEnd();											// Done Building Triangle Strip

						glPopMatrix();
					}
					else // No billboarding (explosion ship)
					{
						glBegin(GL_TRIANGLE_STRIP);							// Build Quad From A Triangle Strip
						glTexCoord2f(1,1); glVertex3f(x + size,y + size,z);	// Top Right
						glTexCoord2f(0,1); glVertex3f(x - size,y + size,z);	// Bottom Right
						glTexCoord2f(1,0); glVertex3f(x + size,y - size,z);	// Top Left
						glTexCoord2f(0,0); glVertex3f(x - size,y - size,z);	// Bottom Left
						glEnd();											// Done Building Triangle Strip
					}
				}
				else // game 2D
				{
					glBegin(GL_TRIANGLE_STRIP);								// Build Quad From A Triangle Strip
					glTexCoord2f(1,1); glVertex3f(x + size,y + size,z);		// Top Right
					glTexCoord2f(0,1); glVertex3f(x - size,y + size,z);		// Bottom Right
					glTexCoord2f(1,0); glVertex3f(x + size,y - size,z);		// Top Left
					glTexCoord2f(0,0); glVertex3f(x - size,y - size,z);		// Bottom Left
					glEnd();												// Done Building Triangle Strip
				}

/*				if (viewport2) // For the Mini-screen, we don't recalculate the movements
					continue;
*/
				// Move the particles
				Particle = Explosion.active_part[loop];
				Particle->MoveRelTo(Particle->Speed * (0.004 * RenderPeriod));// Move On The Y Axis By Y Speed

				// Slow down the particles
				Particle->counteri += (3 * RenderPeriod) / 40;
				if (Particle->counteri > 0.99)
				{
					if (UGKALG_3D == RenderMode)
						Particle->Speed *= 0.90f;
					else
						Particle->Speed *= 0.88f;
					Particle->counteri = 0;
				}

				// Particle life
				Particle->Health -= Explosion.active_part[loop]->FadingSpeed * RenderPeriod;// Reduce Particles Health By 'Fade'

				// Creation of new Particle, if Particle is burned out
				if (Explosion.active_part[loop]->Health < 0.0f && Explosion.Health > 0.0f)
				{
					Particle->Health		= 1.0f;				// Give It New Health
					Particle->FadingSpeed	= float(rand()%100)/10000 + 0.015f;// Random Fade Value
					Particle->Position		= Position;
				}
				V = (float((rand()%3)+2));								// Velocity of the particles
				if (UGKALG_3D == RenderMode)
					V *= 1.5;
				Angle = float(rand()%360);								// Angle of the particles

				Particle->Speed.v[XDIM] = sin(Angle) * V;		// x acceleration
				Particle->Speed.v[YDIM] = cos(Angle) * V;		// y acceleration
				//Explosion.active_part[loop]->zi = ((rand()%10)-5)/5;
				// Explosion life duration diminishes
				Explosion.Health-= ((3 * RenderPeriod) /40);
			}	//If Particle Health > 0.0f
		}	//For every Particle
	}	//Explosion.Alive
}
