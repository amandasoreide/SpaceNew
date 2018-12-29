/**	Definition of the class Player
	Class prefix CR_

	@author Ramon Molla
	@version 2011-09-11
*/

#ifndef CR_CLASS_REACTOR
#define CR_CLASS_REACTOR

#include <ExplosiveChar.h>

#define CR_MAX_HEALTH			 20	///Maximun amount of health of the player by default
#define CR_MAX_MAX_HIT_DUR		500	///Maximun amount of time when the player is hit

using namespace UGK;


/** \typedef CR_EXTRA_TIMERS

*	Types of different local timing managed by any circleship
*/
typedef enum {
	CR_UPD_PERIOD,
	CR_RND_PERIOD,
	CR_MAX_TIMERS
} CR_EXTRA_TIMERS;

class CReactor: public CExplosiveChar
{
	//Attributes
	float	alpha;
	bool	alpha_down;
	float	Size;

public:
	UGK::Vector	Shift;	

	//Methods
	CReactor();		//Constructor

	///Shows the reactor on the screen
	void Render			();
	void RenderLaser	(unsigned int);
};

#endif