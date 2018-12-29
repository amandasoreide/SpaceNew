#include <AI\FSMParser.h>

#include <Navy.h>
#include <Ship.h>
#include <Amanda.h>
#include "atlstr.h"

#include <SIGame.h>
#include <TextureAnimationManager.h>
#include <ShootsManager.h>	/// Header File class Manager for the shoots
#include <glext.h>
#include <iostream>

#define CA_XSIZE 1.0f
#define CA_YSIZE 1.0f
#define CA_ZSIZE 0.0f

#define CA_XSIZE3D 0.4f
#define CA_YSIZE3D 0.5f
#define CA_ZSIZE3D 0.0f

extern CNavy					*Navy;			///<Singleton to save the general configuration of the enemy Navy
extern CTextureAnimationManager	AnimationsManager;
extern CShootsManager			*ShootsManager;

//////////////////////////////////////////////////////////////////////////////////////
//
// Ship initialization
//
// These values are by default. They have to be overwritten in the initialization phase
// when reading the default values from the "initialization.html" file

UGKS_String CA_StateName[CA_MAXSTATE] =
{
	"UNBORN",
	"BORN",
	"LIVING",
	"BURSTING",
	"DEAD"
};

///Sensitive transition names
UGKS_String CA_NameTransition[CA_MAXTRANSITION] =
{
	"DEFAULT",
	"BORNING",
	"GROWING",
	"MOVING",
	"DISPLAYING",
	"BURST",
	"REBORN",
	"DYING"
};

void CAmanda::Init()
{
	Health = MaxHealth = CA_MAX_HEALTH;
	Hit_duration = CA_MAX_MAX_HIT_DUR;
	Type = CHARS_AMANDA;
	SubType = CA_HIGH1;

	shield = CA_SHIELD;
	specialattack = CA_SPECIALATTACK;
	teleport = CA_TELEPORT;
	threeattacks = CA_THREEATTACKS;

	Speed.v[XDIM] = 0.015f;	//Units/ms

#ifdef CHAR_USE_AABB
	UpdateAABB(CA_XSIZE, CA_YSIZE, CA_ZSIZE);
#endif

	zi = 0.0f;
	zi_speed = (float)(rand() % 8 + 3) * 0.001f;
	Explosion.Health = CA_MAX_EXPLOSION_LIFE;

	Bounce = false;

	InitTransforms2D();

	UpdateSF(TimerManager.GetSF());

	Position.Set(0.0, 3, 0.0);
}

void CAmanda::UpdateRadius()
{
	float	AuxRadiusX,
		AuxRadiusY;

	AuxRadiusX = Scale.v[XDIM] * CharAABB.AABB[CHAR_BBSIZE][XDIM].Value;
	AuxRadiusY = Scale.v[YDIM] * CharAABB.AABB[CHAR_BBSIZE][YDIM].Value;

	Radius = sqrt(AuxRadiusX * AuxRadiusX + AuxRadiusY * AuxRadiusY);
	Radius *= 0.5f;
}

void CAmanda::InitTransforms2D()
{
	Rotation.v.x = 0.0;
	Rotation.v.y = 0.0;
	Rotation.v.z = 180.0;

	Scale.v[XDIM] = 0.8;
	Scale.v[YDIM] = 0.8;
	Scale.v[ZDIM] = 0.8;

	std::cout << "Running InitTransforms2D";

	UpdateRadius();
}

void CAmanda::InitTransforms3D()
{
	Rotation.v.x = 90.0;
	Rotation.v.y = 0.0;
	Rotation.v.z = 0.0;

	Scale.v[XDIM] = 1.5;
	Scale.v[YDIM] = 1.5;
	Scale.v[ZDIM] = 1.5;

	std::cout << "Running InitTransforms3D";

	UpdateRadius();
}

/**
* Destroys the 'Ship' object.
* The actions for destroying the Ship are performed through to the base class 'CCharacter'.
*/
CAmanda::~CAmanda()
{

};

/*void CAmanda::AI_Move(void)
{
	float DeltaTime;
	Vector DeltaPos;

	if (Bounce) {
		Speed.v[XDIM] *= -1;
		if (Navy->ShipsMoveDown)
			MoveRelTo(0.0f, CN_DOWN_STEP, 0.0f);	// Descending a bit down to the player

		Collisionable(UGKPHY_NON_COLLISIONABLE);
		Bounce = false;
	}

	PositionPrev = Position;

	if (((CSIGame*)Directory[CHARS_GAME_REF])->BouncingMode) {
		DeltaTime = Timer[CA_UPD_PERIOD].GetAlarmPeriodms();
		DeltaPos = Speed * DeltaTime;
		MoveRelTo(DeltaPos);	///Move the ship in X and Y dimension
		if (Position.v[XDIM] > SIGLBD_PG_RIGHT) {
			Speed.v[XDIM] = -Speed.v[XDIM];
			Position.v[XDIM] = SIGLBD_PG_RIGHT;
		}
		if (Position.v[XDIM] < SIGLBD_PG_LEFT) {
			Speed.v[XDIM] = -Speed.v[XDIM];
			Position.v[XDIM] = SIGLBD_PG_LEFT;
		}
		if (Position.v[YDIM] > SIGLBD_PG_SHIPS_CEILING) {
			Speed.v[YDIM] = -Speed.v[YDIM];
			Position.v[YDIM] = SIGLBD_PG_SHIPS_CEILING;
		}
		if (Position.v[YDIM] < SIGLBD_PG_SHIPS_BOTTOM) {
			Speed.v[YDIM] = -Speed.v[YDIM];
			Position.v[YDIM] = SIGLBD_PG_SHIPS_BOTTOM;
		}
	}
	else {
		MoveRelTo(Speed.v[XDIM] * Timer[CA_UPD_PERIOD].GetAlarmPeriodms(), 0.0f, 0.0f);	///Move the ship in X and Y dimension
		if ((Position.v[XDIM] > SIGLBD_PG_RIGHT) || (Position.v[XDIM] < SIGLBD_PG_LEFT)) {
			Navy->Bounce = true;
			Sound[CN_DESCEND_SND]->Play(UGKSND_VOLUME_80);
		}
	}

	UpdateCollisionDetection();
}*/

void CAmanda::AI_Die(void)
{
	SetDefault();

	Collisionable(UGKPHY_NON_COLLISIONABLE);
	EntombMe();

}

bool CAmanda::OutEvent(CA_AMANDA_TRANSITION EventName) {
	if (NULL != AI)
		return AI->outEvent(CA_NameTransition[EventName], NULL, this);
	else return false;
}

///PhysiCA methods
void CAmanda::Collided(CCharacter *CollidedChar)
{
	RTDESK_CMsg *Msg;
	HRT_Timems RenderPeriod = Timer[CA_RND_PERIOD].GetAlarmPeriodms();

	if (CHARS_COLLISION_TABLE[CHARS_SHIP][CollidedChar->Type])
		switch (CollidedChar->Type)
		{
		case CHARS_SHOOT:
			switch (((CShoot*)CollidedChar)->SubType)
			{
			case CSH_PLAYER2D:
			case CSH_PLAYER3D:
			case CSH_AUX_LASER:
			case CSH_PLAYER3D_CHEVRON:
				Health -= CollidedChar->Health;

				Hit_duration--;

				if (Health <= 0)
				{
					Sound[CN_EXPLOSION_SND]->Play(UGKSND_VOLUME_80);
					Explosion.Init(this, RenderPeriod);
					Explosion.Activate();
					//OutEvent(CA_BURST); //v 2->3

					RTDESK_CMsg *Msg = GetMsgToFill(UMSG_MSG_BASIC_TYPE);
					SendMsg(Msg, Directory[CHARS_GAME_REF], RTDESKT_INMEDIATELY);

					//OutEvent(CA_DIE); //v 3->4
					AI_Die();
					//OutEvent(CA_REBORN); //v 3->1
				}
			default:;
			}
			break;
		case CHARS_PLAYER:
			AI_Die();
			Explosion.Init(this, RenderPeriod);

			Msg = GetMsgToFill(UMSG_MSG_BASIC_TYPE);
			SendMsg(Msg, Directory[CHARS_GAME_REF], RTDESKT_INMEDIATELY);
			break;
		case CHARS_SHIP:
			// change to A here?
			if (((CSIGame*)Directory[CHARS_GAME_REF])->BouncingMode)
			{
				CAmanda *other = (CAmanda*)CollidedChar;

				GLfloat dx = Position.v[XDIM] - other->Position.v[XDIM];
				GLfloat dy = Position.v[YDIM] - other->Position.v[YDIM];
				//GLfloat dz = Position.v[ZDIM] - other->Position.v[ZDIM];

				GLfloat distToBall = dx * dx + dy * dy;// + dz*dz;

				if (distToBall <= (Radius + other->Radius)*(Radius + other->Radius))
				{
					GLfloat normal[3];
					normal[XDIM] = other->Position.v[XDIM] - Position.v[XDIM];
					normal[YDIM] = other->Position.v[YDIM] - Position.v[YDIM];
					//normal[2] = other->Position.v[ZDIM] - Position.v[ZDIM];

					GLfloat aux[3];
					aux[XDIM] = Speed.v[XDIM] - other->Speed.v[XDIM];
					aux[YDIM] = Speed.v[YDIM] - other->Speed.v[YDIM];
					//aux[2] = Speed.v[ZDIM] - other->Speed.v[ZDIM];

					GLfloat paralelo[3];
					GLfloat dotProd = aux[XDIM] * normal[XDIM] + aux[YDIM] * normal[1];// + aux[2]*normal[2];
					GLfloat length = normal[XDIM] * normal[XDIM] + normal[YDIM] * normal[YDIM];// + normal[2]*normal[2];
					GLfloat dot = dotProd / length;
					paralelo[XDIM] = normal[XDIM] * dot;
					paralelo[YDIM] = normal[YDIM] * dot;
					//paralelo[ZDIM]=normal[ZDIM]*dot;

					GLfloat perpen[3];
					perpen[XDIM] = aux[XDIM] - paralelo[XDIM];
					perpen[YDIM] = aux[YDIM] - paralelo[YDIM];
					//perpen[ZDIM] = aux[ZDIM] - paralelo[ZDIM];

					Speed.v[XDIM] = perpen[XDIM] + other->Speed.v[XDIM];
					Speed.v[YDIM] = perpen[YDIM] + other->Speed.v[YDIM];
					//Speed.v[ZDIM] = perpen[2] + other->Speed.v[ZDIM];
					other->Speed.v[XDIM] = paralelo[XDIM] + other->Speed.v[XDIM];
					other->Speed.v[YDIM] = paralelo[YDIM] + other->Speed.v[YDIM];
					//other->Speed.v[ZDIM] = paralelo[ZDIM] + other->Speed.v[ZDIM];
				}
			}
			break;
		default:;
		}
}

//Displays the ship while hit
void CAmanda::DisplayHit()
{
	TimerManager.GetTimer(SIGLBT_APP_TIMING)->EndCounting();
	Hit_duration -= 10 * TimerManager.GetTimer(SIGLBT_APP_TIMING)->GetLastPeriodms();
	if (Hit_duration <= 0)
		Hit_duration = CA_MAX_MAX_HIT_DUR;

	Mesh->modelo.Draw();	// ship not textured, white
}

//Renders a ships in 3D or 2D way

void CAmanda::Render2() {

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
	// not
	glEnable(GL_LIGHTING);			// activate lights on

									//not									//Disable the players lighting
	glDisable(SIGLB_PLAYER_LIGHT);
	//Enable the navy ships lighting
	//not
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

		AnimationsManager.Animations[IndAnimation2D]->UpdateAnimation();
		AnimationsManager.Animations[IndAnimation2D]->SetCurrentPhotogram();

		Render2D();

		if (!Blending)	glDisable(GL_BLEND);
		break;
	case CHAR_LINES3D:
	case CHAR_3D:
		glEnable(GL_CULL_FACE);		// Back face culling set on
		glFrontFace(GL_CCW);		// The faces are defined counter clock wise
		glEnable(GL_DEPTH_TEST);	// Occlusion culling set on

									// The ship 'floats'
		zi += zi_speed * Timer[CA_UPD_PERIOD].GetAlarmPeriodms() / 15;


		switch (SubType)
		{
		case CA_HIGH1:
			Color3D.r = Color3D.g = Color3D.b = .5f;
			break;
		case CA_HIGH2:
			Color3D.r = Color3D.g = Color3D.b = .6f;
			break;
		case CA_HIGH3:
			Color3D.r = Color3D.g = Color3D.b = .7f;
			break;
		case CA_MIDDLE:
			Color3D.r = Color3D.g = Color3D.b = .85f;
			break;
		case CA_LOW:
		default:
			Color3D.r = Color3D.g = Color3D.b = 1.0f;
		}

		Color3D.a = 1.0f;
		// if ship is being touched...
		if (Hit_duration >= CA_MAX_MAX_HIT_DUR)
			glColor4f(Color3D.r, Color3D.g, Color3D.b, Color3D.a);

		Render3D();

		break;
	default: return;
	}

	if (Explosion.Active())
		Explosion.Render();

#ifdef XM_RND_TIME_DISC
	TimerManager.EndAccCounting(SIGLBT_RENDER_TIMING);
#endif
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Displays a ship 3DS
//
//
void CAmanda::Render3D()
{
	Mesh->modelo.rot.v = Rotation.v;

	Mesh->modelo.pos.x = Position.v[XDIM];
	Mesh->modelo.pos.y = Position.v[YDIM];
	Mesh->modelo.pos.z = cosf(zi * 4) / 15;

	Mesh->modelo.scale.v = Scale.v;

	// affiche le ship touché
	if (Hit_duration < CA_MAX_MAX_HIT_DUR)
		DisplayHit();
	else
		Mesh->modelo.Draw();

}

void CAmanda::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	RenderMode = Mode;
	Explosion.ChangeRenderMode(Mode);

	switch (RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		break;
	case CHAR_2D:
		InitTransforms2D();
		break;
	case CHAR_LINES3D:
		InitTransforms3D();
		break;
	case CHAR_3D:
		InitTransforms3D();
		FitMeshIntoBoundingBox();
		break;
	default: return;
	}
}

void CAmanda::Update(void)
{
	/*std::stringstream ss;
	ss << "Shield Amanda: " << shield;
	CString msg = UGKS_string2CString(ss.str());
	MessageBox(NULL, msg, L"Frank rendered the first frame", MB_OK);*/

	double msUpdShip;

	if (Alive())					///If the Ship is alive
	{
		if (Timer[CA_UPD_PERIOD].IsSounding())
		{
			if (Explosion.Active())
				Explosion.Update();

#ifndef XM_CONTINUOUS_WITH_SIMULATE_TIME
			LastUpdTime = nowTime;

			//Next execution time calculation.
			double auxX = abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value / Speed.v[XDIM]);
			double auxY = abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value / Speed.v[YDIM]);
			double auxZ = abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value / Speed.v[ZDIM]);
			msUpdShip = sqrt((auxX*auxX) + (auxY*auxY));//+(auxZ*auxZ));
			if (msUpdShip > SIGLBD_MAX_UPDATETIME) msUpdShip = SIGLBD_MAX_UPDATETIME;
			Timer[CA_UPD_PERIOD].SetAlarm(Timer[CA_UPD_PERIOD].ms2Ticks(msUpdCAhip));

			//Shooting calculation
			if ((floor((rand() % 100000 / (1 + Navy->ShootsFrequency)) / msUpdShip)) == 1) ///Has the Supply ship to fire?
			{
				Vector P, S;

				P.Set(Position.v[XDIM],
					Position.v[YDIM] - .3f,
					.05f);
				S.Set(0.0f,
					-CA_SHOOT_SPEED,
					0.0f);

				if (Navy->WithShoots)
					ShootsManager->NewShoot(CSH_SHIP, P, S);
			}

			OutEvent(CA_MOVING); //v 2->2

								 /*if(((CAIGame*)Directory[CHARS_GAME_REF])->BouncingMode){
								 for(unsigned int i=numId+1;i<Navy->Ship.size();i++)
								 if(Navy->Ship[i]->Alive()) Collided(Navy->Ship[i]);*/
		}
#else

			//Next execution time calculation.
			double auxX = abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value / Speed.v[XDIM]);
			double auxY = abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value / Speed.v[YDIM]);
			msUpdShip = sqrt((auxX*auxX) + (auxY*auxY));
			if (msUpdShip > SIGLBD_MAX_UPDATETIME) msUpdShip = SIGLBD_MAX_UPDATETIME;
			Timer[CA_UPD_PERIOD].ResetAlarm();

			//Shooting calculation
			if ((floor((rand() % 100000 / (1 + Navy->ShootsFrequency)) / msUpdShip)) == 1) ///Has the Supply ship to fire?
			{
				Vector P, S;

				P.Set(Position.v[XDIM],
					Position.v[YDIM] - .3f,
					.05f);
				S.Set(0.0f,
					-CA_SHOOT_SPEED,
					0.0f);

				if (Navy->WithShoots)
					ShootsManager->NewShoot(CSH_SHIP, P, S);
			}

			//OutEvent(CA_MOVING); //v 2->2
			//AI_Move();

			//if (((CAIGame*)Directory[CHARS_GAME_REF])->BouncingMode){
			//	for(unsigned int i=numId+1;i<Navy->Ship.size();i++)
			//		if(Navy->Ship[i]->Alive()) Collided(Navy->Ship[i]);
#endif
	}
}
}

void CAmanda::DiscreteUpdate(void)
{
	double msUpdShip;

	if (Alive() && !EndByTime && !EndByFrame)					///If the Ship is alive
	{
#ifdef XM_UPD_TIME_DISC
		TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
#endif	
		if (Bounce) {
			Speed.v[XDIM] *= -1;
			if (Navy->ShipsMoveDown)
				MoveRelTo(0.0f, CN_DOWN_STEP, 0.0f);	// Descending a bit down to the player

			Collisionable(UGKPHY_NON_COLLISIONABLE);
			Bounce = false;
		}

		msUpdShip = Timer[CA_UPD_PERIOD].GetAlarmPeriodms();

		//Shooting calculation
		if ((floor((rand() % 100000 / (1 + Navy->ShootsFrequency)) / msUpdShip)) == 1) ///Has the Supply ship to fire?
		{
			Vector P, S;

			P.Set(Position.v[XDIM],
				Position.v[YDIM] - .3f,
				.05f);
			S.Set(0.0f,
				-CA_SHOOT_SPEED,
				0.0f);

			if (Navy->WithShoots)
				ShootsManager->NewShoot(CSH_SHIP, P, S);

		}

		PositionPrev = Position;

		if (((CSIGame*)Directory[CHARS_GAME_REF])->BouncingMode)
		{
			//for(unsigned int i=numId+1;i<Navy->Ship.size();i++)
			//	if(Navy->Ship[i]->Alive()) 
			//		Collided(Navy->Ship[i]);
			MoveRelTo(Speed.v[XDIM] * msUpdShip, Speed.v[YDIM] * msUpdShip, 0.0f);	///Move the ship in X dimension

			if (Position.v[XDIM] > SIGLBD_PG_RIGHT) {
				Speed.v[XDIM] *= -1;
				Position.v[XDIM] = SIGLBD_PG_RIGHT - Radius;
			}
			if (Position.v[XDIM] < SIGLBD_PG_LEFT) {
				Speed.v[XDIM] *= -1;
				Position.v[XDIM] = SIGLBD_PG_LEFT + Radius;
			}
			if (Position.v[YDIM] > SIGLBD_PG_SHIPS_CEILING) {
				Speed.v[YDIM] *= -1;
				Position.v[YDIM] = SIGLBD_PG_SHIPS_CEILING - Radius;
			}
			if (Position.v[YDIM] < SIGLBD_PG_SHIPS_BOTTOM) {
				Speed.v[YDIM] *= -1;
				Position.v[YDIM] = SIGLBD_PG_SHIPS_BOTTOM + Radius;
			}
		}
		else {
			MoveRelTo(Speed.v[XDIM] * msUpdShip, 0.0f, 0.0f);	///Move the ship in X dimension		
			if (Position.v[XDIM] > SIGLBD_PG_RIGHT || Position.v[XDIM] < SIGLBD_PG_LEFT) {
#ifdef XM_UPD_TIME_DISC
				TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
#endif
#ifdef DEF_RTD_TIME
				TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING].InitCounting();
#endif

				cMsgNavy *msg2Navy = (cMsgNavy*)GetMsgToFill(UMSG_MSG_NAVY);
				msg2Navy->SubType = UMSG_TURNALL;
				SendMsg(msg2Navy, Navy, Timer[CA_UPD_PERIOD].GetAlarmPeriod() / 4);

#ifdef DEF_RTD_TIME
				TimerManager.EndAccCounting(SIGLBT_RTDSKMM_TIMING);
#endif
			}
		}

		UpdateCollisionDetection();

		double auxX = abs(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value / Speed.v[XDIM]);
		double auxY = abs(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value / Speed.v[YDIM]);
		//double auxZ= abs(CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value/Speed.v[ZDIM]);

		msUpdShip = UGKALG_Min(auxX, auxY);

		if (msUpdShip > SIGLBD_MAX_UPDATETIME) msUpdShip = SIGLBD_MAX_UPDATETIME;
		Timer[CA_UPD_PERIOD].SetAlarm(Timer[CA_UPD_PERIOD].ms2Ticks(msUpdShip));

#ifdef XM_UPD_TIME_DISC
		TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
#endif
#ifdef DEF_RTD_TIME
		TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING].InitCounting();
#endif

		msgUpd = (cMsgNavy*)GetMsgToFill(UMSG_MSG_NAVY);
		msgUpd->Type = UMSG_MSG_NAVY;
		msgUpd->SubType = UMSG_UPDSHIPS;
		SendSelfMsg(msgUpd, Timer[CA_UPD_PERIOD].GetAlarmPeriod());

#ifdef DEF_RTD_TIME
		TimerManager.EndAccCounting(SIGLBT_RTDSKMM_TIMING);
#endif
	}
}

void CAmanda::ReceiveMessage(RTDESK_CMsg *pMsg) {

	switch (pMsg->Type)
	{
	case UMSG_MSG_NAVY:
		cMsgNavy * auxMsg;
		auxMsg = (cMsgNavy*)pMsg;
		switch (auxMsg->SubType)
		{
		case UMSG_UPDSHIPS:

			DiscreteUpdate();

			break;
		case UMSG_TURNALL:
			Bounce = true;
			break;
		}
		break;
	case UMSG_MSG_BASIC_TYPE:

		DiscreteUpdate();

		break;
	}
}

void *init_amanda(LPSTR *args, CAmanda *Amanda)
{
	///This is the room for may be whatever additional actions to perform this current ship initialization
	return NULL;
}

void *CA_Move(LPSTR *args, CAmanda *Amanda)
{
	//Amanda->AI_Move();
	return NULL;
}

void *CA_Display(LPSTR *args, CAmanda *Amanda)
{
	Amanda->Render();
	return NULL;
}

void *CA_Bursting(LPSTR *args, CAmanda *Amanda)
{
	Amanda->Explosion.Render();
	return NULL;
}

void *CA_Kill(LPSTR *args, CAmanda *Amanda)
{
	Amanda->EntombMe();
	return NULL;
}