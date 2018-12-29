/**	Definition of the class Bunker

*	Prefix: CBNK_

*	@author Ramon Molla
*	@version 2011-07
*/

#ifndef CBNK_BUNKER
#define CBNK_BUNKER

#include <Brick.h>

/// Every bunker may be 14 bricks Width and 10 lines high

#define CBNK_MAX_LINES		10
#define CBNK_MAX_COLUMNS	14

/**
*	\typedef CBNK_BUNKER_TYPE

*	Different types of bunkers by default
*/
typedef enum {
	CBNK_BLANK,			///<No bunker at all
	CBNK_HOME,			///<Default bunker. It seems a little house
	CBNK_USER_DEFINED,	///<The shape depends on the content of the configuration level html file
	CBNK_MAXTYPE		///<For accounting purpouses only
} CBNK_BUNKER_TYPE;

extern UGKS_String CBNK_TypeNames[CBNK_MAXTYPE];

/**
*	@class CBunker

	@author Ramon Molla
	@version 2011-09
*/
class CBunker: public CCharacter
{
	//Atributes
public:	
	CBrick Brick[CBNK_MAX_LINES][CBNK_MAX_COLUMNS];

	void CreateBrickLine(unsigned int line, Vector &Pos);

	//Methods
	void Init (void);
	void Init (float x, float y, float z);
	void Init (CBNK_BUNKER_TYPE Type, Vector &Pos);					///<The same as Init(Pos, true)
	void Init (Vector &Pos, bool BrickMatrix[CBNK_MAX_LINES][CBNK_MAX_COLUMNS]);	///<If true, the bunker is initialized by default

	CBunker()		///<Constructor of the class
	{
		Mesh = NULL;
		Init();
	}					
	void AssignSounds2Bricks();
	void MoveBricksTo	(Vector &Move);	///<Moving to an absolute position. It does not matter where the bunker is
	void TranslateBunker	(Vector &Pos);

	void Render(void);	///<Shows the bunker on the screen
	///Change the way all the bricks that belong to the bunker are going to be rendered on the screen
	void ChangeRenderMode(CHAR_RENDER_MODE);

#ifdef CHAR_USE_QUADTREE
	void SetQTRoot(QuadtreeRoot *QTR);
#endif

	//Physics
	void Collided (CCharacter *CollidedChar);
	void UpdateAABB();
	///Recalculates its AABB given a brick
	void UpdateAABB(CBrick Brick);
};


#endif