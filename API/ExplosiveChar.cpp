/**
 	@file ExplosiveChar.cpp
 *	Character suitable for explode visibly on the screen in Space Invaders
 *	
 *	@author Ramon Molla <rmolla@dsic.upv.es>
 *	Prefix: CEC_
 *	HISTORY:
 *	@version 1.0 
 *  @date 02-2016
 */

#include <ExplosiveChar.h>

using namespace UGK;

/**
 * ~CExplosiveChar
 * Destroy all the information created at the construction and suring the playing time
 */
CExplosiveChar::~CExplosiveChar()
{
	if (Timer.size())
		Timer.resize(0);
	if (Directory.size())
		Directory.resize(0);
	if (Sound.size())
		Sound.resize(0);
}