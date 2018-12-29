/*	Definition of the class Explosion

	Prefix: CE_

	@author Ramon Molla
	@version 2011-11
*/

#ifndef CE_EXPLOSION   //Se borra la macro explocion
#define CE_EXPLOSION   //Se define la macro explocion

#include <ParticleSystem.h>
#include <GlobalDefs.h>

#define CEX_MAX_HEALTH			500 //Maximun amount of health of a given Supply Ship by default

using namespace UGK;

/**   
   * It defines the types of explosions that exist in the game
   * @param enum CE_EXPLOSION_TYPE which lists the types of explosion
*/
typedef enum
{
	CE_NO_EXPLOSION,		 ///< No explosion is allowed
	CE_SHIP_EXPLOSION,		 ///< Explosion of enemies
	CE_CIRCLESHIP_EXPLOSION, ///< Explosion of the Ship Supply
	CE_SUPPLYSHIP_EXPLOSION, ///< Explosion of the Ship Supply
	CE_PLAYER_EXPLOSION,     ///< Explosion of the character or ship
	CE_LASER_EXPLOSION,      ///< Laser Gun Explosion of the Ship
	CD_MAX_EXPLOSION		 ///< For management purpouses only
} CE_EXPLOSION_TYPE;

/**
	* Explotion class is defined with derivation or inheritance of the class CParticleSystem
    * Where is the constructor and update
    * @param subtype CE_EXPLOSION_TYPE: predefined variable for the types of explosion
*/
class CExplosion: public CParticleSystem
{

public:
	//Methods
	void Init(HRT_Timems RenderPeriod);
	void Init (CCharacter *C, HRT_Timems RenderPeriod);

	CExplosion(){ Init(SIGLBD_MAX_RENDERTIME); }		//Constructor of the class at 60Hz by default

	void Update		(void);
};

#endif
