/**
 *	@file: ExplosiveChar.h 
 *	Character suitable for explode visibly on the screen in Space Invaders
 *	
 *	@author Ramon Molla <rmolla@dsic.upv.es>
 *	Prefix: CEC_
 *	HISTORY:
 *	@version 1.0 
 *  @date 02-2016
 */

#ifndef CEC_EXPLOSIVE_CHAR
#define CEC_EXPLOSIVE_CHAR

#include <Explosion.h>
#include <GameCharacters.h>
#include <GlobalTiming.h>


using namespace UGK;

/**
 * @class CExplosiveChar
 * This class supports only a character that may rendered on the screen
 *
 */
class CExplosiveChar : public CCharacter
{
public:

	CExplosion	Explosion;	///<When the ships loses its life, it burst. This is the particle system to perform the explosion

	bool		EndByTime,
				EndByFrame;

	//Methods
	inline  void Init(void){}

	CExplosiveChar(){ Init(); }	//Constructor of the class
	~CExplosiveChar();

	/**
	@fn void ResizeDirectory(unsigned int S) 
	*	Change the size of the Directory of references to external objects a Explosive Character needs to perform correctly
	*	It also changes the Directory of references to external objects for its Explosion attribute
	*/
	inline void ResizeDirectory(unsigned int S)
	{ 
		Directory.resize(S);
		Explosion.Directory.resize(S);
	}
	
	/**
	@fn void AssignGame(CCharacter * TM)
	*	Assigns the Game reference and the Texture Manager in the Directory of object references
	*	It also changes input in the Directory of references to external objects for its Explosion attribute
	*/
	inline void AssignTMG(CCharacter *Game)
	{
		ResizeDirectory(CHARS_MAX_REFERENCES);
		AssignGame(Game);
		AssignTextMngr(Game->Directory[CHARS_TEXTURES_MNGR_REF]);
	}
		
	/**
	@fn void AssignTextMngr(CCharacter * TM)
	*	Assigns the Texture Manager reference in the Directory of object references in order to render the character correctly
	*	It also changes input in the Directory of references to external objects for its Explosion attribute
	*/
	inline void AssignTextMngr(CCharacter * TM)
	{ 
		Directory[CHARS_TEXTURES_MNGR_REF] = TM; 
		Explosion.Directory[CHARS_TEXTURES_MNGR_REF] = TM; 
	}
	
	/**
	@fn void AssignGame(CCharacter * TM)
	*	Assigns the Game reference in the Directory of object references
	*	It also changes input in the Directory of references to external objects for its Explosion attribute
	*/
	inline void AssignGame(CCharacter * G)
	{ 
		Directory[CHARS_GAME_REF] = G; 
		Explosion.Directory[CHARS_GAME_REF] = G;
	}

	/**
	*	@fn SetLocalTimers()
	*	Starts up all the local timers required to work during tipical phases update, render,...
	*/
	inline void SetLocalTimers(unsigned int Size) {Timer.resize(Size); UpdateSF(TimerManager.GetSF());}
};
#endif