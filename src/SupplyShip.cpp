/** /////////////////////////////////////////////////////////////////////////////////////
//
// SupplyShip initialization
//
// These values are by default. They have to be overwritten in the initialization phase
// when reading the default values from the "initialization.html" file

	@author Ramon Molla
	@version 2011-10-11
*/

#include <Navy.h>

#include <ShootsManager.h>	/// Header File class Manager for the shoots

#include <Ship.h>
#include <SupplyShip.h>
#include <Shoot.h>
#include <GameCharacters.h>
#include <ExecutionMode.h>
#include <GlobalDefs.h>
#include <glext.h>

#define CSS_WIDTH	1.0F
#define CSS_HEIGHT	0.6F

extern CNavy			*Navy;			///<Singleton to save the general configuration of the enemy Navy
extern CShootsManager	*ShootsManager;

///Different name states for the FSMs that control the SS behaviour
UGKS_String CSS_NameState[CSS_MAXSTATE] = 
{
	"DEFAULT",
	"BORN",
	"LIVING",
	"DYING",
	"DEAD"
};

///Sensitive transition names
UGKS_String CSS_NameTransition[CSS_MAXTRANSITION] = 
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

void CSupplyShip::InitTransforms()
{
	Scale.v[XDIM] = 0.25;
	Scale.v[YDIM] = 0.25;
	Scale.v[ZDIM] = 0.25;

	Rotation.v[XDIM] = 90.0;
	Rotation.v[YDIM] = 0.0;
	Rotation.v[ZDIM] = 0.0;
}

//Class initialization
void CSupplyShip::Init()
{
	Health = MaxHealth	=	CSS_MAX_HEALTH;
	Hit_duration		=	CSS_MAX_HIT_DURATION;
	SubType				=	CSS_SUPPLY_SHIP;
	Type				=	CHARS_SUPPLYSHIP;
	
	Speed.v[XDIM]		=	0.002f;	//Units/ms

	Explosion.Init(SIGLBD_MAX_UPDATETIME);
	Explosion.Health	=	CS_MAX_EXPLOSION_LIFE;

#ifdef CHAR_USE_AABB
	UpdateAABB(CSS_WIDTH, CSS_HEIGHT, 0.0f);
#endif
	ResetTransformations();
}

CSupplyShip::CSupplyShip()
{
	Init(); 

	/*msgUpd = new cMsgNavy;
	msgUpd->Type = UMSG_MSG_NAVY;
	msgUpd->SubType = UMSG_UPDSSHIPS;
	msgUpd->Propietary = true;*/
};

//
//void CSupplyShip::AI_Init(SIAI_AI_TYPE AIType) 
//{
//	switch(AIType)
//	{
//		case SIAI_SUPPLYSHIP_DEFAULT:
//			AI = AIManager.GetAI(SIAI_SUPPLYSHIP_DEFAULT);  ///<Asks the AIManager for the 'FSM' corresponding to the SupplyShip appropiately initialized.
//		break;
//		default:;
//	}//Behaviour switch
//}

///Argument means the amount of miliseconds spent during the last 10 frames/game iterations
void CSupplyShip::AI_Healthing()
{	
	if	(Health < MaxHealth)
		Health += Timer[CSS_RND_PERIOD].GetAlarmPeriodms()*0.0002f;
	else Health = MaxHealth;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// SupplyShip movement
// @args[0] : SupplyShip moving
//
//
void CSupplyShip::AI_Move()
{
	PositionPrev=Position;
	//Argument means the amount of miliseconds spent during the last 10 frames/game iterations
	//UGKPHY_EULER integrator. No acceleration taken into account
	MoveRelTo(Speed.v[XDIM] * Timer[CSS_UPD_PERIOD].GetAlarmPeriodms(), 0.0f, 0.0f);
	if (Position.v[XDIM] < -CSS_MAX_X_SHIFT)			// Change movement direction
	{
		//Infinite acceleration
		if (0>Speed.v[XDIM]) Speed.v[XDIM] = -Speed.v[XDIM];
	}
	else if (Position.v[XDIM] > CSS_MAX_X_SHIFT)
		//Infinite acceleration
		if (0<Speed.v[XDIM]) Speed.v[XDIM] = -Speed.v[XDIM];

	UpdateCollisionDetection();
}

///The supply ship is going to burst before being dead
void CSupplyShip::AI_Die(void)
{
	///Changing internal attributes
	SetDefault();
	Explosion.Health = float((rand()%100)+ CSS_MAX_HEALTH);
	Collisionable(UGKPHY_NON_COLLISIONABLE);
	EntombMe();
}

///Nothinf at all has to be done. The supply ship is dead
void CSupplyShip::AI_Death()
{
}

bool CSupplyShip::OutEvent(CSS_SUPPLYSHIP_TRANSITIONS EventName){ return AI->outEvent(CSS_NameTransition[EventName], NULL, this); }

//Physics
void CSupplyShip::Collided (CCharacter *CollidedChar)
{
	int typeAux;

	if (CHARS_COLLISION_TABLE[CHARS_SUPPLYSHIP][CollidedChar->Type])
		switch (CollidedChar->Type)
		{
		case CHARS_SHOOT:
			typeAux = ((CShoot*)CollidedChar)->SubType;

			if (CSH_PLAYER2D==typeAux || CSH_PLAYER3D==typeAux || CSH_PLAYER3D_CHEVRON==typeAux ||CSH_AUX_LASER==typeAux)
			{
				Health -= CollidedChar->Health;
				if (Health <= 0)
				{
					Sound[CN_EXPLOSION_SND]->Play(UGKSND_VOLUME_80); 
					AI_Die();
					Explosion.Init(this, Timer[CSS_UPD_PERIOD].GetAlarmPeriodms());
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

void CSupplyShip::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;
	Explosion.ChangeRenderMode(Mode);

	switch(Mode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		break;
	case CHAR_2D:
		CharAABB.AABB[CHAR_BBSIZE][XDIM].Value			=	0.9f;
		CharAABB.AABB[CHAR_BBSIZE][YDIM].Value			=	0.5f;
		CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value			=	0.0f;
		break;
	case CHAR_LINES3D:
		break;
	case CHAR_3D:
		CharAABB.AABB[CHAR_BBSIZE][XDIM].Value			=	0.9f;
		CharAABB.AABB[CHAR_BBSIZE][YDIM].Value			=	0.5f;
		CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value			=	0.0f;

	#ifdef CHAR_USE_QUADTREE
		FitMeshIntoBoundingBox();
	#endif
		break;
	default: return;
	}	
}

void CSupplyShip::Render()
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

	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		break;
	case CHAR_2D:
		if (!Blending)
			glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		
		((CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF])->Textures[IndTexture2D]->SetTexture();

		// ALPHA TEST + BLEND
		glAlphaFunc(GL_GREATER, 0.4f);								// for TGA alpha test
		glEnable(GL_ALPHA_TEST);									// for TGA alpha test
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing

		//Render2D();

		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		glEnable(GL_CULL_FACE);		// Back face culling set on
		glFrontFace(GL_CCW);		// The faces are defined counter clock wise
		glEnable(GL_DEPTH_TEST);	// Occlusion culling set on

		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		Mesh->modelo.pos.v	 = Position.v;
		Mesh->modelo.rot.v	 = Rotation.v;
		Mesh->modelo.scale.v = Scale.v;

		// SupplyShip touché
		if (Hit_duration < CSS_MAX_HIT_DURATION)
		{
			Hit_duration -= 10.0 * Timer[CSS_UPD_PERIOD].GetAlarmPeriodms();
			if (Hit_duration <= 0) Hit_duration = CSS_MAX_HIT_DURATION;
		}		
		// SupplyShip normal
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

void CSupplyShip::Update(void)
{
	double msUpdSShip;

#ifdef XM_UPD_TIME_CONT
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
#endif
	if (Alive())
	{
		Vector P, S;
	
		if (Timer[CSS_UPD_PERIOD].IsSounding())
		{

#ifndef XM_CONTINUOUS_WITH_SIMULATE_TIME
			Timer[CSS_UPD_PERIOD]->ResetAlarm();

			//Next execution time calculation. (TicksToUpdateSupplyShip)
			double auxX= abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value/Speed.v[XDIM]);
			double auxY= abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value/Speed.v[YDIM]);
			//double auxZ= abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value/Speed.v[ZDIM]);	
			msUpdSShip = UGKALG_Min(auxX,auxY);
			if(msUpdSShip > SIGLBD_MAX_UPDATETIME) msUpdSShip = SIGLBD_MAX_UPDATETIME;
			Timer[CSS_UPD_PERIOD].SetAlarm(Timer[CSS_UPD_PERIOD].ms2Ticks(msUpdSShip));

			//Shooting calculation
			if (( floor((rand()%100000/(1+Navy->ShootsFrequency))/msUpdSShip)) == 1) ///Has the Supply ship to fire?
			{
				P.Set(Position.v[XDIM],
					  Position.v[YDIM] - .3f,
					  .05f);
				S.Set(0.0f,
					  -CSS_SHOOT_SPEED,
					  0.0f);

				if(Navy->WithShoots)
					ShootsManager->NewShoot(CSH_SUPPLY_SHIP, P, S);
				
			}

			//Move the supply ship
			OutEvent(CSS_MOVING);	//v 2->2
#else
			bool SynWithRealTime = false;
			if (Timer[CSS_UPD_PERIOD].IsSounding()) Timer[CSS_UPD_PERIOD].AdvanceOneAlarmPeriod();

			//Next execution time calculation. (TicksToUpdateSupplyShip)
			double auxX= abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value/Speed.v[XDIM]);
			double auxY= abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value/Speed.v[YDIM]);
			//double auxZ= abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value/Speed.v[ZDIM]);	

			msUpdSShip = UGKALG_Min(auxX, auxY);
			
			if(msUpdSShip > SIGLBD_MAX_UPDATETIME) msUpdSShip = SIGLBD_MAX_UPDATETIME;
			Timer[CSS_UPD_PERIOD].SetAlarm(Timer[CSS_UPD_PERIOD].ms2Ticks(msUpdSShip)); 

			//Shooting calculation
			if (( floor((rand()%100000/(1+Navy->ShootsFrequency))/msUpdSShip)) == 1) ///Has the Supply ship to fire?
			{
				P.Set(Position.v[XDIM],
						Position.v[YDIM] - .3f,
						.05f);
				S.Set(0.0f,
						-CSS_SHOOT_SPEED,
						0.0f);

				if(Navy->WithShoots)
					ShootsManager->NewShoot(CSH_SUPPLY_SHIP, P, S);
					
			}

			//Move the supply ship
			OutEvent(CSS_MOVING);	//v 2->2
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

void CSupplyShip::DiscreteUpdate(void)
{
	double msUpdSShip;

	if (Alive() && !EndByTime && !EndByFrame)
	{
		#ifdef XM_UPD_TIME_DISC
		TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
		#endif

		//Shooting calculation
		if ((floor((rand() % 100000 / (1 + Navy->ShootsFrequency)) / Timer[CSS_UPD_PERIOD].GetAlarmPeriodms())) == 1) ///Has the Supply ship to fire?
		{
			Vector P, S;

			P.Set(Position.v[XDIM],
				  Position.v[YDIM] - .3f,
				  .05f);
			S.Set(0.0f,
				  -CSS_SHOOT_SPEED,
				  0.0f);

			if(Navy->WithShoots)
				ShootsManager->NewShoot(CSH_SUPPLY_SHIP, P, S);			
		}

		//Move the supply ship
		OutEvent(CSS_MOVING);	//v 2->2
	
		double auxX= abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value/Speed.v[XDIM]);
		double auxY= abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value/Speed.v[YDIM]);
		//double auxZ= abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value/Speed.v[ZDIM]);

		msUpdSShip = UGKALG_Min(auxX, auxY);

		if(msUpdSShip > SIGLBD_MAX_UPDATETIME) msUpdSShip = SIGLBD_MAX_UPDATETIME;

		#ifdef XM_UPD_TIME_DISC
		TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
		#endif
		#ifdef DEF_RTD_TIME
		TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING].InitCounting();
		#endif
	
		msgUpd =(cMsgNavy*) GetMsgToFill(UMSG_MSG_NAVY);
		msgUpd->Type = UMSG_MSG_NAVY;
		msgUpd->SubType = UMSG_UPDSSHIPS;
		SendSelfMsg(msgUpd, Timer[CSS_UPD_PERIOD].ms2Ticks(msUpdSShip));

		#ifdef DEF_RTD_TIME
		TimerManager.EndAccCounting(SIGLBT_RTDSKMM_TIMING);
		#endif
	}
}

void CSupplyShip::ReceiveMessage(RTDESK_CMsg *pMsg) {

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
void *CSS_FSM_Healthing	(LPSTR *args, CSupplyShip *SupplyShip)
{
	SupplyShip->AI_Healthing();
	return NULL;
}

///Called everytime a time slot happens and a moving has to be done
void *CSS_FSM_Move		(LPSTR *args, CSupplyShip *SupplyShip)
{
	
	SupplyShip->AI_Move();
	
	return NULL;
	
}

///Called when the supply ship is going to burst before dying
void *CSS_FSM_Dye	(LPSTR *args, CSupplyShip *SupplyShip)
{
	// SupplyShip dead
	SupplyShip->Explosion.Init(SupplyShip, SupplyShip->Timer[CSS_UPD_PERIOD].GetAlarmPeriodms());
	SupplyShip->AI_Die();

	RTDESK_CMsg *Msg = SupplyShip->GetMsgToFill(UMSG_MSG_BASIC_TYPE);
	SupplyShip->SendMsg(Msg, SupplyShip->Directory[CHARS_GAME_REF], RTDESKT_INMEDIATELY);

	return NULL;
};

void *CSS_FSM_Death		(LPSTR *args, CSupplyShip *SupplyShip)
{
	SupplyShip->AI_Die();
	return NULL;
}

///Called when the supply ship passed from death to unborn states
void *CSS_FSM_Unborning (LPSTR *args, CSupplyShip *SupplyShip)
{
	SupplyShip->AI_Init();
	return NULL;
}

void *CSS_display(LPSTR *args, CSupplyShip *SupplyShip)
{
	SupplyShip->Render();
	return NULL;
}
