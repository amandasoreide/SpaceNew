/**	Definition of the class Brick

*	Prefix: CB_

*	@author Ramon Molla
*	@version 2011-08
*/

#ifndef CB_BRICK
#define CB_BRICK

#include <ExplosiveChar.h>

using namespace UGK;

//X - Y - Z
#define CB_BRICK_WIDTH	0.1f
#define CB_BRICK_LENGTH	0.1f
#define CB_BRICK_HEIGHT	0.1f

#define CB_BRICK_Z_POS	0.0f

#define CB_HLF_BRICK_WIDTH	0.05f
#define CB_HLF_BRICK_LENGTH	0.05f
#define CB_HLF_BRICK_HEIGHT	0.05f

#define CB_HEALTH_DEFAULT	1.0f
#define CB_MAX_MAX_HIT_DUR	400

/**
*	\typedef CB_BRICK_STATE
*	Different states a brick can reach during the playing
*/
typedef enum {
	CB_UNBORN=0,	///<For management purpouses only
	CB_BORN,		///<The character is just born but it is not still operative (living)
	CB_LIVING,		///<Everything this character may do while it is alive 
	CB_DYING,		///<The character has been touched so many times that its life has gone negative. So it has to burst before being died. Starts an explosion and dies
	CB_DEAD,		///<The character is no operative. Reached after dying
	CB_MAXSTATE		///<For management purpouses only
} CB_BRICK_STATE;

/**
*	\typedef CB_BRICK_TYPE
*	Different types of bricks can be used by a bunker during the playing
*/
typedef enum {
	CB_NO_BRICK=0,		///<There is no brick 
	CB_DEFAULT,			///<Brick with default hardness
	CB_HARD,			///<Hard brick. It last more than default brick
	CB_INDESTRUCTIBLE,	///<This brick is never destroyed even by the most powerful weapons
	CB_MAXTYPE			///<For management purpouses only
} CB_BRICK_TYPE;

/**
*	\typedef CB_BRICK_SOUNDS
*	Different sounds a brick can perform during the playing: explode, touch a laser,...
*/
typedef enum {
	CB_TOUCH_SND,	///<Sound produced when a shoot hits the brick
	CB_MAX_SOUND	///<For management purpouses only
} CB_BRICK_SOUNDS;

/**
*	\class CBrick
*	The little bricks used to build the bunkers that defend the player against the enemy navy
*	The colour/material used may vary depending on its type
*/
class CBrick: public CExplosiveChar
{	
public:

	//Attributes
	unsigned int Column;
	unsigned int Row;

	//Methods
	void Init ();

	CBrick()
	{
		Mesh = NULL;
		Init();
	}	///<Constructor of the class

	//Physics
	void Collided (CCharacter *CollidedChar);

	void Render(void);	///<Shows the brick on the screen
};

#endif