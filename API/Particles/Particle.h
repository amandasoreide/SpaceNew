/**
 *	PROJECT - Particle system management
 *
 *	Particle.h
 *	
 *	Copyright (c) 2011. All rights reserved.
 *
 *	@author Ramon Molla <rmolla@dsic.upv.es>
 *	Prefix: CPL_
 *	HISTORY:
 *	@version 1.0 (Dec, 2011)
 */

#ifndef CPL_PARTICLE
#define CPL_PARTICLE

#include <UGKCharacter.h>

/** \typedef CP_EXTRA_TIMERS
*	Types of different local timing managed by any ship
*/
typedef enum {
	CP_UPD_PERIOD,
	CP_RND_PERIOD,
	CP_MAX_TIMERS
} CP_EXTRA_TIMERS;

/**
 * CParticle
 * This class supports only a single particle that belongs to a given particle system
 *
 * @todo Finish to implement basic functionality
 */
class CParticle: public UGK::CCharacter
{
public:
	/// Particle Life
	float	Life;
	/// Fading Speed
	float	FadingSpeed;
	/// Counter to know when to reduce acceleration
	float	counteri;

	//Methods
	void Init (void);

	CParticle(){Init(); };	//Constructor of the class

	///Shows the particle on the screen
	void		Render				(void);
};
#endif