/* /////////////////////////////////////////////////////////////////////////////////////
//
// Class Particles System
//
	Prefix: CPS_

	@author Ramon Molla
	@version 2011-09
*/

#ifndef CPS_PARTICLESYSTEM
#define CPS_PARTICLESYSTEM

#include <Particle.h>

// # of particles
#define CPS_MAX_PARTICLES 8

class CParticleSystem: public UGK::CCharacter
{
public:
	CParticle		Particle[CPS_MAX_PARTICLES];	/// Particles used in a particle system
	float			Life;							/// Average particle life in seconds
	float			FadingSpeed;					/// Average particle Fading speed
	float			InitialParticleSpeed;			/// Average speed a particle flows away from the particle system mass center

	vector<CParticle*> active_part; ///< Active particles in the system

	//Methods
	void Init ();

	CParticleSystem(){Init();}	//Constructor of the class

	///Shows all the particles on the screen
	void Render				(void);
	///Change the way all the particles are going to be rendered on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE);
};

#endif