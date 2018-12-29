/* /////////////////////////////////////////////////////////////////////////////////////
//
// Class Particles System
//
	Class prefix: CPS_

	@author Ramon Molla
	@version 2011-07
*/

#include <ParticleSystem.h>
#include <GameCharacters.h>

extern CTexturesManager TexturesManager;

void CParticleSystem::Init ()
{
	/// PARTICLES INITIALIZATION
	float V, Angle;

	FadingSpeed				= 0.01f;		/// Fading speed by default
	Life					= 0.5f;			/// Average life for every particle
	InitialParticleSpeed	= 1.4f;
	PhysicMode				= UGKPHY_LIGHT;	/// Visible but not touchable
	///No AABB initialization is required since explosions do not have to collide to anything. They are a consequence of a character collision

	Timer.resize(CP_MAX_TIMERS);

	Directory.resize(CHARS_MAX_REFERENCES);
	Directory[CHARS_TEXTURES_MNGR_REF] = (CCharacter*)&TexturesManager;//Neccesary to access the textures used for every character to be rendered

	for (int loop=0;loop<CPS_MAX_PARTICLES;loop++)				/// Initializes all the particles
	{
		Particle[loop].Life			= Life + float(rand()%10000)/10000.0f;			/// Give All The Particles Full Life in the range [0.5,1.5]
		Particle[loop].FadingSpeed	= FadingSpeed + float(rand()%10000)*0.0001f;	/// Random Fading Speed

		Particle[loop].Directory.resize(CHARS_MAX_REFERENCES);
		Particle[loop].Directory[CHARS_TEXTURES_MNGR_REF] = Directory[CHARS_TEXTURES_MNGR_REF];	//Tipically CTM_PARTICLE2
		
		V = float(rand()%4+2) * InitialParticleSpeed;			/// Speed of the particle 
		Angle = float(rand()%360);								/// Angle of the particle

		Particle[loop].counteri	= 0;		

		Particle[loop].MoveTo(0.0f, 0.0f, 0.0f);				/// Set the initial position of every particle at the current position of the particle system

		Particle[loop].Speed.v[XDIM] = sin(Angle) * V;				/// Set X velocity
		Particle[loop].Speed.v[YDIM] = cos(Angle) * V;				/// Set Y velocity
		Particle[loop].Speed.v[ZDIM] = float(((rand()%10)-5)*0.1) * V;	/// Set Z velocity
		///No AABB initialization is required since explosions do not have to collide to anything. They are a consequence of a character collision
	}
};

///Shows all the particles on the screen
void CParticleSystem::Render				(void)
{
	unsigned int loop;

	for (loop=0; loop<CPS_MAX_PARTICLES; loop++)				// Loop Through All The Particles
		Particle[loop].Render();
}

///Change the way all the particles are going to be rendered on the screen
void CParticleSystem::ChangeRenderMode	(CHAR_RENDER_MODE Mode)
{
	unsigned int loop;

	RenderMode = Mode;

	for (loop=0; loop<CPS_MAX_PARTICLES; loop++)				// Loop Through All The Particles
		Particle[loop].ChangeRenderMode(Mode);
}

