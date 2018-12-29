#ifndef CA_AMANDA
#define CA_AMANDA

#include <ExplosiveChar.h>
#include <UGKTexture.h>
#include <SIMessage.h>

#define CA_MAX_HEALTH			 25	//Maximun amount of health of a given Ship by default
#define CA_MAX_MAX_HIT_DUR		500	//Maximun amount of time when a given Ship is hit
#define CA_MAX_EXPLOSION_LIFE 10000	//Time in ms the ship may be exploding
#define CA_SPECIALATTACK		0
#define CA_SHIELD				0
#define CA_TELEPORT				3
#define CA_THREEATTACKS			0

#define CA_SHOOT_SPEED		  0.004f//In units per milisecond

/** \typedef CS_EXTRA_TIMERS
*	Types of different local timing managed by any ship
*/
typedef enum {
	CA_UPD_PERIOD,
	CA_RND_PERIOD,
	CA_MAX_TIMERS
} CA_EXTRA_TIMERS;

typedef enum {
	CA_NO_AMANDA,
	CA_HIGH1,
	CA_HIGH2,
	CA_HIGH3,
	CA_MIDDLE,
	CA_LOW,
	CA_MAXTYPE
} CA_AMANDA_TYPE;

typedef enum {
	CA_UNBORN = 0,	///For management purpouses only
	CA_BORN,		///The character is just born but it is not still operative (living)
	CA_LIVING,		///Everything this character may do while it is alive 
	CA_BURSTING,	///The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CA_DEAD,		///The character is no operative. Reached after dying
	CA_MAXSTATE		///For management purpouses only
} CA_AMANDA_STATE;

typedef enum {
	CA_DEFAULT = 0,		///For management purpouses only
	CA_BORNING,			///The character is going to be born but it is not still operative (living)
	CA_GROWING,			///Change to LIVING State
	CA_MOVING,			///Any time this character is moving while it is alive 
	CA_DISPLAYING,		///Renders the ship on the screen
	CA_BURST,			///The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CA_REBORN,
	CA_DIE,				///The ship has got bursted and now has to be dead
	CA_MAXTRANSITION	///For management purpouses only
} CA_AMANDA_TRANSITION;

class CAmanda : public CExplosiveChar
{
	void InitTransforms3D();
	void UpdateRadius();

	//Atributes
public:
	float			zi;				// The ship floats (coord z in 3D mode)
	float			zi_speed;		// The ship floats (speed)
	float			my_rand;		// random number
	float Radius;

	int specialattack;
	int shield;
	int teleport;
	int threeattacks;

	C3DS_Color4f 	Color3D;		//For 3D mode

									//For united ships
	bool Bounce;

	//RT-DESK
	double timeRTdeskMsg;						//Tiempo Mensaje RTDESK
	cMsgNavy *msg;
	cMsgNavy *msgUpd;							//RTDESK Message Time
												//Methods
												//Constructor / Initializer
	void Init();

	CAmanda()
	{
		Mesh = NULL;
		Init();
	}	// Constructor of the class

	~CAmanda();

	void InitTransforms2D();

	//AI functions
	//void AI_Living();
	///Moving the Supply Ship
	//void AI_Move();
	///Character AI virtual methods rewritten
	void AI_Die(void);

	bool OutEvent(CA_AMANDA_TRANSITION EventName);

	//Physics
	void Collided(CCharacter *CollidedChar);

	///Shows the ship on the screen
	void Render(void);
	void Render2(void);
	///Change the way the ship is going to be rendered on the screen
	void ChangeRenderMode(CHAR_RENDER_MODE);

	void Render3D();
	void DisplayHit();

	void Update(void);
	void DiscreteUpdate(void);

	void ReceiveMessage(RTDESK_CMsg *pMsg);
	//CAmanda		();
};
// changed
void *init_amanda(LPSTR *args, CAmanda *Amanda);
void *CA_Move(LPSTR *args, CAmanda *Amanda);
void *CA_Bursting(LPSTR *args, CAmanda *Amanda);
void *CA_Kill(LPSTR *args, CAmanda *Amanda);
void *CA_Display(LPSTR *args, CAmanda *Amanda);

#endif