/*/////////////////////////////////////////////////////////////////////////////////////
//
// Bunkers manager
//

	@author Ramon Molla
	@version 2017-05
*/

#include <GlobalTiming.h>
#include <BunkersManager.h>
#include <GameCharacters.h>

//////////////////////////////////////////////////////////////////////////////////////
//
// Bunkers manager Initialisation 
//
//

void CBunkersManager::Init (void)	//Used when all the values are initialized by default
						
{
	BunkerAvailable = CBM_MAX_BUNKERS - 1;	//As soon as a new bunker is demanded, this pointer is increased by one, getting out of range and reseting to point to the first bunker available, that is to say "0"
	BunkersAmount	= CBM_MAX_BUNKERS;
	DefaultBunker	= CBNK_HOME;
	RenderMode		= CHAR_2D;		///By default, render is always in 2D
	Type			= CHARS_BUNKERS_MNGR;
}


void CBunkersManager::AssignSounds2Bunker(CBunker * B)
{
	unsigned int i, s;

	B->SetSoundsAmount(Sound.size());
	//Bunkers sound assignment
	for (s = 0; s < Sound.size(); s++)
		B->SetSound(Sound[s], s);

	//Bricks sounds assignment is performed by the bunker once it is updated as before
	B->AssignSounds2Bricks();
}

void CBunkersManager::NewBunker(Vector &Pos)	///pointer of available bunkers is updated to the next position
{
	Bunker[BunkerAvailable].Activate ();
	Bunker[BunkerAvailable].TranslateBunker(Pos);
}

/**
*	@fn void CBonusManager::SetLocalTimers(unsigned int Size)
*	Set the global timers to every bonus available and start up local ones
*/
void CBunkersManager::SetLocalTimers()
{
	unsigned int i;

	Timer.resize(CBM_MAX_TIMERS);
	ChangeSF(TimerManager.GetSF());

	for (i = 0; i < CBM_MAX_BUNKERS; i++)
		Bunker[i].SetLocalTimers(CBM_MAX_TIMERS);
}