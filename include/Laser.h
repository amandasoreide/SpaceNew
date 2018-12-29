/**	Definition of the class Laser
	Class prefix CL_

	@author Ramon Molla
	@version 2011-09
*/

#ifndef CL_CLASS_LASER /// Macro laser weapon is erased
#define CL_CLASS_LASER /// Macro laser weapon is defined

#include <ExplosiveChar.h> /// Library to define the explosion of the laser weapon
#include <Shoot.h>

#define CL_MAX_HEALTH			 20	///Maximun amount of health of a given Laser by default
#define CL_MAX_MAX_HIT_DUR		500	///Maximun amount of time when a given Laser is hit

/** \typedef CL_EXTRA_TIMERS

*	Types of different local timing managed by any circleship
*/
typedef enum {
	CL_UPD_PERIOD,
	CL_RND_PERIOD,
	CL_MAX_TIMERS
} CL_EXTRA_TIMERS;

/**
	@class CLaser: public CCharacter

	* Laser class is defined with derivation or inheritance of the class CCharacter
    * Here we define the start and death of the movement of the laser IA
    * You show how the laser and the explosion of the same on the screen depending on the coordinates
	@author Ramon Molla
	@version 2011-09
*/
class CLaser: public CExplosiveChar
{
	//Attributes
public:
	float		yi,				// cuando se mueve la nave en la coordenada y
		yi_speed,		// la velocidad de la nave en la coordenada y
		yi_cos,			// variable coseno de nave en la coordenada y
		zi,				// el flotamiento de la nave coordenada z para el modo 3D
		zi_speed;		// la velocidad de la nave coordenada z para el modo 3D

	CSH_SHOOT_TYPE	ShootType;	///<TYPE of shooting power in a given moment. It depends on the bonus got by the player during the playing

	//ACHTUNG: Cambiar estAS REFERENCIAS por un Directory y tipificar. Normalizar como el resto de objetos del sistema
	CCharacter	*Player,			///< Player to where the laser is linked to
				*BonusManager;

	bool		Passing2D23D,	///< Flag that prevents any destruction of the player, lasers,... while moving from 2D rendering mode to 3D one
				Billboard;		///< Application windows use Billboards 

	//Methods
	bool Init ();	//Used when all the values are initialized by default
					//when reading the global initialization game file
					//Returns true if the initialization of the attribute has been correctly done

	CLaser();		//Constructor

	void AI_Explode (void);
	
	//Physics
	void Collided (CCharacter *CollidedChar);

	///Shows the laser on the screen
	void Render				(void);
	void RenderExplosion	(void);
};

#endif
