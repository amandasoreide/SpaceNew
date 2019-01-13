/**	Definition of the class Super Ship

	Class Prefix: CSS_

	@author Ramon Molla
	@version 2011-01-11
*/


#ifndef CA_AMANDA
#define CA_AMANDA

#include <ExplosiveChar.h>
#include <SIMessage.h>

//Definitions for the game
#define AMA_MAX_FILE_NAME		 25
#define AMA_MAX_HEALTH			500 //Maximun amount of health of a given Supply Ship by default
#define AMA_MAX_HIT_DURATION	500

#define AMA_MAX_X_SHIFT			  7

#define AMA_SHOOT_SPEED		  0.017f

#define CA_SPECIALATTACK		0
#define CA_SHIELD				0
#define CA_TELEPORT				3
#define CA_THREEATTACKS			0

/** \typedef AMA_EXTRA_TIMERS

*	Types of different local timing managed by any supplyship
*/
typedef enum {
	AMA_UPD_PERIOD,
	AMA_RND_PERIOD,
	AMA_MAX_TIMERS
} CSS_EXTRA_TIMERS_AMA;

typedef enum {
	AMA_NO_SUPPLY_SHIP, ///For management purpouses
	AMA_SUPPLY_SHIP,
	AMA_BIG_SUPPLY_SHIP,	//Super powerful supply ship. More health, bigger and can launch ships from inside. Only in 3D mode
	AMA_MAXTYPE
} CA_TYPE_AMA;

///Artificial Intelligence
///Different states for the FSMs that control the SS behaviour
typedef enum {
	AMA_UNBORN = 0,	///For management purpouses only
	AMA_BORN,		///The character is just born but it is not still operative (living)
	AMA_LIVING,		///Everything this character may do while it is alive 
	AMA_DYING,		///The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	AMA_DEAD,		///The character is no operative. Reached after dying
	AMA_MAXSTATE	///For management purpouses only
} CA_AMDANDA_STATE;	///Generic character possible states that can be any character by default

///Different transitions for the FSMs that control the SS behaviour
typedef enum {
	AMA_DEFAULT = 0,				///For management purpouses only
	AMA_BORNING,				///The character is just borning. Passing from UNBORN to BORN states
	AMA_GETTING_ALIVE,			///Passing from BORN to LIVING states
	AMA_MOVING,					///Remaining in LIVING state while moving the Supply Ship
	AMA_DISPLAYING,				///Remaining in LIVING state while displaying the Supply Ship
	AMA_HEALTHING,				///Remaining in LIVING state while increasing the health of the Supply Ship
	AMA_BURST,					///The character has been touched so many times that its life has gone negative. So it has to burst. Passing from LIVING to DYING states
	AMA_DIE,					///The character is no operative. Reached after dying
	AMA_FINISHED,				///The character is no operative. Reached after the game has finished: player has lost or win the level or game is exited
	AMA_RESURRECTING,			///Passing to Unborn state
	AMA_MAXTRANSITION			///For management purpouses only
} CA_AMANDA_TRANSITIONS;	///Generic character possible states that can be any character by default

///The ships that contains the captain of the enemy navy. They are moving over the whole navy. It has a different geometry
class CAmanda : public CExplosiveChar
{
	//Attributes
public:
	//RT-DESK
	double		timeRTdeskMsg;		//Tiempo Mensaje RTDESK
	cMsgNavy	*msg;
	cMsgNavy	*msgUpd;			//RTDESK Message Time

	//Methods
	void Init();
	void CAmanda::InitTransforms();

	int specialattack;
	int shield;
	int teleport;
	int threeattacks;

	//AI Methods
	///Increasing health while time passes by
	void AI_Healthing();
	///Moving the Supply Ship
	void AI_Move();
	///The supply ship is going to burst before being dead
	void AI_Die();
	///What to do when the supply ship is dead
	void AI_Death();
	bool OutEvent(CA_AMANDA_TRANSITIONS EventName);

	//Physics
	void Collided(CCharacter *CollidedChar);

	///Shows the supplyship on the screen
	void Render(void);
	///Change the way the supplyship is going to be rendered on the screen
	void ChangeRenderMode(CHAR_RENDER_MODE Mode);

	void Update(void);
	void DiscreteUpdate(void);

	void ReceiveMessage(RTDESK_CMsg *pMsg);

	///Constructor of the class
	CAmanda();
	~CAmanda() {}
};

//External methods to use with the class internal FSM
///Called everytime a time slot happens and its health has to be increased
void *AMA_FSM_Healthing(LPSTR *args, CAmanda *SupplyShip);
///Called everytime a time slot happens and a moving has to be done
void *AMA_FSM_Move(LPSTR *args, CAmanda *SupplyShip);
///Called when the supply ship is going to burst before dying
void *AMA_FSM_Dye(LPSTR *args, CAmanda *SupplyShip);
void *AMA_FSM_Dying(LPSTR *args, CAmanda *SupplyShip);
void *AMA_FSM_Death(LPSTR *args, CAmanda *SupplyShip);
///Called when the supply ship passed from death to unborn states
void *AMA_FSM_Unborning(LPSTR *args, CAmanda *SupplyShip);
///ACHTUNG: Temporalmente. Cambiar en el futuro
void *AMA_display(LPSTR *args, CAmanda *SupplyShip);

#endif