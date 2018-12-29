/**	Definition of the class Bonus

*	Prefix: CBN_

*	HISTORY

*	@author Ramon Molla
*	@version 2016-02
*/

#ifndef CBN_BONUS
#define CBN_BONUS

#include <ExplosiveChar.h>

#define CBN_WIDTH_2D  0.8f
#define CBN_HEIGTH_2D 0.8f
#define CBN_LENGTH_2D 0.5f

#define CBN_WIDTH_3D  3.0f
#define CBN_HEIGTH_3D 1.0f
#define CBN_LENGTH_3D 0.0f

#define CBN_YPOS 0.05f
#define CBN_ZPOS SIGLBD_DEFAULT_Z_POSITION+0.01f
#define CBN_MAX_ALPHA 0.9f
#define CBN_FADING_SPEED 0.0005f

/** \typedef CBN_EXTRA_TIMERS
*	Types of timers that could be used during the bonus activity
*/
typedef enum {
	CBN_UPD_PERIOD,
	CBN_RND_PERIOD,
	CBN_MAX_TIMERS
} CBN_EXTRA_TIMERS;

/** \typedef CBN_SOUNDS_TYPE
*	Types of SOUNDS bonuses that could be played by bonus activity
*/
typedef enum {
CBN_SHIELD_SND,
CBN_BONUS_SND,
CBN_MAX_SND
} CBN_SOUNDS_TYPE;

/** \typedef CBN_BONUS_TYPE
*	Types of bonuses that could be thrown to the player. Power ups
*/
typedef enum {
	CBN_BONUS_3D,
	CBN_BONUS_LASER,
	CBN_BONUS_WEAPON,
	CBM_BONUS_HEAL,
	CBM_BONUS_SHELL,
	CBN_MAX_BONUSES		// This tag is for management purpouses only. There is no correspondence to any string label in the HTML_FSM file
} CBN_BONUS_TYPE;

class CBonusManager;

/**
	@class CBonus: public CCharacter

	Support the Bonus class for upgrading the power or visualization 

	@author Ramon Molla
	@version 2011-02
*/
class CBonus: public CExplosiveChar
{
	//Atributes
public:
	float			Shake1, Shake2;	///<The scene is shaking
	float			Alpha,			///< For waving the vanishment of the bonus while falling downwards to the player
					FadingSpeed;	///< Velocity of Fading in and out the alpha channel of the texture for the bonus

	//Timing
	double		msUpdMsg;
	RTDESK_CMsg *msgUpd;		///<RTDESK Message for timing

	int 		GenPercent;		///< Percentage of time a new Bonus is generated

	//Methods
	void Init (void);

	CBonus();	///<Constructor of the class

	void Update (void);
	void DiscreteUpdate (void);

	void AI_Explode (void);	///<What to do when exploding a Bonus

	//Physics
	void Collided (CCharacter *CollidedChar);
	void PlayerCollided ();

	void Render(void);	///<Shows the bonus on the screen
	///Change the way the bonus is going to be rendered on the screen
	void ChangeRenderMode(CHAR_RENDER_MODE);

	inline void SetGenPercentage(float Percentage){GenPercent = RAND_MAX *Percentage*0.01f;}

	void ReceiveMessage(RTDESK_CMsg *pMsg);
};

#endif