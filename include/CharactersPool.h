/**
*	@file	 CharactersPool.cpp
*	@author  Santiago Jim&eacute;nez Serrano [sanjiser@gmail.com]
*	@date	 2016-01-25
*  @version 1.0
*
*	@brief	 Implementation of interface IUGKCharactersFactory.\n
*			 Provides a CCharactersFactory, to create and store CCharacter objects.
**/

#include <UGKCharactersPool.h>
#include <UGKCharactersFactory.h>
#include <GameCharacters.h>

#include <Bonus.h>
#include <Brick.h>
#include <Bunker.h>
#include <BunkersManager.h>
//#include <CircleShip.h>
#include <Game.h>
#include <Laser.h>
//#include <Missil.h>
#include <Navy.h>
#include <Player.h>
// #include <PlayerShoot.h>
#include <Reactor.h>
#include <Ship.h>
//#include <SphereOgive.h>
#include <SupplyShip.h>
//#include <Weapon.h>
#include <Shoot.h>

using namespace UGK;

// GCHARS_CharacterType;
// Definidos en GameCharacters.h

/**
* @brief CCharactersFactory \n
* Represents a Factory of CCharacters objects. Implements IUGKCharactersFactory.
*/
class CCharactersFactory : IUGKCharactersFactory
{
	/**
	* @fn     createNewCharacter
	* @param  int characterType Type of CCharacter to be created. Possible values: GCHARS_CharacterType enum values.
	* @return Pointer to the new CCharacter created.
	* @brief  Creates a new CCharacter of the specified type. See GCHARS_CharacterType
	*/
	CCharacter* createNewCharacter(int characterType) override
	{
		switch (characterType)
		{
		case  GCHARS_CharacterType::CHARS_CHARACTER_UNKNOWN: return new CCharacter();		///<For management purpouses only
		case  GCHARS_CharacterType::CHARS_BONUS:             return new CBonus();			///<For improving player strenght
		case  GCHARS_CharacterType::CHARS_BRICK:             return new CBrick();			///<Pieces that make a bunker
		case  GCHARS_CharacterType::CHARS_BUNKER:            return new CBunker();			///<A collection of bricks together in order to defend the player
		case  GCHARS_CharacterType::CHARS_BUNKERSMANAGER:    return new CBunkersManager();	///<The manager of all the bunkers available in a given play
		case  GCHARS_CharacterType::CHARS_CIRCLESHIP:        return new CCircleShip();		///<Alien space ships that turns once and once again in clircles on the screen
		case  GCHARS_CharacterType::CHARS_GAME:              return new CGame();			///<The game itself 
		case  GCHARS_CharacterType::CHARS_LASER:             return new CLaser();			///<Additional shooting left or right device for the player
		case  GCHARS_CharacterType::CHARS_MISSIL:            return new CShoot();			///<A common shooting from any ship, supplyship or player
		case  GCHARS_CharacterType::CHARS_NAVY:              return new CNavy();			///<Holds all the supplyships and all types of ships
		case  GCHARS_CharacterType::CHARS_PLAYER:            return new CPlayer();			///<The player
		case  GCHARS_CharacterType::CHARS_PLAYER_SHOOT:      return new CShoot();			///<Player shoots
		case  GCHARS_CharacterType::CHARS_REACTOR:           return new CReactor();			///<Extra power engines and shooters for the player. Obtained after the corresponding bonus has been won
		case  GCHARS_CharacterType::CHARS_SHIP:              return new CShip();			///<Alien normal space ships
		case  GCHARS_CharacterType::CHARS_SHIP_SHOOT:        return new CShoot();			///<Alien normal space ships shoots
		case  GCHARS_CharacterType::CHARS_SPHERE_OGIVE:      return new CCharacter();		///<Not available by the moment
		case  GCHARS_CharacterType::CHARS_SUPPLYSHIP:        return new CSupplyShip();		///<Super ship of the alien troops
		case  GCHARS_CharacterType::CHARS_WEAPON:            return new CCharacter();		///<Not available by the moment
		case  GCHARS_CharacterType::CHARS_MAX_CHARTYPE:      return new CCharacter();		///<Only for character types management. No object exist in the game for this type

		default: return NULL;
		}
	}


	/**
	* @fn     getNumDistinctCharacterTypes
	* @return The number of different types of Characters that this factory can produce. In this case, GCHARS_CharacterType::CHARS_MAX_CHARTYPE.
	* @brief  Gets the total number of different types of Characters that this factory can produce.
	*/
	int getNumDistinctCharacterTypes() override
	{
		return GCHARS_CharacterType::CHARS_MAX_CHARTYPE;
	}
};

/**
* @brief CCharactersPool \n
* Represents a Factory of CCharacters objects, contained at SpaceInvaders.
*/
class CCharactersPool : CUGKCharactersPool
{
public:

	/**
	* @fn    CCharactersPool
	* @brief CCharactersPool constructor. It employs a CCharactersFactory as CCharacters Factory for Space Invaders objects only.
	*/
	CCharactersPool()
		:CUGKCharactersPool((IUGKCharactersFactory*)new CCharactersFactory())
	{
	}

};

