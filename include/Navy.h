/*Definition of the class game
  General class to support all the attribute general to all the ships, supply ships, general tactics,...
  Prefix: CN_

	@author Ramon Molla
	@version 2011-11
*/

#ifndef CN_CLASS_NAVY //se borra la macro de la nave
#define CN_CLASS_NAVY //se define la macro de la nave

#include <UGKMeshManager.h>
#include <ExplosiveChar.h>
#include <SIMessage.h>

#ifndef DEQUE_LOADED
#define DEQUE_LOADED
#include <deque>			// Uses Standard Template Library Double Ended Queue
using namespace std;
#endif

//Definitions for the Navy
#define CN_MAX_X_SHIFT		 10			//Initial value of the attribute ShipsBalancer
#define CN_SHIPS_MAX_SPEED	 0.001f		//Maximun speed for any given ship in any moment of the game in units/ms
#define CN_INITIAL_SPEED	-0.0005f	//Initial speed for any given ship at the begining of the game in units/ms
#define CN_INITIAL_ACC		 0.00001f	//Acceleration for any given ship every time the nay bounces in units/ms

#define CN_INITIAL_X_POS	(SIGLBD_PG_LEFT+0.3f*SIGLBD_PG_WIDTH)			//Initial position for any given ship at the begining of the game
#define CN_INITIAL_Y_POS	(SIGLBD_PG_CEILING-0.6f*SIGLBD_PG_HEIGHT)		//Initial position for any given ship at the begining of the game
#define CN_INITIAL_Z_POS	 0.0f		//Initial position for any given ship at the begining of the game

#define CN_INITIAL_WIDTH_POS	(SIGLBD_PG_WIDTH*0.85f)		//Initial navy width area where any given ship is set at the begining of the game
#define CN_INITIAL_HEIGHT_POS	(SIGLBD_PG_HEIGHT*0.25f)	//Initial navy height area where any given ship is set at the begining of the game

#define CN_MAX_SHIPS		50		///Amount of Ships available during the same playing level
#define CN_MAX_ROWS			 5		///Amount of ship rows the navy can have
#define CN_MAX_COLUMNS		10		///Amount of ship columns the navy can have

#define CN_SHIP_X_SHIFT		1.0f
#define CN_SHIP_Y_SHIFT		0.8f

#define CN_DOWN_STEP		-0.005f

#define CN_LIMIT_CHANGE2PHOTO1	100	//When reached this limit, change to photogram1
#define CN_LIMIT_CHANGE2PHOTO2	 50	//When reached this limit, change to photogram2

#define CN_DEFAULT_INDEXES	CHAR_DEFAULT_INDEXES		///Indexes of Textures and Meshes are initialized to -1.

#define CN_MAX_CIRCLESHIPS	10		///Amount of cicleship 

/** \typedef CN_EXTRA_TIMERS
*	Types of different local timing managed by the NAVY
*/
typedef enum {
	CN_UPD_PERIOD,
	CN_RND_PERIOD,
	CN_MAX_TIMERS
} CN_EXTRA_TIMERS;

/** \typedef CN_SOUNDS_TYPE
*	Types of SOUNDS a circle ship, supply ship or a plain ship (the whole navy) could play during the game
*/
typedef enum {
	CN_EXPLOSION_SND,
	CN_DESCEND_SND,
	CN_MAX_SND
} CN_SOUNDS_TYPE;

/**   
   * @class CNavy: public CCharacter
   * Defined the class Ship with inherit of the class CCharacter
   * And in this class it defined the variables for the ship, direction, shut, life, animation, suministries
   * Well as updates of the ship when you get items and bonus
   
	@author Ramon Molla
	@version 2011-11
*/

class CNavy: public CCharacter
{
public:
	//Attributes

	//Game general attributes
	UGKS_String Version;	// Game VERSION
	UGKS_String Title;		// Game TITLE

	//Ships Control
	///Increases the amount of shootings simultaneously on the same screen
	float			ShootsFrequency;
	///Ships health level per level
	int				ShipLevDefaultHealth;
	///Ships health by default
	int				ShipDefaultHealth;

	bool			ReGenShips;

	CSound DescendingSound;

	///When loading a level, this is the current ship being loaded right now
	///Belongs to the interval [0,CN_MAXSHIPS[
	unsigned int	Rows, Columns;	//Amount of ships

	bool Bounce;
	int  ShipsMoveDown;
	int  WithShoots;

	//RT-DESK
	double		timeRTdeskMsg;					//Tiempo Mensaje RTDESK Render
	cMsgNavy	*msg;							//RTDESK Message Time

//methods
	void ReceiveMessage(RTDESK_CMsg *pMsg);

	void Init (void);
	void SetBounce(void);	///Performs all the geometric transformations on every ship alive in the navy

	//Constructor
	CNavy () {Init();};
	
	//Setters for all the ships
	void SetLocalTimers	();
	void SetRenderPeriod();
	void SetMeshes		(CMeshesManager* MeshesManager);	///<Assigns a loaded mesh from the MeshesManager to every ship and supplyship 
	void SetSounds2Ships();
	void SetShipsAttributes();

	///AI method
	void AI_Go2Play(void);
};

#endif
