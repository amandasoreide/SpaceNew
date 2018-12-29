/**	Definition of the class Bonus

*	Prefix: CBN_

*	HISTORY

*	@author Ramon Molla
*	@version 2016-02
*/

#include <SITexturesResources.h>
#include <UGKTexturesManager.h>
#include <GameSounds.h>
#include <GameCharacters.h>
#include <BonusManager.h>
#include <SIMessage.h>
#include <GlobalTiming.h>

#if defined(XM_UPD_TIME_DISC) || defined(XM_UPD_TIME_CONT) || defined(DEF_RTD_TIME)
#include <GlobalTiming.h>
#endif


/**
*	@fn CBonus::CBonus()
*	Constructor of the class
*/
CBonus::CBonus()
{
	Mesh = NULL;

	Init();

	msgUpd = new RTDESK_CMsg;
	msgUpd->Type = UMSG_MSG_BASIC_TYPE;
	msgUpd->Propietary = true;
}

/**
*	@fn CBonus::Init()
*	Initializer of the class
*/
void CBonus::Init()
{
	Explosion.Health	= 4;
	SetDefault();

	//Initial position
	Position.Set	(float (rand()%10) - 5.0f, CBN_YPOS, CBN_ZPOS);
	Rotation.Set	(180.0f, 0.0f, 0.0f);
	ResetScale		();

	//Speed is reset in the CCharacter class constructor
	Speed.v[YDIM] = -0.003f;	//Units per milisecond

	//Do not use Moveto method to avoid to be inserted in the collision detection facilities
	Shake1 = 1;
	Shake2 = 0;

	Alpha		=  CBN_MAX_ALPHA;
	FadingSpeed = -CBN_FADING_SPEED;

	Type		=	CHARS_BONUS;	///<Class of character is a Bonus
	SubType		=	CBN_BONUS_3D;	///<When initializing the type of bonus, the first time is 3D
}

void CBonus::Render(void)
{
	CTexturesManager * TextMngr = (CTexturesManager *) Directory[CHARS_TEXTURES_MNGR_REF];
	
	GLboolean	Blending = glIsEnabled(GL_BLEND),
				AlphaT	 = glIsEnabled(GL_ALPHA_TEST);

	unsigned int Texture;

	if (!Blending)	glEnable(GL_BLEND);
	if (!AlphaT)	glEnable(GL_ALPHA_TEST);									// for TGA alpha test

	glDisable(GL_LIGHTING);			//Activate lights on
									//Disable the players lighting
	//glEnable(SIGLB_PLAYER_LIGHT);
	//Enable the navy ships lighting
	//glEnable(SIGLB_SHIP_LIGHT);
	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glColor4f(0.5f, 1.0f, 0.6f, Alpha);

	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character. Nothing to do
		return;
	case CHAR_2D:
		if (CBN_BONUS_3D != SubType)
		{
			//Not possible to render in 2D mode a non CBN_BONUS_3D Bonus
			//Bonus not allowed. Destroy bonus
			SetDefault();
			return;
		}
		Texture = CTM_POWERUP;
		break;
	case CHAR_LINES3D:
		//Nothing to do at this version
		break;
	case CHAR_3D:
		switch (SubType)
		{
		case CBN_BONUS_3D:
			//Not possible to render in 3D mode a CBN_BONUS_3D Bonus
			//Bonus not allowed. Destroy bonus
			SetDefault();
			return;
		case CBN_BONUS_LASER:
			Texture = CTM_TEXT_BONUS;
			break;
		case CBN_BONUS_WEAPON:
			Texture = CTM_TEXT_WEAPON;
		}

		//Fading in and out of the Bonus
		Alpha += FadingSpeed * msUpdMsg;
		if (Alpha <= 0)
			if (FadingSpeed <=0 ) FadingSpeed = -FadingSpeed;
		else if (Alpha >= CBN_MAX_ALPHA)
			if (FadingSpeed >=0 ) FadingSpeed = -FadingSpeed;

		break;
	default: return;
	}
	glColor4f(0.5f, 1.0f, 0.6f, Alpha);
	TextMngr->Textures[Texture]->SetTexture();
	Render2D();

	if (!Blending)	glDisable(GL_BLEND);
	if (!AlphaT)	glDisable(GL_ALPHA_TEST);									// for TGA alpha test
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void CBonus::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;

	switch(Mode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		break;
	case CHAR_2D:
		SetAABBInGlobalCoord(CBN_WIDTH_2D, CBN_HEIGTH_2D, CHAR_2D_AABB_THICKNESS);
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		SetAABBInGlobalCoord(CBN_WIDTH_3D, CBN_HEIGTH_3D, CHAR_AABB_THICKNESS_DEFAULT);
		break;
	default: return;
	}	
}

void CBonus::Update (void)	///What the character has to do on every time tick 
{
	Vector DeltaPos;

	if (Timer[CBN_UPD_PERIOD].IsSounding())
	{
#ifndef XM_CONTINUOUS_WITH_SIMULATE_TIME
		Timer[CBN_UPD_PERIOD].ResetAlarm();
		//Move downwards one step
		DeltaPos = Speed * msUpdMsg;
		MoveRelTo(DeltaPos);// Move On The X Axis By X Speed

		if (Position.v[YDIM] < 	SIGLBD_PG_BOTTOM)						//Out of game playground
			AI_Die();

		#ifdef CHAR_USE_AABB
		UpdateAABB();
		#endif
#else
		if (Timer[CBN_UPD_PERIOD].IsSounding()) Timer[CBN_UPD_PERIOD].AdvanceOneAlarmPeriod();

		//Move downwards one step
		MoveRelTo(0.0f, Speed.v[YDIM] * msUpdMsg, 0.0f);
#endif
	}
}

void CBonus::DiscreteUpdate (void)	///What the character has to do on every time tick 
{
	Vector DeltaPos;

	#ifdef XM_UPD_TIME_DISC
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
	#endif

	//Move downwards one step
	DeltaPos = Speed * msUpdMsg;
	MoveRelTo(DeltaPos);// Move On The X Axis By X Speed

	if (Position.v[YDIM] < SIGLBD_PG_BOTTOM)						//Out of game playground
		AI_Die();

	#ifdef XM_UPD_TIME_DISC
	TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
	#endif


	if(Alive() && !EndByTime && !EndByFrame){
		#ifdef DEF_RTD_TIME
		TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING].InitCounting();
		#endif

		SendSelfMsg(msgUpd, Timer[CBN_UPD_PERIOD].GetAlarmPeriod());

		#ifdef DEF_RTD_TIME
		TimerManager.EndAccCounting(SIGLBT_RTDSKMM_TIMING);
		#endif
	}
}

void CBonus::AI_Explode (void)	//What to do when exploding a Bonus
{
	Explosion.Init(this, Explosion.Timer[CBN_UPD_PERIOD].GetAlarmPeriodms());
	Activate();
	AI_Die();
}

//Physics
void CBonus::Collided (CCharacter *CollidedChar)
{
	if (CHARS_COLLISION_TABLE[CHARS_BONUS][CollidedChar->Type])
		PlayerCollided();
}

void CBonus::PlayerCollided ()
{
	CSound * S;

	if (Alive() && !Explosion.Alive())
		AI_Explode();
	RTDESK_CMsg *Msg = GetMsgToFill(UMSG_MSG_BASIC_TYPE);
	SendMsg(Msg, Directory[CHARS_GAME_REF], RTDESKT_INMEDIATELY);

	//Make sound
	switch(SubType)
	{
	case CBN_BONUS_3D:
		S = GetSound(CBN_SHIELD_SND);
		S->Play(UGKSND_VOLUME_80);
		break;
	case CBN_BONUS_LASER:
	case CBN_BONUS_WEAPON:
		S = GetSound(CBN_BONUS_SND);
		S->Play(UGKSND_MAX_VOLUME);
		break;
	}
}

void CBonus::ReceiveMessage(RTDESK_CMsg *pMsg){
	DiscreteUpdate();
}
