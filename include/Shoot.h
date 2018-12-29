/**	
*	@file Shoot.h
*	Definition of the class Shoot

*	Prefix: CSH_

*	@author Ramon Molla
*	@version 2016-06
*/

#ifndef CSH_SHOOT
#define CSH_SHOOT

#include <UGKSoundsManager.h>
#include <ExplosiveChar.h>
#include <UGKTexturesManager.h>
#include <GlobalTiming.h>

/**
*	\typedef CSH_SHOOT_TYPE 
*	Different types of shoots that can be active at any moment while playing the game
*/

#define UPDATE_SHOOT 40 //ms of time in which shoots should be updated
#define CSH_DEFAULT_SPEED	0.002F


/** \typedef CSH_EXTRA_TIMERS
*	Types of different local timing managed by any ship
*/
typedef enum {
	CSH_UPD_PERIOD,
	CSH_MAX_TIMERS
} CSH_EXTRA_TIMERS;

//Type of different ammounition available
typedef enum {
	CSH_DEFAULT,			///<For management purpouses only
	CSH_PLAYER2D,				///<Shoot sent by the player by default in 2D mode
	CSH_PLAYER3D,			///<Shoot sent by the player in 3D mode
	CSH_PLAYER3D_CHEVRON,	///<Shoot sent by the player
	CSH_AUX_LASER,			///<Shoot sent by the right and left laser auxiliar player ships
	CSH_AUX_LASER_CHEVRON,	///<Shoot sent by the right and left laser auxiliar player ships when the player has a Chevron shooting
	CSH_SHIP,				///<Shoot sent by a enemy ship. This kind of shoot has to be always after all player's shootings
	CSH_SUPPLY_SHIP,		///<Shoot sent by a enemy Supply Ship. This kind of shoot has to be always after all player's shootings
	CSH_CIRCLE_SHIP,		///<Shoot sent by a enemy Circle Ship. This kind of shoot has to be always after all player's shootings
	CSH_BULLET,				///
	CSH_MAX_SHOOTS_TYPES	///<Not a real shoot. This is for accounting purpouses only
} CSH_SHOOT_TYPE;

//Type of different sounds a shoot can make when shot or impacted
typedef enum {
	CSH_EXPLOSION_SND,		///<For general explosion
	CSH_TOUCH_SND,			///<Every time a shoot reaches a target
	CSH_MAX_SND	///<Not a real shoot. This is for accounting purpouses only
} CSH_SHOOT_SOUND_TYPE;

using namespace UGK;

/**
*	\class CShoot
*	Supports every shoot sent by any character in the game
*/
class CShoot: public CExplosiveChar
{
public:
	//RT-DESK
	double		msUpdMsg;
	RTDESK_TIME ticksUpdMsg;						//RTDESK Message Time

	RTDESK_CMsg *msg;
	RTDESK_CMsg *msgUpd;							//RTDESK Update Message

	//Methods
	void Init (void);
	void InitTransforms(CHAR_RENDER_MODE Mode);

	void SubtypeChange(CSH_SHOOT_TYPE	SubType);	///<Any subtype change has to update AABB and size

	///Character AI virtual methods rewritten
	inline void AI_Die (void) {	
		Collisionable(UGKPHY_NON_COLLISIONABLE);
		Init();
		Deactivate();
	}

	void AI_Explode (void);

	//Physics
	void Collided (CCharacter *CollidedChar);

	void Render				(void);	///<Shows the shoot on the screen
	void Update				(void);
	void DiscreteUpdate		(void);

	void ReceiveMessage(RTDESK_CMsg *pMsg);

	CShoot ();
	~CShoot();
};

#endif