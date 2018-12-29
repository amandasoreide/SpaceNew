/*Definition of the class CircleShip

Prefix: CCS_

@author Ramon Molla
@version 2016-02
*/

#include <Shoot.h>
#include <CircleShip.h>
#include <Navy.h>
#include <ShootsManager.h>	/// Header File class Manager for the shoots

#include <GameCharacters.h>
#include <ExecutionMode.h>
#include <GlobalDefs.h>

#define _USE_MATH_DEFINES 
#include <math.h>
#include <glext.h>

#define CCS_XSIZE2D 1.0f
#define CCS_YSIZE2D 0.6f

#define CCS_XSIZE3D 0.9f
#define CCS_YSIZE3D 0.5f
#define CCS_ZSIZE   0.0f

extern CNavy			*Navy;
extern CShootsManager	*ShootsManager;
extern CShootsManager	*ShootsManager;

//////////////////////////////////////////////////////////////////////////////////////
//
// Ship initialization
//
// These values are by default. They have to be overwritten in the initialization phase
// when reading the default values from the "initialization.html" file

UGKS_String CCS_StateName[CCS_MAXSTATE] =
{
	"DEFAULT",
	"BORN",
	"LIVING",
	"DYING",
	"DEAD"
};

///Sensitive transition names
UGKS_String CCS_NameTransition[CCS_MAXTRANSITION] = 
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

CCircleShip::CCircleShip()
{
	Mesh = NULL;

#ifdef CHAR_USE_AABB
	InitializeAABB();
#endif

	Init();
	
	/*msgUpd = new cMsgNavy;
	msgUpd->Type = UMSG_MSG_TYPE_NAVY;
	msgUpd->SubType = UMSG_UPDCSHIPS;
	msgUpd->Propietary = true;*/
}

void CCircleShip::InitTransforms()
{
	Scale.v[XDIM] = 0.25;
	Scale.v[YDIM] = 0.25;
	Scale.v[ZDIM] = 0.25;

	Rotation.v[XDIM] = 90.0;
	Rotation.v[YDIM] =  0.0;
	Rotation.v[ZDIM] =  0.0;
}

void CCircleShip::Init()
{
	Health = MaxHealth	=	CCS_MAX_HEALTH;
	Hit_duration		=	CCS_MAX_MAX_HIT_DUR;
	SubType				=	CCS_BOTHTYPE;
	Type				=	CHARS_CIRCLESHIP;

	Speed.v[XDIM]		=	0.002f;	//Units/ms

#ifdef CHAR_USE_AABB
	UpdateAABB(CCS_XSIZE3D, CCS_YSIZE3D, CCS_ZSIZE);			///Update by default the AABB relative to local coordinates
#endif

	Radius=2.0f;
	Center.Set(	0.0f,
				4.0f,
				SIGLBD_DEFAULT_Z_POSITION);
	Degrees=0;
	Velocity=0.01;
	ChgDir = false;
	UpdateSF(TimerManager.GetSF());
	ResetTransformations();
}

void CCircleShip::AI_Healthing()
{
	
	if	(Health < MaxHealth)
	{
		Timer[CCS_HEALTH_INC_TMR].EndCounting();
		Health += Timer[CCS_HEALTH_INC_TMR].GetLastPeriodms() * 0.0002f;
		Timer[CCS_HEALTH_INC_TMR].InitCounting();
	}
	else Health = MaxHealth;
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CircleShip movement
// @args[0] : CircleShip moving
//
//
void CCircleShip::AI_Move()
{
	HRT_Timems RenderPeriod;

	PositionPrev=Position;
	RenderPeriod = Timer[CCS_RND_PERIOD].GetAlarmPeriodms();

	if(ChgDir){
		//recalculate angle
		Degrees -= Velocity*RenderPeriod;
	}else{
		//recalculate angle
		Degrees += Velocity*RenderPeriod;
	}

	//Move
	MoveTo((sin(Degrees)*Radius)+Center.v[XDIM],(cos(Degrees)*Radius)+Center.v[YDIM],-0.000000001);
	//Choose if it must send a change turn event
	if (Degrees>2 * M_PI){
			ChgDir = true;
			Degrees=0;
	}
	else if (Degrees<-2 * M_PI){
			ChgDir = false;
			Degrees=0;
	}

	UpdateCollisionDetection();
}

///The circle ship is going to burst before being dead
void CCircleShip::AI_Die(void)
{
	///Changing internal attributes
	SetDefault();
	Explosion.Health = float((rand()%100)+ CEX_MAX_HEALTH);
	
	Collisionable(UGKPHY_NON_COLLISIONABLE);
	EntombMe();
}

///Nothinf at all has to be done. The circle ship is dead
void CCircleShip::AI_Death()
{
}

bool CCircleShip::OutEvent(CCS_CIRCLESHIP_TRANSITION EventName){ return AI->outEvent(CCS_NameTransition[EventName], NULL, this); }

//Physics
void CCircleShip::Collided (CCharacter *CollidedChar)
{
	HRT_Timems RenderPeriod;
	int typeAux;

	if (CHARS_COLLISION_TABLE[CHARS_CIRCLESHIP][CollidedChar->Type])
	{
		RenderPeriod = Timer[CCS_RND_PERIOD].Ticks2ms(Timer[CCS_RND_PERIOD].GetAlarmPeriodms());

		switch (CollidedChar->Type)
		{
		case CHARS_SHOOT:
			typeAux = ((CShoot*)CollidedChar)->SubType;

			if (CSH_PLAYER2D == typeAux || CSH_PLAYER3D == typeAux || CSH_PLAYER3D_CHEVRON == typeAux || CSH_AUX_LASER == typeAux)
			{
				Health -= CollidedChar->Health;
				if (Health <= 0)
				{
					Sound[CN_EXPLOSION_SND]->Play(UGKSND_VOLUME_80);
					AI_Die();
					Explosion.Init(this, RenderPeriod);
					Explosion.Activate();

					RTDESK_CMsg *Msg = GetMsgToFill(UMSG_MSG_BASIC_TYPE);
					SendMsg(Msg, Directory[CHARS_GAME_REF], RTDESKT_INMEDIATELY);
				}
			}
			break;
		case CHARS_SHIP:
			break;
		}
	}
}

//Rendering procedures

void CCircleShip::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;
	Explosion.ChangeRenderMode(Mode);

	switch(Mode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		break;
	case CHAR_2D:
		SetAABBInGlobalCoord(CCS_XSIZE2D, CCS_YSIZE2D, CCS_ZSIZE);
		break;
	case CHAR_LINES3D:
		break;
	case CHAR_3D:
		SetAABBInGlobalCoord(CCS_XSIZE3D,CCS_YSIZE3D,CCS_ZSIZE);
		FitMeshIntoBoundingBox();
		break;
	default: return;
	}
}

void CCircleShip::Render()
{
	HRT_Timems RenderPeriod = Timer[CCS_RND_PERIOD].GetAlarmPeriodms();
	GLboolean	Blending = glIsEnabled(GL_BLEND),
				Alpha = glIsEnabled(GL_ALPHA_TEST);

#ifdef XM_RND_TIME_DISC
	TimerManager.GetTimer(SIGLBT_RENDER_TIMING)->InitCounting();
#endif

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
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		((CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF])->Textures[IndTexture2D]->SetTexture();

		// ALPHA TEST + BLEND
		//glAlphaFunc(GL_GREATER, 0.4f);								// for TGA alpha test
		if (!Blending)	glEnable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);									// for TGA alpha test
		//if (!Alpha)		glEnable(GL_ALPHA_TEST);				// for TGA alpha test
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// for anti-aliasing
		//Render2D();

		// BACK TO NON ALPHA TEST + PREVIOUS BLEND
		if (!Blending)	glDisable(GL_BLEND);
		//if (!Alpha)		glDisable(GL_ALPHA_TEST);									// for TGA alpha test

		glDisable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		glEnable(GL_CULL_FACE);		// Back face culling set on
		glFrontFace(GL_CCW);		// The faces are defined counter clock wise
		glEnable(GL_DEPTH_TEST);	// Occlusion culling set on
	
		glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
		Mesh->modelo.pos.v = Position.v;
		Mesh->modelo.rot.v = Rotation.v;

		//Set the scale
		Mesh->modelo.scale.v = Scale.v;

		// SupplyShip touché
		if (Hit_duration < CCS_MAX_MAX_HIT_DUR)
		{
			Hit_duration -= 10.0 * RenderPeriod;
			if (Hit_duration <= 0) Hit_duration = CCS_MAX_MAX_HIT_DUR;

			Mesh->modelo.Draw();
		}
		
		// SupplyShip normal
		else
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

///Called everytime a time slot happens and its health has to be increased
void *CCS_FSM_Healthing	(LPSTR *args, CCircleShip *CircleShip)
{
	CircleShip->AI_Healthing();
	return NULL;
}

///Called everytime a time slot happens and a moving has to be done
void *CCS_FSM_Move		(LPSTR *args, CCircleShip *CircleShip)
{
	CircleShip->AI_Move();
	return NULL;
}

///Called when the circle ship is going to burst before dying
void *CCS_FSM_Dye	(LPSTR *args, CCircleShip *CircleShip)
{
	HRT_Timems RenderPeriod = CircleShip->Timer[CCS_RND_PERIOD].Ticks2ms(CircleShip->Timer[CCS_RND_PERIOD].GetAlarmPeriodms());

	// CircleShip dead
	CircleShip->Explosion.Init(CircleShip, RenderPeriod);
	CircleShip->AI_Die();

	RTDESK_CMsg *Msg = CircleShip->GetMsgToFill(UMSG_MSG_BASIC_TYPE);
	CircleShip->SendMsg(Msg, CircleShip->Directory[CHARS_GAME_REF], RTDESKT_INMEDIATELY);
	
	return NULL;
};

void CCircleShip::Update(void)
{
	double msUpdCShip;

#ifdef XM_UPD_TIME_CONT
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
#endif	

	if(Alive())
	{
		Vector P,	//For position
			   S;	//For Speed

		if (Timer[CCS_UPD_PERIOD].IsSounding())
		{
#ifndef XM_CONTINUOUS_WITH_SIMULATE_TIME
			Timer[CCS_UPD_PERIOD].ResetAlarm();

			//Next execution time calculation
			double auxX= abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value/Speed.v[XDIM]);
			double auxY= abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value/Speed.v[YDIM]);
			//double auxZ= abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value/Speed.v[ZDIM]);

			msUpdCShip = UGKALG_Min(auxX,auxY);

			if(msUpdCShip > SIGLBD_MAX_UPDATETIME) msUpdCShip = SIGLBD_MAX_UPDATETIME;
			Timer[CCS_UPD_PERIOD].SetAlarm(Timer[CCS_UPD_PERIOD].ms2Ticks(msUpdCShip));

			//Shooting calculation
			if (( floor((rand()%100000/(1+Navy->ShootsFrequency))/msUpdCShip)) == 1) ///Has the Supply ship to fire?
			{
				P.Set(Position.v[XDIM],
					  Position.v[YDIM] - .3f,
					  .05f);
				S.Set(0.0f,
					  -CCS_SHOOT_SPEED,
					  0.0f);

				if(Navy->WithShoots)
					ShootsManager->NewShoot(CSH_CIRCLE_SHIP, P, S);
			}
		
			//Move the circle ship
			OutEvent(CCS_MOVING);	//v 2->2
#else
			//Do not do an alarm reset. Account for a alarm period and 
			if (Timer[CCS_UPD_PERIOD].IsSounding()) Timer[CCS_UPD_PERIOD].AdvanceOneAlarmPeriod();

			//Next execution time calculation.(TicksToUpdateCircleShip)
			double auxX= abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value/Speed.v[XDIM]);
			double auxY= abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value/Speed.v[YDIM]);
			//double auxZ= abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value/Speed.v[ZDIM]);

			msUpdCShip = UGKALG_Min(auxX,auxY);

			if(msUpdCShip > SIGLBD_MAX_UPDATETIME) msUpdCShip = SIGLBD_MAX_UPDATETIME;
			Timer[CCS_UPD_PERIOD].SetAlarm(Timer[CCS_UPD_PERIOD].ms2Ticks(msUpdCShip));

			//Shooting calculation
			if (( floor((rand()%100000/(1+Navy->ShootsFrequency))/msUpdCShip)) == 1) ///Has the Supply ship to fire?
			{
				P.Set(	Position.v[XDIM],
						Position.v[YDIM] - .3f,
						.05f);
				S.Set(	0.0f,
						-CCS_SHOOT_SPEED,
						0.0f);

				if(Navy->WithShoots)
					ShootsManager->NewShoot(CSH_CIRCLE_SHIP, P, S);					
			}
		
			//Move the circle ship
			OutEvent(CCS_MOVING);	//v 2->2
			AI_Move();
#endif
		}
	}

	if (Explosion.Active())
		Explosion.Update();

#ifdef XM_UPD_TIME_CONT
	TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
#endif

}

void CCircleShip::DiscreteUpdate(void)
{
	double		msUpdCShip;
	
	if(Alive() && !EndByTime && !EndByFrame)
	{
		#ifdef XM_UPD_TIME_DISC
		TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
		#endif

		//Shooting calculation
		if ((floor((rand() % 100000 / (1 + Navy->ShootsFrequency)) / Timer[CCS_UPD_PERIOD].GetAlarmPeriodms())) == 1) ///Has the Supply ship to fire?
		{
			Vector P, S;
			P.Set(Position.v[XDIM],
				  Position.v[YDIM] - .3f,
				  .05f);
			S.Set(0.0f,
				  -CCS_SHOOT_SPEED,
				  0.0f);

			if(Navy->WithShoots)
				ShootsManager->NewShoot(CSH_CIRCLE_SHIP, P, S);			
		}
		
		//Move the circle ship
		OutEvent(CCS_MOVING);	//v 2->2

		double auxX= abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value/Speed.v[XDIM]);
		double auxY= abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value/Speed.v[YDIM]);
		//double auxZ= abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value/Speed.v[ZDIM]);

		msUpdCShip = UGKALG_Min(auxX, auxY);

		if(msUpdCShip > SIGLBD_MAX_UPDATETIME) msUpdCShip = SIGLBD_MAX_UPDATETIME;

		#ifdef XM_UPD_TIME_DISC
		TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
		#endif

		#ifdef DEF_RTD_TIME
		TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING].InitCounting();
		#endif

		msgUpd = (cMsgNavy*)GetMsgToFill(UMSG_MSG_NAVY);
		msgUpd->Type = UMSG_MSG_NAVY;
		msgUpd->SubType = UMSG_UPDCSHIPS;
		SendSelfMsg(msgUpd, TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING)->ms2Ticks(msUpdCShip));

		#ifdef DEF_RTD_TIME
		TimerManager.EndAccCounting(SIGLBT_RTDSKMM_TIMING);
		#endif
	}
}

void CCircleShip::ReceiveMessage(RTDESK_CMsg *pMsg){

	switch (pMsg->Type)
	{
	case UMSG_MSG_NAVY:
		cMsgNavy *auxMsg;
			auxMsg = (cMsgNavy*)pMsg;
			switch (auxMsg->SubType)
			{
			case UMSG_UPDCSHIPS:

				DiscreteUpdate();

				break;
			}
		break;
	case UMSG_MSG_BASIC_TYPE:

		DiscreteUpdate();

		break;
	}

}

void *CCS_FSM_Death		(LPSTR *args, CCircleShip *CircleShip)
{
	CircleShip->AI_Die();
	return NULL;
}

void *CCS_display(LPSTR *args, CCircleShip *CircleShip)
{
	CircleShip->Render();
	return NULL;
}
