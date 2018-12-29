/*The place where resides the class Navy
  General class to support all the attribute general to all the ships, supply ships,...

	@author Ramon Molla
	@version 2012-10
*/

#include <FSMParser.h>
#include <Ship.h>
#include <CircleShip.h>
#include <SupplyShip.h>

#include <Navy.h>
#include <UGKTexturesManager.h>
#include <GlobalTiming.h>
#include <GameCharacters.h>
#include <SISceneGraph.h>	
#include <Player.h>
#include <SIGame.h>

extern SISceneGraph		SceneGraph;
extern CMeshesManager	MeshesManager;
extern CSIGame			*Game;

/**   
   * @fn  CNavy::Init
   * Start Navy
   * The default parameters starts firing frequency, level disabled health    
   * Speed ??starts in left direction, and acceleration from 0    
   * Starts the position in 0    
   * It also defined the limits of movement of enemy ships
*/
void CNavy::Init(void)
{
	//Ships Control
	ShootsFrequency			=  0;
	ShipLevDefaultHealth	= -1;				///Ships health level per level
	ReGenShips				= false;

	Acceleration.Set(CN_INITIAL_ACC,
		0.0f,
		0.0f);

	Speed.Set(CN_INITIAL_SPEED,	// Initial speed value. Only moving in the "x" direction towards the left
			  0.0f,
			  0.0f);

	Position.Set( CN_INITIAL_X_POS,
				  CN_INITIAL_Y_POS,
				  CN_INITIAL_Z_POS);

	AABBRelPos		= CHAR_AABB_LL;	//The navy has its origin at the lower left corner of its AABB
	RenderMode		= CHAR_NO_RENDER;
	PhysicMode		= UGKPHY_PHANTOM;			//Invisible and untouchable
	Type			= CHARS_NAVY;
	Bounce			= false;
	ShipsMoveDown	= 1;
	WithShoots		= 1;

#ifdef CHAR_USE_AABB
	///Global dimensions of the navy playground.
	///When any ship of the navy reaches left or right BB limit, the navy has to change direction
	///When any ship of the navy reaches the bottom of the AABB, the game is finished
	UpdateAABB(CN_INITIAL_WIDTH_POS, CN_INITIAL_HEIGHT_POS, 0.0f);
#endif

	msg = new cMsgNavy;
	msg->Type = UMSG_MSG_NAVY;
	msg->SubType = UMSG_UPDSHIPS;
	msg->Propietary = true;
	//Sounds
}

/**   
   * @fn CNavy::SetBounce
   * Updates ships with the value of a bounce
*/
void CNavy::SetBounce (void)	///Performs all the geometric transformations on every ship alive in the navy
{
#if defined(XM_UPD_TIME_DISC) || defined(XM_UPD_TIME_CONT)
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
#endif

	CCharacter *C;
	
	unsigned int	i;

	for (i = 0; i < SceneGraph.Size(); i++)
	{
		C = SceneGraph.Object(i);
		if (NULL != C)
			if(CHARS_SHIP == C->Type)
				((CShip *)C)->Bounce = Bounce;
	}//For

	Bounce = false;
	
#if	defined(XM_UPD_TIME_DISC) || defined(XM_UPD_TIME_CONT)
	TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
#endif
}

/**   
   * @fn CNavy::SetMeshes
   * Set ship's mesh
   * Asigne a mesh of MeshesManager to each ship and supplyship 
*/
void CNavy::SetMeshes(CMeshesManager* MeshesManager)
{
	unsigned int i,
				 indexMesh;
	CCharacter *C;

	CTexturesManager * TextMngr = (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF];

	for (i = 0; i < SceneGraph.Size(); i++)
	{
		C = SceneGraph.Object(i);
		if (NULL != C)
		switch (C->Type)
		{
		case CHARS_SHIP:
		case CHARS_SUPPLYSHIP:
		case CHARS_CIRCLESHIP:
			indexMesh = C->IndMesh;
			if (indexMesh != CM3D_NO_MESH) {
				C->Mesh = MeshesManager->GetMesh(indexMesh);
				if (C->IndTexture3D != UGKCTM_NO_TEXTURE) C->Mesh->modelo.Materials->tex.texture[0] = TextMngr->Textures[C->IndTexture3D]->Texture.gl_id;
			}
		}//Switch
	}//For
}

/**
* @fn CNavy::SetSounds2Ships
* Set ship's mesh
* Asigne a mesh of MeshesManager to each ship and supplyship
*/
void CNavy::SetSounds2Ships()
{
	unsigned int i;
	CCharacter *C;

	for (i = 0; i < SceneGraph.Size(); i++)
	{
		C = SceneGraph.Object(i);
		if (NULL != C)
		switch (C->Type)
		{
		case CHARS_SHIP:
		case CHARS_SUPPLYSHIP:
		case CHARS_CIRCLESHIP:
			C->SetSounds(Sound);
		}//Switch
	}//For
}

void CNavy::ReceiveMessage(RTDESK_CMsg *pMsg){

	CCharacter * C;
	unsigned int i;

	switch (pMsg->Type)
	{
	case UMSG_MSG_NAVY:
		cMsgNavy *auxMsg;
			auxMsg = (cMsgNavy*)pMsg;
			switch (auxMsg->SubType)
			{
				case UMSG_TURNALL:
					#ifdef DEF_RTD_TIME
					TimerManager.GetTimer(SIGLBT_RTDSKMM_TIMING].InitCounting();
					#endif

					for (i = 0; i < SceneGraph.Size(); i++)
					{
						C = SceneGraph.Object(i);
						if (NULL != C)
						if (CHARS_SHIP == C->Type)
						{
							cMsgNavy *msg2Ships;
							msg2Ships = (cMsgNavy*)GetMsgToFill(UMSG_MSG_NAVY);
							msg2Ships->SubType = UMSG_TURNALL;
							SendMsg(msg2Ships, C, C->Timer[CS_UPD_PERIOD].GetAlarmPeriod() / 4);
						}
					}//For

					#ifdef DEF_RTD_TIME
					TimerManager.EndAccCounting(SIGLBT_RTDSKMM_TIMING);
					#endif
					break;
				case UMSG_UPDSHIPS:
					break;
				case UMSG_UPDSSHIPS:
					break;
				case UMSG_UPDCSHIPS:
					break;
			}
	case UMSG_MSG_BASIC_TYPE:
		break;
	}
}

/**
   * @fn CNavy::AI_Go2Play
   * Go to Play IA
   * This function through the enemies ships to attack
*/
void CNavy::AI_Go2Play(void)
{
	CCharacter *C;

	unsigned int	i;

	for (i = 0; i < SceneGraph.Size(); i++)
	{
		C = SceneGraph.Object(i);
		if (NULL != C)
		switch (C->Type)
		{
		case CHARS_SHIP:
			((CShip*)C)->OutEvent(CS_GROWING);  //v 1->2
			break;
		case CHARS_SUPPLYSHIP:
			((CSupplyShip*)C)->OutEvent(CSS_GETTING_ALIVE);	//v 1->2
			break;
		case CHARS_CIRCLESHIP:
			((CCircleShip*)C)->OutEvent(CCS_GETTING_ALIVE);	//v 1->2

		}//Switch
	}//For
}

void CNavy::SetShipsAttributes()
{
	//Update the whole navy attributes
	//Assign Render Period for the whole navy: CircleShip, SupplyShip, Ships,...
	SetRenderPeriod();
	//Assign Timer Manager for the Timer of the navy
	SetLocalTimers();
	//Assign Render Period for the whole navy: CircleShip, SupplyShip, Ships,...
	SetRenderPeriod();
	SetMeshes(&MeshesManager);

}

/**
* @fn void CNavy::SetLocalTimers()
* Set a local timer for all the navy ships
*/
void CNavy::SetLocalTimers()
{
	CCharacter *C;

	unsigned int	i;

	Timer.resize(CN_MAX_TIMERS);
	UpdateSF(TimerManager.GetSF());	//The same as done in explosive char

	for (i = 0; i < SceneGraph.Size(); i++)
	{
		C = SceneGraph.Object(i);
		if (NULL != C)
		switch (C->Type)
		{
		case CHARS_SHIP:
			C->SetLocalTimers(CS_MAX_TIMERS);
			break;
		case CHARS_SUPPLYSHIP:
			C->SetLocalTimers(CSS_MAX_TIMERS);
			break;
		case CHARS_CIRCLESHIP:
			C->SetLocalTimers(CCS_MAX_TIMERS);
		}//Switch

	}//For
}

/**
* @fn void CNavy::SetRenderPeriod(HRT_Timems RP)
* Set a local Rendering Period for all the navy ships
*/
void CNavy::SetRenderPeriod()
{
	CCharacter *C;

	HRT_Timems RP = Game->DefaultRndPeriod[CHARS_NAVY];

	unsigned int	i;

	Timer[CN_RND_PERIOD].SetAlarm(RP);

	for (i = 0; i < SceneGraph.Size(); i++)
	{
		C = SceneGraph.Object(i);
		if (NULL != C)
		switch (C->Type)
		{
		case CHARS_SHIP:
			C->Timer[CS_RND_PERIOD].SetAlarm(RP);
			break;
		case CHARS_SUPPLYSHIP:
			C->Timer[CSS_RND_PERIOD].SetAlarm(RP);
			break;
		case CHARS_CIRCLESHIP:
			C->Timer[CCS_RND_PERIOD].SetAlarm(RP);
		}//Switch
	}//For
}


