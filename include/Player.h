/**	Definition of the class Player
	Class prefix CPL_

	@author Ramon Molla
	@version 2011-09-11
*/

#ifndef CPL_CLASS_PLAYER
#define CPL_CLASS_PLAYER

#include <Shoot.h>
#include <Reactor.h>
#include <Laser.h>

#define CPL_NO_PLAYER			-1

#define CPL_PLAYER_Y_2D			SIGLBD_PG_BOTTOM + 4.0f
#define CPL_PLAYER_Y_3D			SIGLBD_PG_BOTTOM + 4.0f

/** \typedef CPL_EXTRA_TIMERS
*	Types of different local timing managed by any player
*/
typedef enum {
	CPL_UPD_PERIOD,
	CPL_RND_PERIOD,
	CPL_MAX_TIMERS
} UGKP_EXTRA_TIMERS;

/** \typedef CPL_PLAYER_LASER
*	Lasers managed by any player
*/
typedef enum {
	CPL_LEFT_LASER,					///<Code for left laser
	CPL_RIGHT_LASER,				///<Code for right laser
	CPL_MAX_LASERS					///<Amount of extra lasers the player has
} CPL_PLAYER_LASER;

/** \typedef CPL_PLAYER_REACTOR
*	Reactors managed by any player
*/
typedef enum {
	CPL_LEFT_REACTOR,				///<Code for left reactor
	CPL_RIGHT_REACTOR,				///<Code for right reactor
	CPL_MAX_REACTORS				///<Amount of extra reactors the player has
} CPL_PLAYER_REACTOR;

typedef enum {
	CPL_MOVE_LEFT,			///<The player wants to move to the left
	CPL_MOVE_RIGHT,			///<The player wants to move to the right
	CPL_SHOOT,				///<The player wants to fire a shoot
	CPL_MAX_PLAYER_EVENTS	// Only for management purpouses
} CPL_PLAYER_EVENTS;

/** \typedef CPL_SOUNDS_TYPE
*	Types of SOUNDS the player could play during its game activity
*/
typedef enum {
	CPL_EXPLOSION_SND,
	CPL_SHOOT_SND,
	CPL_SHOOT3D_SND,
	CPL_TOUCH_SND,
	CPL_MAX_SND
} CPL_SOUNDS_TYPE;

typedef enum {
	CPL_UNBORN = 0,	///<For management purpouses only
	CPL_BORN,		///<The character is just born but it is not still operative (living)
	CPL_LIVING,		///<Everything this character may do while it is alive 
	CPL_DYING,		///<The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CPL_MOVING,		///<The player is moving to the left or to the right
	CPL_SPEEDINGUP, ///<Player is moving the 
	CPL_SHOOTING,	///<The player is shooting to the space invaders
	CPL_DEAD,		///<The character is no operative. Reached after dying
	CPL_MAXSTATE	///<For management purpouses only
} CPL_PLAYER_STATE;

class CPlayer : public CExplosiveChar
{
	//Attributes
public:
	short int	Lives;		///<Amount of lives a player can lose before finishing the game
	float		yi;			// le vaisseau est secoué (passage à game 3D)
	float		yi_speed;	// le vaisseau est secoué (vitesse)
	float		yi_cos;		// le vaisseau est secoué (variable de cosinus)
	float		zi;			// le vaisseau 'flotte' (coord z, pour mode 3D)
	float		zi_speed;	// le vaisseau 'flotte' (vitesse)
	float		Scrolling;	// Speed used in the scrolling, Control to rotate the Player

	//Shooting
	CSH_SHOOT_TYPE	ShootType;	///<TYPE of shooting power in a given moment. It depends on the bonus got during the playing
	float			ShootSpeed;	///<Initial speed of the current shoot

	CPL_PLAYER_STATE State;
	bool			Passing2D23D,	///< Flag that prevents any destruction of the player, lasers,... while moving from 2D rendering mode to 3D one
					DiscreteSimulation;
	float			Direction;		//-1.0 to the left and +1.0 to the right

	//RT-DESK
	double timeRTdeskMsg;			///<RTDESK Message Time
	RTDESK_CMsg *msg;				///<RTDESK Message

	//Weapons
	CLaser*		Laser	[CPL_MAX_LASERS];	///<Extra lasers a player can have in a given moment
	CReactor*	Reactor	[CPL_MAX_REACTORS];	///<Reactors the player's ship have to perform the movement

	//Methods
	bool Init ();	//Used when all the values are initialized by default
					//when reading the global initialization game file
					//Returns true if the initialization of the attirbute has been correctly done

	CPlayer();		//Constructor

#ifdef CHAR_USE_QUADTREE
	void SetQTRoot(QuadtreeRoot *QTR);
#elif CHAR_USE_AABB
#endif

	void SetEndingCriteria	(bool ByTime, bool ByFrame);

	///Character AI virtual methods rewritten
	void AI_Init			(void);	///<Default Artificial Intelligence character initialization
	void AI_Explode			(void);
	void AI_Die				(void);

	void ActivateLaser		(CPL_PLAYER_LASER Laser);
	void ActivateReactor	(void);

	void Pass2D3D();
	void SetRenderPeriod	(HRT_Timems RP);
	void SetUpdatePeriod	(HRT_Timems RP);
	void SetLocalTimers		();

	void AssignTextMngr		(CCharacter * TM);
	void ResizeDirectory	(unsigned int S);
	void SetGameRef			(CCharacter *Game);
	inline void SetScale2D()
	{
		Scale.v[XDIM] = 0.3;
		Scale.v[YDIM] = 0.6;
		Scale.v[ZDIM] = 0.1;
	}

	inline void SetScale3D()
	{
		Scale.v[XDIM] = 0.1;
		Scale.v[YDIM] = 0.1;
		Scale.v[ZDIM] = 0.1;
	}

	//Physics
	void Collided			(CCharacter *CollidedChar);

	///Shows the player on the screen
	void Render				(void);
	///Change the way the player is going to be rendered on the screen
	void ChangeRenderMode	(CHAR_RENDER_MODE);
	void RenderExplosion	(void);
	void DiscreteUpdate		(void);
	void Update				(void);

	//Transformation methods
	void MoveTo		(float x, float y, float z);	///<Moves to the absolute coordinate x,y,z
	void MoveTo		(Vector &Move);					///<Moving to an absolute position. It does not matter where the character is
	void MoveRelTo	(float x, float y, float z);	///<Moves x,y,z units from the to the current position
	void MoveRelTo	(Vector &Move);					///<Moving relative to the character current Position

	//User interaction. One method for every player command sent to the player's avatar
	void ManageEvent(unsigned int, bool On);	///<Virtual method for managing events
	void UserRight			(bool On);
	void UserLeft			(bool On);
	void UserShoot			();
	
	void ReceiveMessage(RTDESK_CMsg *pMsg);	
};

extern vector<CPlayer*>	Players;		///<Amount of Players available for the game
extern int				CurrentPlayer;	///<Initially, the player by default is the one before the first one

#endif