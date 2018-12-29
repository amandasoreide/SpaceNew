/*	
	@file BunkersManager->h
	Definition of the class Bunkers Manager

	Prefix: CBM_

	@author Ramon Molla
	@version 2011-10
*/

#ifndef CBM_BUNKERSMNG
#define CBM_BUNKERSMNG

#include <Bunker.h>

//Definitions for the game
#define CBM_MAX_BUNKERS 6	//Amount of simultaneous Bunkers in the same screen in a given time

/** \typedef CBM_EXTRA_TIMERS
*	Types of different local timing managed by any player
*/
typedef enum {
	CBM_UPD_PERIOD,
	CBM_RND_PERIOD,
	CBM_MAX_TIMERS
} CBM_EXTRA_TIMERS;

class CBunkersManager: public CCharacter
{
	
public:

	//Methods
	void Init (void);

	CBunkersManager(){Init();}	///Constructor of the class
	void AssignSounds2Bunker(CBunker * B);

	void		NewBunker(Vector &Pos);	///Generates a new bunker
	inline void NewBunker() { Vector Pos = { 0.0, 0.0, 0.0 }; NewBunker(Pos); }

	/**
	*	@fn SetLocalTimers()
	*	Starts up all the local timers required to work during tipical phases update, render,...
	*/
	void SetLocalTimers();
};

#endif