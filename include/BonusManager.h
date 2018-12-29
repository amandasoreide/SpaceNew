/**	Definition of the class Bonus Manager

*	Prefix: CBM_

*	@author Ramon Molla
*	@version 2011-11
*/

#ifndef CBM_BONUSMANAGER
#define CBM_BONUSMANAGER

#include <Bonus.h>

#define CBM_BONUS_INITIAL_Y3D	SIGLBD_PG_CEILING+2.0f*CBN_HEIGTH_3D

class CBonusManager: public CExplosiveChar
{
	//Atributes
public:

	///Amount of bonuses the manager can take care of
	CBonus*			Bonus[CBN_MAX_BONUSES];
	CBN_BONUS_TYPE	NextBonus;

	RTDESK_TIME TimeRTdeskMsg;

	bool		EndByTime,
				EndByFrame,
				DiscreteSimulation;

	RTDESK_CMsg *msg;			//RTDESK Message Time
	RTDESK_CMsg *msgUpd;		//RTDESK Message Time

	CBonusManager();	///<Constructor of the class
	void Init();		///<Initialization by default method

	void SetQTRoot(QuadtreeRoot *QTR);

	void SetGameRef(CCharacter *Game);

	void SetLocalTimers();

	void AssignSounds2Bonus();
	void SetEndingCriteria(bool ByTime, bool ByFrame);
	void SetRenderPeriod(HRT_Timems RP);

	///Generates an event to all the bonuses active at a given moment in order to do something
	void Maintenance();
	void UpdateActiveBonus(void);

	void GenerateRandomBonus(); ///<Generates randomly the corresponding bonus. Only once a time

	inline void SetSound(CSound * sound, unsigned int Index) { Sound[Index] = sound; }

	RTDESK_TIME GenerateBonus(CBN_BONUS_TYPE BonusType);	///<Generates a new bonus for the player
	void DiscreteGenerateBonus(CBN_BONUS_TYPE BonusType);	///<Generates a new bonus for the player

	void Render				(void);	///<Shows the bonuses on the screen
	
	void ChangeRenderMode	(CHAR_RENDER_MODE);///Change the way all the bonuses are going to be rendered on the screen

	void ReceiveMessage(RTDESK_CMsg *pMsg);
};

#endif