/**	Character.cpp - Clases, types, definitions, constants,..
	related with any kind of characters that may appear in the game.

	@author Ramon Molla
	@version 2012-06
*/

//#include "Copyright.h"
#include <Characters.h>
#include <malloc.h>

/**
*	\fn CCharacter * CCharacterFactory::Create(GCHARS_CharacterType Type, unsigned int Amount)
*	Generates a pointer to the array of characters requested
*	\param[in] Type The token that identifies a character
*	\param[in] Amount The number of characters to create
*	returns The array of requested characters
*/
void *CCharacterFactory::Create (CFC_CharacterIdType Type, unsigned int Amount)
{
	for (CFC_CharacterDict::iterator it=Characters.begin(); it!=Characters.end(); ++it)
		if (Type == it->second.Id)
				return malloc(it->second.Size*Amount);

	return NULL;
}