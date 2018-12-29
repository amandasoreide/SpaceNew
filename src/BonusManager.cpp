/**
*	Bonus manager

*	Prefix: CBM_

*	@author Ramon Molla
*	@version 2011-11
*/

#include <BonusManager.h>
#include <RTDeskMsg.h>
#include <SIMessage.h>
#include <UGKCharactersPool.h>
#include <GameCharacters.h>
#include <GlobalDefs.h>
#include <GlobalTiming.h>

extern CCharactersPool	*CharacterPool;   ///< Pool of CCharacters

/////////////////////////////////////////////////
//
// Bonus manager Initialization
//

/**
*	@fn void CBonusManager::SetEndingCriteria(bool ByTime, bool ByFrame)
*	Set the conditions to finish the game
*/
void CBonusManager::SetEndingCriteria(bool ByTime, bool ByFrame)
{
	EndByTime	= ByTime;
	EndByFrame	= ByFrame;

	for (unsigned int i = 0; i < CBN_MAX_BONUSES; i++)
	{
		Bonus[i]->EndByTime	= ByTime;
		Bonus[i]->EndByFrame = ByFrame;
	}
}


void CBonusManager::SetLocalTimers()
{
	Timer.resize(CBN_MAX_TIMERS);
	UpdateSF(TimerManager.GetSF());	//The same as done in explosive char

	for (unsigned int i = 0; i < CBN_MAX_BONUSES; i++)
	{
		Bonus[i]->SetLocalTimers(CBN_MAX_TIMERS);
		//Set the sampling frequency tuple info into every timer of every bonus
		for (unsigned int t = 0; t < CBN_MAX_TIMERS; t++)
		{
			Bonus[i]->UpdateSF(TimerManager.GetSF());
			Bonus[i]->Timer[t].SetAlarm(Timer[t].GetAlarmPeriod());
		}
	}
}

/**
*	@fn void CBonusManager::Init ()
*	Used when all the values are initialized by default
*/

void CBonusManager::Init ()
{
	RenderMode	= CHAR_2D;
	NextBonus	= CBN_BONUS_3D;
	Type		= CHARS_BONUS_MNGR;

	for (unsigned int i = 0; i < CBN_MAX_BONUSES; i++)
	{
		Bonus[i] = (CBonus*)CharacterPool->get(CHARS_BONUS, UGKOBJM_NO_SUBTYPE);

		Bonus[i]->UpdateAABBInColDetect(CBN_WIDTH_2D, CBN_HEIGTH_2D, CBN_LENGTH_2D);

		//Set ending criteria
		Bonus[i]->EndByTime		= EndByTime;
		Bonus[i]->EndByFrame	= EndByFrame;
	}

	SetLocalTimers();

	Bonus[CBN_BONUS_3D]->SetGenPercentage(0.5);	//5 into one thousand 0.5%
	Bonus[CBN_BONUS_LASER]->SetGenPercentage(0.25);	//2.5 into one thousand 0.5%
	Bonus[CBN_BONUS_WEAPON]->SetGenPercentage(0.025);

	NextBonus = CBN_BONUS_3D;

	if (RTDM_NO_MSG == msgUpd)
		msgUpd = new RTDESK_CMsg;
	msgUpd->Type = UMSG_MSG_BASIC_TYPE;
	msgUpd->Propietary = true;
}


#ifdef CHAR_USE_QUADTREE
void CBonusManager::SetQTRoot(QuadtreeRoot *QTR)
{
	QTRoot = QTR;

	for (unsigned int i = 0; i < CBN_MAX_BONUSES; i++)
		Bonus[i]->SetQTRoot(QTR);
}
#elif CHAR_USE_AABB
#endif

CBonusManager::CBonusManager()
{
	unsigned int b;	//Bonuses iterator

	msgUpd = RTDM_NO_MSG;
	Init();
}

/**
*	@fn void CBonusManager::AssignSounds2Bonus()
*	Used when all the sounds are just loaded and assigned to the Bonus Manager
*/
void CBonusManager::AssignSounds2Bonus()
{
	for (unsigned int i = 0; i < CBN_MAX_BONUSES; i++)
	{
		Bonus[i]->SetSoundsAmount(Sound.size());
		for (unsigned s = 0; s < Sound.size(); s++)
			Bonus[i]->SetSound(Sound[s], s);
	}
}
	
/**
*	@fn void CBonusManager::Render(void)
*	Renders all the bonuses available
*/
void CBonusManager::Render(void)
{
	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		return;
	case CHAR_2D:
	case CHAR_LINES3D:
	case CHAR_3D:
		for (unsigned int b=0; b < CBN_MAX_BONUSES; b++){
			if(Bonus[b]->Alive())
				Bonus[b]->Render();
			if(Bonus[b]->Explosion.Alive()){
				Bonus[b]->Explosion.Update();
				Bonus[b]->Explosion.Render();
			}
		}
		break;
	default: return;
	}
}

/**
*	@fn void CBonusManager::ChangeRenderMode(CHAR_RENDER_MODE Mode)
*	Transfers the event to all shoots available
*/

void CBonusManager::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;

	for (unsigned int b=0; b < CBN_MAX_BONUSES; b++)
		Bonus[b]->ChangeRenderMode(Mode);
	
	switch (RenderMode)
	{
	case CHAR_2D:
		NextBonus = CBN_BONUS_3D;
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		NextBonus = CBN_BONUS_LASER;
		break;
	default:;
	}
}

///Generates randomly the corresponding bonus to the game status. Only once a time
void CBonusManager::GenerateRandomBonus()
{
	// BONUS -> RANDOM BONUS CREATION
	//It is only allowed to produce a CBN_BONUS_3D Bonus to pass to 3D mode
	if (!Bonus[NextBonus]->Active())
		//There is no bonus active
		if (rand() <= Bonus[NextBonus]->GenPercent)
			{
				if (DiscreteSimulation) DiscreteGenerateBonus(NextBonus);
				else GenerateBonus(NextBonus);
			}
}

void CBonusManager::SetGameRef(CCharacter *Game)
{
	unsigned int i;

	AssignTMG(Game);
	
	for (i = 0; i < CBN_MAX_BONUSES; i++)
		Bonus[i]->AssignTMG(Game);
}

///Shoots management for the Player and enemy navy
void CBonusManager::Maintenance()
{
	if (!Bonus[NextBonus]->Alive())
	{
		GenerateRandomBonus();
		return;
	}
}

void CBonusManager::UpdateActiveBonus(void)
{
	#if defined(XM_UPD_TIME_DISC) || defined(XM_UPD_TIME_CONT)
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
	#endif

	if (Bonus[NextBonus]->Alive())
		Bonus[NextBonus]->Update();

	#if defined(XM_UPD_TIME_DISC) || defined(XM_UPD_TIME_CONT)
	TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
	#endif
}

RTDESK_TIME CBonusManager::GenerateBonus(CBN_BONUS_TYPE BonusType)
{
	double		msUpdBonus;
	RTDESK_TIME ticksUpdBonus;

	Bonus[BonusType]->Type				= CHARS_BONUS;
	Bonus[BonusType]->SubType			= BonusType;
	Bonus[BonusType]->Activate();
	Bonus[BonusType]->MoveTo(0.0f, CBM_BONUS_INITIAL_Y3D, 0.0f);

	msUpdBonus = abs(Bonus[BonusType]->CharAABB.AABB[CHAR_BBSIZE][YDIM].Value/Bonus[BonusType]->Speed.v[YDIM]);
	if(msUpdBonus > SIGLBD_MAX_UPDATETIME) msUpdBonus = SIGLBD_MAX_UPDATETIME;
	Bonus[BonusType]->msUpdMsg	 = msUpdBonus;
	ticksUpdBonus = Timer[CBN_UPD_PERIOD].ms2Ticks(msUpdBonus);
	Bonus[BonusType]->UpdateSF(TimerManager.GetSF());
	Bonus[BonusType]->Timer[CBN_UPD_PERIOD].SetAlarm(ticksUpdBonus);
	return ticksUpdBonus;
}

void CBonusManager::DiscreteGenerateBonus(CBN_BONUS_TYPE BonusType)
{
	RTDESK_TIME ticksUpdBonus = GenerateBonus(BonusType);

	#ifdef DEF_RTD_TIME
	TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
	#endif

	#ifdef DEF_RTD_TIME
	TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING].InitCounting();
	#endif

	Bonus[BonusType]->SetMsgDispatcher(GetMsgDispatcher());
	msg = GetMsgToFill(UMSG_MSG_BASIC_TYPE);
	msg->Type = UMSG_MSG_BASIC_TYPE;
	SendMsg(msg,Bonus[BonusType],ticksUpdBonus);

	#ifdef DEF_RTD_TIME
	TimerManager.EndAccCounting(SIGLBT_RTDSKMM_TIMING);
	#endif

	#ifdef DEF_RTD_TIME
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
	#endif
}

/**
*	@fn void CBonusManager::SetRenderPeriod(HRT_Timems RP)
*	Set the rendering period to the bonus manager and all the bonuses
*/
void CBonusManager::SetRenderPeriod(HRT_Timems RP)
{
	Timer[CBN_RND_PERIOD].SetAlarm(RP);

	for (unsigned int i = 0; i < CBN_MAX_BONUSES; i++)
		Bonus[i]->Timer[CBN_UPD_PERIOD].SetAlarm(RP);
}

void CBonusManager::ReceiveMessage(RTDESK_CMsg *pMsg){
	//Maintenance();	 
}