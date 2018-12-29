/**	Definition of the class Super Ship

	Class Prefix: CSS_

	@author Ramon Molla
	@version 2011-01-11
*/


#ifndef CSS_SUPPLYSHIP
#define CSS_SUPPLYSHIP

#include <ExplosiveChar.h>
#include <SIMessage.h>

//Definitions for the game
#define CSS_MAX_FILE_NAME		 25
#define CSS_MAX_HEALTH			500 //Maximun amount of health of a given Supply Ship by default
#define CSS_MAX_HIT_DURATION	500

#define CSS_MAX_X_SHIFT			  4

#define CSS_SHOOT_SPEED		  0.017f

/** \typedef CSS_EXTRA_TIMERS

*	Types of different local timing managed by any supplyship
*/
typedef enum {
	CSS_UPD_PERIOD,
	CSS_RND_PERIOD,
	CSS_MAX_TIMERS
} CSS_EXTRA_TIMERS;

typedef enum {
	CSS_NO_SUPPLY_SHIP, ///For management purpouses
	CSS_SUPPLY_SHIP,
	CSS_BIG_SUPPLY_SHIP,	//Super powerful supply ship. More health, bigger and can launch ships from inside. Only in 3D mode
	CSS_MAXTYPE
} CSS_TYPE;

///Artificial Intelligence
///Different states for the FSMs that control the SS behaviour
typedef enum {
	CSS_UNBORN=0,	///For management purpouses only
	CSS_BORN,		///The character is just born but it is not still operative (living)
	CSS_LIVING,		///Everything this character may do while it is alive 
	CSS_DYING,		///The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CSS_DEAD,		///The character is no operative. Reached after dying
	CSS_MAXSTATE	///For management purpouses only
} CSS_SUPPLYSHIP_STATE;	///Generic character possible states that can be any character by default

///Different transitions for the FSMs that control the SS behaviour
typedef enum {
	CSS_DEFAULT=0,				///For management purpouses only
	CSS_BORNING,				///The character is just borning. Passing from UNBORN to BORN states
	CSS_GETTING_ALIVE,			///Passing from BORN to LIVING states
	CSS_MOVING,					///Remaining in LIVING state while moving the Supply Ship
	CSS_DISPLAYING,				///Remaining in LIVING state while displaying the Supply Ship
	CSS_HEALTHING,				///Remaining in LIVING state while increasing the health of the Supply Ship
	CSS_BURST,					///The character has been touched so many times that its life has gone negative. So it has to burst. Passing from LIVING to DYING states
	CSS_DIE,					///The character is no operative. Reached after dying
	CSS_FINISHED,				///The character is no operative. Reached after the game has finished: player has lost or win the level or game is exited
	CSS_RESURRECTING,			///Passing to Unborn state
	CSS_MAXTRANSITION			///For management purpouses only
} CSS_SUPPLYSHIP_TRANSITIONS;	///Generic character possible states that can be any character by default

///The ships that contains the captain of the enemy navy. They are moving over the whole navy. It has a different geometry
class CSupplyShip: public CExplosiveChar
{
	//Attributes
public:
	//RT-DESK
	double		timeRTdeskMsg;		//Tiempo Mensaje RTDESK
	cMsgNavy	*msg;
	cMsgNavy	*msgUpd;			//RTDESK Message Time

	//Methods
	void Init ();
	void CSupplyShip::InitTransforms();

	//AI Methods
	///Increasing health while time passes by
	void AI_Healthing	();
	///Moving the Supply Ship
	void AI_Move		();
	///The supply ship is going to burst before being dead
	void AI_Die			();
	///What to do when the supply ship is dead
	void AI_Death		();
	bool OutEvent		(CSS_SUPPLYSHIP_TRANSITIONS EventName);

	//Physics
	void Collided		(CCharacter *CollidedChar);

	///Shows the supplyship on the screen
	void Render				(void);
	///Change the way the supplyship is going to be rendered on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE Mode);

	void Update(void);
	void DiscreteUpdate(void);

	void ReceiveMessage(RTDESK_CMsg *pMsg);

	///Constructor of the class
	CSupplyShip();
	~CSupplyShip(){}
};

//External methods to use with the class internal FSM
///Called everytime a time slot happens and its health has to be increased
void *CSS_FSM_Healthing	(LPSTR *args, CSupplyShip *SupplyShip);
///Called everytime a time slot happens and a moving has to be done
void *CSS_FSM_Move		(LPSTR *args, CSupplyShip *SupplyShip);
///Called when the supply ship is going to burst before dying
void *CSS_FSM_Dye		(LPSTR *args, CSupplyShip *SupplyShip);
void *CSS_FSM_Dying		(LPSTR *args, CSupplyShip *SupplyShip);
void *CSS_FSM_Death		(LPSTR *args, CSupplyShip *SupplyShip);
///Called when the supply ship passed from death to unborn states
void *CSS_FSM_Unborning (LPSTR *args, CSupplyShip *SupplyShip);
///ACHTUNG: Temporalmente. Cambiar en el futuro
void *CSS_display		(LPSTR *args, CSupplyShip *SupplyShip);

#endif