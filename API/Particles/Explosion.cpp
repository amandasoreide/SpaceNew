/*/////////////////////////////////////////////////////////////////////////////////////
//
// Explosion declarations
//

	@author Ramon Molla
	@version 2017-05-11
*/

#include <ExecutionMode.h>
#include <GlobalTiming.h>
#include <Explosion.h>

using namespace UGK;

/**
	* Constructor for the explosion of a character
    * Starts the life of a Character
    * This Explosion is for any given time.
    * @param CChraracter C: is an input to the function
    * @param float Time lag to avoid too many updates per second
    * It takes a Loop to recognize the speed and position of C and assign to the particle velocity
*/
void CExplosion::Init(CCharacter *C, HRT_Timems RP)
{
	Init(RP);
	Health 	 = float((rand()%100)+ CEX_MAX_HEALTH); // particle life
	
	Position = C->Position;
	Speed	 = C->Speed;
	for (unsigned int loop=0;loop<active_part.size();loop++)	/// Initializes all the particles
	{
		//Speed and position for all the particles in the particle system is the same as the character father (particle system) they belong to
		active_part[loop]->Speed	= Speed;
		active_part[loop]->Position	= Position;
	}
}

/**
	* Constructor for the Explosion
    * Default parameters starts disabled
    * This function Explosion Starts a Character
*/
void CExplosion::Init(HRT_Timems RP)
{
	SetDefault();
	Health		= -1.0f;
	PhysicMode	= UGKPHY_LIGHT;	//Visible but not touchable
	Timer.resize(CP_MAX_TIMERS);
	UpdateSF(TimerManager.GetSF());
	Timer[CP_RND_PERIOD].SetAlarm(RP);

	///No AABB initialization is required since explosions do not have to collide to anything. They are a consequence of a character collision
}

/**
	* Function for updating when is a Explosion
    * This function depends on the living state of the particle if true
    * @param float V: represents particle velocity for the new Particle
    * @param float Angle: represents the angle for the new particle Particle
    * It traces the life of particles and if they are alive is greater than 0.0f
    * If the particle is alive move the position of the particle in the xy plane, and then slow down depending on RenderMode Particle.counteri 2D
    * After that life is reduced to the particle and if life is a burning creates new particle
    * After the initial V and 2D RenderMode depending Angle for partiuclas
    * And finally reduces the life of Character
    * @The function recalculates the particles of a Character and is destroying the once
*/
void CExplosion::Update(void)
{
	float V, Angle;
	double RenderPeriod;
	unsigned int loop;
	Vector DeltaPos;

#if defined(XM_UPD_TIME_DISC) || defined(XM_UPD_TIME_CONT)
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
#endif

	if (Alive())
	{
		SetDefault();
		for (loop=0; loop<active_part.size(); loop++)				// Loop Through All The Particles
		{
			//Check for particles alive. If no one is alive, explosion is finished
			if (active_part[loop]->Health > 0.0f)
			{
				Revive();
				RenderPeriod = Timer[CP_RND_PERIOD].GetAlarmPeriodms();
				V = 0.004*0.075*RenderPeriod;
				// Move the particles
				DeltaPos = active_part[loop]->Speed * V;
				active_part[loop]->MoveRelTo(DeltaPos);// Move On The X Axis By X Speed

				// Slow down the particles
				active_part[loop]->counteri += (3.0f * RenderPeriod) / 40.0f;
				if (active_part[loop]->counteri > 0.99)
				{
					if (CHAR_2D == RenderMode)
						active_part[loop]->Speed *= 0.88f;//.90
					else
						active_part[loop]->Speed *= 0.90f;//.94
					active_part[loop]->counteri = 0;
				}

				// Particle life
				active_part[loop]->Health -= active_part[loop]->FadingSpeed * 0.075f * RenderPeriod;// Reduce Particles Health By 'Fade'

				// Creation of new Particle, if Particle is burned out
				if (active_part[loop]->Health < 0.0f && Health > 0.0f)
				{
					active_part[loop]->Health				= 1.0f;				// Give It New Health
					active_part[loop]->FadingSpeed		= float(rand()%100)/10000 + 0.015f;// Random Fade Value
					active_part[loop]->Position	= Position;	// Center On X Axis
				}
				V = (float((rand()%3)+2));								// Velocity of the particles
				if (CHAR_2D != RenderMode)
					V *= 1.5;
				Angle = float(rand()%360);								// Angle of the particles

				active_part[loop]->Speed.v[XDIM] = sin(Angle) * V;		// x acceleration
				active_part[loop]->Speed.v[YDIM] = cos(Angle) * V;		// y acceleration
				//active_part[loop]->zi = ((rand()%10)-5)/5;
				// Explosion life duration diminishes
				Health -= ((3.0f * RenderPeriod) / 40.0f);
			}	//If Particle Health > 0.0f
		}	//For every Particle
	}	//Alive
#if defined(XM_UPD_TIME_DISC) || defined(XM_UPD_TIME_CONT)
	TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
#endif

}
