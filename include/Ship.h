/**	Definition of the class Ship

	Prefix: CS_
	@author Ramon Molla
	@version 2011-09-11
*/


#ifndef CS_SHIP
#define CS_SHIP

#include <ExplosiveChar.h>
#include <UGKTexture.h>
#include <SIMessage.h>

#define CS_MAX_HEALTH			 25	//Maximun amount of health of a given Ship by default
#define CS_MAX_MAX_HIT_DUR		500	//Maximun amount of time when a given Ship is hit
#define CS_MAX_EXPLOSION_LIFE 10000	//Time in ms the ship may be exploding

#define CS_SHOOT_SPEED		  0.004f//In units per milisecond

/** \typedef CS_EXTRA_TIMERS
*	Types of different local timing managed by any ship
*/
typedef enum {
	CS_UPD_PERIOD,
	CS_RND_PERIOD,
	CS_MAX_TIMERS
} CS_EXTRA_TIMERS;

typedef enum {
	CS_NO_SHIP,
	CS_HIGH1,
	CS_HIGH2,
	CS_HIGH3,
	CS_MIDDLE,
	CS_LOW,
	CS_MAXTYPE
} CS_SHIP_TYPE;

typedef enum {
	CS_UNBORN=0,	///For management purpouses only
	CS_BORN,		///The character is just born but it is not still operative (living)
	CS_LIVING,		///Everything this character may do while it is alive 
	CS_BURSTING,	///The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CS_DEAD,		///The character is no operative. Reached after dying
	CS_MAXSTATE		///For management purpouses only
} CS_SHIP_STATE;

typedef enum {
	CS_DEFAULT=0,		///For management purpouses only
	CS_BORNING,			///The character is going to be born but it is not still operative (living)
	CS_GROWING,			///Change to LIVING State
	CS_MOVING,			///Any time this character is moving while it is alive 
	CS_DISPLAYING,		///Renders the ship on the screen
	CS_BURST,			///The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CS_REBORN,
	CS_DIE,				///The ship has got bursted and now has to be dead
	CS_MAXTRANSITION	///For management purpouses only
} CS_SHIP_TRANSITION;

class CShip: public CExplosiveChar
{
	void InitTransforms3D();
	void UpdateRadius();

	//Atributes
public:
	float			zi;				// The ship floats (coord z in 3D mode)
	float			zi_speed;		// The ship floats (speed)
	float			my_rand;		// random number
	float Radius;

	C3DS_Color4f 	Color3D;		//For 3D mode
	
	//For united ships
	bool Bounce;

	//RT-DESK
	double timeRTdeskMsg;						//Tiempo Mensaje RTDESK
	cMsgNavy *msg;
	cMsgNavy *msgUpd;							//RTDESK Message Time

	//Methods
	
	//Constructor / Initializer
	void Init ();

	CShip ()
	{
		Mesh = NULL;
		Init();
	}	// Constructor of the class

	~CShip();

	void InitTransforms2D();

	//AI functions
	//void AI_Living();
	///Moving the Supply Ship
	void AI_Move			();
	///Character AI virtual methods rewritten
	void AI_Die				(void);	
	
	bool OutEvent			(CS_SHIP_TRANSITION EventName);

	//Physics
	void Collided			(CCharacter *CollidedChar);

	///Shows the ship on the screen
	void Render				(void);
	///Change the way the ship is going to be rendered on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE);

	void Render3D			();
	void DisplayHit			();

	void Update				(void);
	void DiscreteUpdate		(void);

	void ReceiveMessage		(RTDESK_CMsg *pMsg);
};

void *init_ship		(LPSTR *args, CShip *Ship);
void *CS_Move		(LPSTR *args, CShip *Ship);
void *CS_Bursting	(LPSTR *args, CShip *Ship);
void *CS_Kill		(LPSTR *args, CShip *Ship);
void *CS_Display	(LPSTR *args, CShip *Ship);

#endif