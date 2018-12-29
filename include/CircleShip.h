/**	Definition of the class CircleShip

	Prefix: CCS_
	@author Héctor Martínez Cebrián
	@version 2013-04-29
*/


#ifndef CCS_CIRCLESHIP
#define CCS_CIRCLESHIP

#include <ExplosiveChar.h>
#include <SIMessage.h>

#define CCS_MAX_HEALTH			 25	//Maximun amount of health of a given Ship by default
#define CCS_MAX_MAX_HIT_DUR		500	//Maximun amount of time when a given Ship is hit
#define CCS_MAX_EXPLOSION_LIFE 10000	//Time in ms the ship may be exploding

#define CCS_SHOOT_SPEED		  0.017f

typedef enum {
	CCS_UPD_PERIOD,
	CCS_RND_PERIOD,
	CCS_HEALTH_INC_TMR,
	CCS_MAX_TIMERS
} CCS_EXTRA_TIMERS;

typedef enum {
	CCS_LEFTTYPE,
	CCS_RIGHTTYPE,
	CCS_BOTHTYPE,
	CCS_MAXTYPE
} CCS_SHIP_TYPE;

typedef enum {
	CCS_UNBORN=0,	///For management purpouses only
	CCS_BORN,		///The character is just born but it is not still operative (living)
	CCS_LIVING,		///Everything this character may do while it is alive 
	CCS_BURSTING,	///The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CCS_DEAD,		///The character is no operative. Reached after dying
	CCS_MAXSTATE		///For management purpouses only
} CCS_CIRCLESHIP_STATE;

typedef enum {
	CCS_DEFAULT=0,				///For management purpouses only
	CCS_BORNING,				///The character is just borning. Passing from UNBORN to BORN states
	CCS_GETTING_ALIVE,			///Passing from BORN to LIVING states
	CCS_MOVING,					///Remaining in LIVING state while moving the Circle Ship
	CCS_DISPLAYING,				///Remaining in LIVING state while displaying the Circle Ship
	CCS_HEALTHING,				///Remaining in LIVING state while increasing the health of the Circle Ship
	CCS_BURST,					///The character has been touched so many times that its life has gone negative. So it has to burst. Passing from LIVING to DYING states
	CCS_DIE,					///The character is no operative. Reached after dying
	CCS_FINISHED,				///The character is no operative. Reached after the game has finished: player has lost or win the level or game is exited
	CCS_RESURRECTING,			///Passing to Unborn state
	CCS_MAXTRANSITION			///For management purpouses only
} CCS_CIRCLESHIP_TRANSITION;

class CCircleShip: public CExplosiveChar
{
//Attributes
public:
	///When the ships loses its life, it burst. This is the particle system to perform the explosion
	CExplosion	Explosion;

	//RT-DESK
	RTDESK_TIME timeRTdeskMsg;						//Tiempo Mensaje RTDESK
	RTDESK_CMsg *msg;								//RTDESK Message Time
	cMsgNavy	*msgUpd;

	//Radio
	float Radius;
	//Center
	Vector Center;
	//Degrees
	float Degrees;
	//Velocity
	float Velocity;
	bool ChgDir;


	//Methods
	void Init			();
	void InitTransforms	();

	///Increasing health while time passes by
	void AI_Healthing	();
	///Moving the Supply Ship
	void AI_Move		();
	///The supply ship is going to burst before being dead
	void AI_Die			();
	///What to do when the supply ship is dead
	void AI_Death		();
	bool OutEvent		(CCS_CIRCLESHIP_TRANSITION EventName);
	
	//Physics
	void Collided		(CCharacter *CollidedChar);

	///Shows the supplyship on the screen
	void Render			(void);
	///Change the way the supplyship is going to be rendered on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE Mode);

	void Update			(void);
	void DiscreteUpdate	(void);

	void ReceiveMessage	(RTDESK_CMsg *pMsg);

	///Constructor of the class
	CCircleShip			();
};

//External methods to use with the class internal FSM
///Called everytime a time slot happens and its health has to be increased
void *CCS_FSM_Healthing	(LPSTR *args, CCircleShip *CircleShip);
///Called everytime a time slot happens and a moving has to be done
void *CCS_FSM_Move		(LPSTR *args, CCircleShip *CircleShip);
///Called when the supply ship is going to burst before dying
void *CCS_FSM_Dye		(LPSTR *args, CCircleShip *CircleShip);
void *CCS_FSM_Dying		(LPSTR *args, CCircleShip *CircleShip);
void *CCS_FSM_Death		(LPSTR *args, CCircleShip *CircleShip);
///Called when the supply ship passed from death to unborn states
void *CCS_FSM_Unborning (LPSTR *args, CCircleShip *CircleShip);
///ACHTUNG: Temporalmente. Cambiar en el futuro
void *CCS_display		(LPSTR *args, CCircleShip *CircleShip);

#endif