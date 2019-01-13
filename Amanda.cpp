/** /////////////////////////////////////////////////////////////////////////////////////
//
// Amanda initialization
//
// These values are by default. They have to be overwritten in the initialization phase
// when reading the default values from the "initialization.html" file

	@author Ramon Molla
	@version 2011-10-11
*/

#include <Navy.h>
#include <atlstr.h>

#include <ShootsManager.h>	/// Header File class Manager for the shoots
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <Ship.h>
#include <Amanda.h>
#include <Shoot.h>
#include <GameCharacters.h>
#include <ExecutionMode.h>
#include <GlobalDefs.h>
#include <glext.h>
#include <iostream>
#include <random>

#define AMA_WIDTH	1.0F
#define AMA_HEIGHT	0.6F

extern CNavy			*Navy;			///<Singleton to save the general configuration of the enemy Navy
extern CShootsManager	*ShootsManager;

///Different name states for the FSMs that control the SS behaviour
UGKS_String AMA_NameState[AMA_MAXSTATE] =
{
	"DEFAULT",
	"BORN",
	"LIVING",
	"DYING",
	"DEAD"
};

///Sensitive transition names
UGKS_String AMA_NameTransition[AMA_MAXTRANSITION] =
{
	"DEFAULT",
	"BORNING",
	"GETTING_ALIVE",
	"MOVING",
	"DISPLAYING",
	"HEALTHING",
	"BURST",
	"DIE",
	"FINISHED",
	"RESURRECTING"
};

void CAmanda::InitTransforms()
{
	Scale.v[XDIM] = 0.3;
	Scale.v[YDIM] = 0.3;
	Scale.v[ZDIM] = 0.3;

	Rotation.v[XDIM] = 90.0;
	Rotation.v[YDIM] = 0.0;
	Rotation.v[ZDIM] = 0.0;
}

//Class initialization
void CAmanda::Init()
{
	Health = MaxHealth = AMA_MAX_HEALTH;
	Hit_duration = AMA_MAX_HIT_DURATION;
	SubType = AMA_SUPPLY_SHIP;
	Type = CHARS_AMANDA;

	shield = CA_SHIELD;
	specialattack = CA_SPECIALATTACK;
	teleport = CA_TELEPORT;
	threeattacks = CA_THREEATTACKS;

	Speed.v[XDIM] = 0.09f;	//Units/ms

	Explosion.Init(SIGLBD_MAX_UPDATETIME);
	Explosion.Health = CS_MAX_EXPLOSION_LIFE;

#ifdef CHAR_USE_AABB
	UpdateAABB(AMA_WIDTH, AMA_HEIGHT, 0.0f);
#endif
	ResetTransformations();
}

CAmanda::CAmanda()
{
	Init();

	/*msgUpd = new cMsgNavy;
	msgUpd->Type = UMSG_MSG_NAVY;
	msgUpd->SubType = UMSG_UPDSSHIPS;
	msgUpd->Propietary = true;*/
};


//void CAmanda::AI_Init(SIAI_AI_TYPE AIType) 
//{
//	switch(AIType)
//	{
//		case SIAI_Amanda_DEFAULT:
//			AI = AIManager.GetAI(SIAI_Amanda_DEFAULT);  ///<Asks the AIManager for the 'FSM' corresponding to the Amanda appropiately initialized.
//		break;
//		default:;
//	}//Behaviour switch
//}

///Argument means the amount of miliseconds spent during the last 10 frames/game iterations
void CAmanda::AI_Healthing()
{
	if (Health < MaxHealth)
		Health += Timer[AMA_RND_PERIOD].GetAlarmPeriodms()*0.0002f;
	else Health = MaxHealth;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Amanda movement
// @args[0] : Amanda moving
//
//
void CAmanda::AI_Move()
{
	PositionPrev = Position;

	// I make a random variable sum
	std::srand(std::time(nullptr)); // use current time as seed for random generator
	int random_variable = std::rand();
	
	int x = 1 + std::rand() / ((RAND_MAX + 1u) / 40);
	int a = x;

	if (x == 8 && teleport > 0) {

		teleport--;
		int telex = (1 + std::rand() / ((RAND_MAX + 1u) / 10)) - 5;
		int teley = (1 + std::rand() / ((RAND_MAX + 1u) / 5));
		Position.v[XDIM] = telex;
		Position.v[YDIM] = teley;
	}
	//Argument means the amount of miliseconds spent during the last 10 frames/game iterations
	//UGKPHY_EULER integrator. No acceleration taken into account
	MoveRelTo(Speed.v[XDIM] * Timer[AMA_UPD_PERIOD].GetAlarmPeriodms(), 0.0f, 0.0f);
	if (Position.v[XDIM] < -AMA_MAX_X_SHIFT)			// Change movement direction
	{
		//Infinite acceleration
		if (0 > Speed.v[XDIM]) Speed.v[XDIM] = -Speed.v[XDIM];
	}
	else if (Position.v[XDIM] > AMA_MAX_X_SHIFT)
		//Infinite acceleration
		if (0 < Speed.v[XDIM]) Speed.v[XDIM] = -Speed.v[XDIM];

	UpdateCollisionDetection();
}

///The supply ship is going to burst before being dead
void CAmanda::AI_Die(void)
{
	///Changing internal attributes
	SetDefault();
	Explosion.Health = float((rand() % 100) + AMA_MAX_HEALTH);
	Collisionable(UGKPHY_NON_COLLISIONABLE);
	EntombMe();
}

///Nothinf at all has to be done. The supply ship is dead
void CAmanda::AI_Death()
{
}

bool CAmanda::OutEvent(CA_AMANDA_TRANSITIONS EventName) { 
	std::string eventname = AMA_NameTransition[EventName];
	bool result = AI->outEvent(eventname, NULL, this);
	return result;
}

//Physics
void CAmanda::Collided(CCharacter *CollidedChar)
{
	int typeAux;

	if (CHARS_COLLISION_TABLE[CHARS_AMANDA][CollidedChar->Type])
		switch (CollidedChar->Type)
		{
		case CHARS_SHOOT:
			typeAux = ((CShoot*)CollidedChar)->SubType;

			if (CSH_PLAYER2D == typeAux || CSH_PLAYER3D == typeAux || CSH_PLAYER3D_CHEVRON == typeAux || CSH_AUX_LASER == typeAux)
			{
				Health -= CollidedChar->Health;
				if (Health <= 0)
				{
					if (Sound.size() > CN_EXPLOSION_SND) {
						Sound[CN_EXPLOSION_SND]->Play(UGKSND_VOLUME_80);
					}
					AI_Die();
					Explosion.Init(this, Timer[AMA_UPD_PERIOD].GetAlarmPeriodms());
					Explosion.Activate();

					RTDESK_CMsg *Msg = GetMsgToFill(UMSG_MSG_BASIC_TYPE);
					SendMsg(Msg, Directory[CHARS_GAME_REF], RTDESKT_INMEDIATELY);
				}

			}
		case CHARS_SHIP:
			break;
		}
}

//Rendering procedures

void CAmanda::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;
	Explosion.ChangeRenderMode(Mode);

	switch (Mode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		break;
	case CHAR_2D:
		CharAABB.AABB[CHAR_BBSIZE][XDIM].Value = 0.9f;
		CharAABB.AABB[CHAR_BBSIZE][YDIM].Value = 0.5f;
		CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value = 0.0f;
		break;
	case CHAR_LINES3D:
		break;
	case CHAR_3D:
		CharAABB.AABB[CHAR_BBSIZE][XDIM].Value = 0.9f;
		CharAABB.AABB[CHAR_BBSIZE][YDIM].Value = 0.5f;
		CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value = 0.0f;

#ifdef CHAR_USE_QUADTREE
		FitMeshIntoBoundingBox();
#endif
		break;
	default: return;
	}
}

void CAmanda::Render()
{
#ifdef XM_RND_TIME_DISC
	TimerManager.GetTimer(SIGLBT_RENDER_TIMING)->InitCounting();
#endif

	GLboolean Blending = glIsEnabled(GL_BLEND);

	if (!Alive())	//Although it is not active
		return;
	//Enable the navy ships lighting
	glEnable(GL_LIGHTING);			// activate lights on
									//Disable the players lighting
	glDisable(SIGLB_PLAYER_LIGHT);
	//Enable the navy ships lighting
	glEnable(SIGLB_SHIP_LIGHT);
	glEnable(SIGLB_SHIP2_LIGHT);

	switch (RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		break;
	case CHAR_2D:
		if (!Blending)
			glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		((CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF])->Textures[IndTexture2D]->SetTexture();

		// ALPHA TEST + BLEND
		glAlphaFunc(GL_GREATER, 0.4f);								// for TGA alpha test
		glEnable(GL_ALPHA_TEST);									// for TGA alpha test
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		//Render2D();

		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		glEnable(GL_CULL_FACE);		// Back face culling set on
		glFrontFace(GL_CCW);		// The faces are defined counter clock wise
		glEnable(GL_DEPTH_TEST);	// Occlusion culling set on

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		Mesh->modelo.pos.v = Position.v;
		Mesh->modelo.rot.v = Rotation.v;
		Mesh->modelo.scale.v = Scale.v;

		// Amanda touché
		if (Hit_duration < AMA_MAX_HIT_DURATION)
		{
			Hit_duration -= 10.0 * Timer[AMA_UPD_PERIOD].GetAlarmPeriodms();
			if (Hit_duration <= 0) Hit_duration = AMA_MAX_HIT_DURATION;
		}
		// Amanda normal
		Mesh->modelo.Draw();

		break;
	default: return;
	}

	if (Explosion.Active())
		Explosion.Render();

#ifdef XM_RND_TIME_DISC
	TimerManager.EndAccCounting(SIGLBT_RENDER_TIMING);
#endif

}

void CAmanda::Update(void)
{
	double msUpdSShip;

#ifdef XM_UPD_TIME_CONT
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
#endif
	if (Alive())
	{
		Vector P, S;

		if (Timer[AMA_UPD_PERIOD].IsSounding())
		{

#ifndef XM_CONTINUOUS_WITH_SIMULATE_TIME
			Timer[AMA_UPD_PERIOD]->ResetAlarm();

			//Next execution time calculation. (TicksToUpdateAmanda)
			double auxX = abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value / Speed.v[XDIM]);
			double auxY = abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value / Speed.v[YDIM]);
			//double auxZ= abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value/Speed.v[ZDIM]);	
			msUpdSShip = UGKALG_Min(auxX, auxY);
			if (msUpdSShip > SIGLBD_MAX_UPDATETIME) msUpdSShip = SIGLBD_MAX_UPDATETIME;
			Timer[AMA_UPD_PERIOD].SetAlarm(Timer[AMA_UPD_PERIOD].ms2Ticks(msUpdSShip));

			//Shooting calculation
			if ((floor((rand() % 100000 / (1 + Navy->ShootsFrequency)) / msUpdSShip)) == 1) ///Has the Supply ship to fire?
			{
				P.Set(Position.v[XDIM],
					Position.v[YDIM] - .3f,
					.05f);
				S.Set(0.0f,
					-AMA_SHOOT_SPEED,
					0.0f);

				if (Navy->WithShoots)
					ShootsManager->NewShoot(CSH_SUPPLY_SHIP, P, S);

			}

			//Move the supply ship
			OutEvent(AMA_MOVING);	//v 2->2
#else
			bool SynWithRealTime = false;
			if (Timer[AMA_UPD_PERIOD].IsSounding()) Timer[AMA_UPD_PERIOD].AdvanceOneAlarmPeriod();

			//Next execution time calculation. (TicksToUpdateAmanda)
			double auxX = abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value / Speed.v[XDIM]);
			double auxY = abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value / Speed.v[YDIM]);
			//double auxZ= abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value/Speed.v[ZDIM]);	

			msUpdSShip = UGKALG_Min(auxX, auxY);

			if (msUpdSShip > SIGLBD_MAX_UPDATETIME) msUpdSShip = SIGLBD_MAX_UPDATETIME;
			Timer[AMA_UPD_PERIOD].SetAlarm(Timer[AMA_UPD_PERIOD].ms2Ticks(msUpdSShip));

		
				//Shooting calculation
			if ((floor((rand() % 100000 / (1 + Navy->ShootsFrequency)) / msUpdSShip)) == 1) ///Has the Supply ship to fire?
			{
				P.Set(Position.v[XDIM],
					Position.v[YDIM] - .3f,
					.05f);
				S.Set(0.0f, -AMA_SHOOT_SPEED, 0.0f);
				// not sure about what this is
				if (Navy->WithShoots) {
					ShootsManager->NewShoot(CSH_AMANDA, P, S);
					// 3 attacks:
					int x = 1 + std::rand() / ((RAND_MAX + 1u) / 10);
					if (x == 1) {
						P.Set(Position.v[XDIM],
							Position.v[YDIM] - .3f,
							.05f);
						S.Set(-0.25f, -AMA_SHOOT_SPEED, 0.0f);
						ShootsManager->NewShoot(CSH_AMANDA, P, S);

						P.Set(Position.v[XDIM],
							Position.v[YDIM] - .3f,
							.05f);
						S.Set(0.25f, -AMA_SHOOT_SPEED, 0.0f);
						ShootsManager->NewShoot(CSH_AMANDA, P, S);
					}
				}
			}
			//Move the supply ship
			OutEvent(AMA_MOVING);	//v 2->2
			AI_Move();
#endif
			if (Explosion.Active())
				Explosion.Update();
		}
	}

#ifdef XM_UPD_TIME_CONT
	TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
#endif
}

void CAmanda::DiscreteUpdate(void)
{
	double msUpdSShip;

	if (Alive() && !EndByTime && !EndByFrame)
	{
#ifdef XM_UPD_TIME_DISC
		TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
#endif

		//Shooting calculation
		if ((floor((rand() % 100000 / (1 + Navy->ShootsFrequency)) / Timer[AMA_UPD_PERIOD].GetAlarmPeriodms())) == 1) ///Has the Supply ship to fire?
		{
			Vector P, S;

			P.Set(Position.v[XDIM],
				Position.v[YDIM] - .3f,
				.05f);
			S.Set(0.0f,
				-AMA_SHOOT_SPEED,
				0.0f);

			if (Navy->WithShoots)
				ShootsManager->NewShoot(CSH_AMANDA, P, S);
		}

		//Move the supply ship
		OutEvent(AMA_MOVING);	//v 2->2

		double auxX = abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value / Speed.v[XDIM]);
		double auxY = abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value / Speed.v[YDIM]);
		//double auxZ= abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value/Speed.v[ZDIM]);

		msUpdSShip = UGKALG_Min(auxX, auxY);

		if (msUpdSShip > SIGLBD_MAX_UPDATETIME) msUpdSShip = SIGLBD_MAX_UPDATETIME;

#ifdef XM_UPD_TIME_DISC
		TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
#endif
#ifdef DEF_RTD_TIME
		TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING].InitCounting();
#endif

		msgUpd = (cMsgNavy*)GetMsgToFill(UMSG_MSG_NAVY);
		msgUpd->Type = UMSG_MSG_NAVY;
		msgUpd->SubType = UMSG_UPDSSHIPS;
		SendSelfMsg(msgUpd, Timer[AMA_UPD_PERIOD].ms2Ticks(msUpdSShip));

#ifdef DEF_RTD_TIME
		TimerManager.EndAccCounting(SIGLBT_RTDSKMM_TIMING);
#endif
	}
}

void CAmanda::ReceiveMessage(RTDESK_CMsg *pMsg) {

	switch (pMsg->Type)
	{
	case UMSG_MSG_NAVY:
		cMsgNavy *auxMsg;
		auxMsg = (cMsgNavy*)pMsg;
		switch (auxMsg->SubType)
		{
		case UMSG_UPDSSHIPS:

			DiscreteUpdate();

			break;
		}
		break;
	case UMSG_MSG_BASIC_TYPE:

		DiscreteUpdate();

		break;
	}
}

///Called everytime a time slot happens and its health has to be increased
void *AMA_FSM_Healthing(LPSTR *args, CAmanda *Amanda)
{
	Amanda->AI_Healthing();
	return NULL;
}

///Called everytime a time slot happens and a moving has to be done
void *AMA_FSM_Move(LPSTR *args, CAmanda *Amanda)
{

	Amanda->AI_Move();

	return NULL;

}

///Called when the supply ship is going to burst before dying
void *AMA_FSM_Dye(LPSTR *args, CAmanda *Amanda)
{
	// Amanda dead
	Amanda->Explosion.Init(Amanda, Amanda->Timer[AMA_UPD_PERIOD].GetAlarmPeriodms());
	Amanda->AI_Die();

	RTDESK_CMsg *Msg = Amanda->GetMsgToFill(UMSG_MSG_BASIC_TYPE);
	Amanda->SendMsg(Msg, Amanda->Directory[CHARS_GAME_REF], RTDESKT_INMEDIATELY);

	return NULL;
};

void *AMA_FSM_Death(LPSTR *args, CAmanda *Amanda)
{
	Amanda->AI_Die();
	return NULL;
}

///Called when the supply ship passed from death to unborn states
void *AMA_FSM_Unborning(LPSTR *args, CAmanda *Amanda)
{
	Amanda->AI_Init();
	return NULL;
}

void *AMA_display(LPSTR *args, CAmanda *Amanda)
{
	Amanda->Render();
	return NULL;
}
