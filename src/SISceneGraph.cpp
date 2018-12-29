/*The place where resides the class Space Invaders Scene Graph
	@file: SISceneGraph.h
	@author Ramon Molla
	@version 2018-02
*/

#include <SISceneGraph.h>
#include <GameCharacters.h>
#include <ExplosiveChar.h>
#include <UGKMeshManager.h>

/**   
   * @fn  SISceneGraph::Init
   * Start the scene graph
*/
void SISceneGraph::Init()
{
}

#ifdef CHAR_USE_QUADTREE
void SISceneGraph::SetQTRoot(QuadtreeRoot *QTR)
{
	CCharacter *C;

	unsigned int	i;

	for (i = 0; i < Size(); i++)
	{
		C = Object(i);
		switch (C->Type)
		{
		case CHARS_SHIP:
		case CHARS_SUPPLYSHIP:
		case CHARS_CIRCLESHIP:
			C->SetQTRoot(QTR);
		}//Switch
	}//For
}
#elif CHAR_USE_AABB
#endif

bool SISceneGraph::AliveShips()
{
	CCharacter *C;

	unsigned int	i;

	for (i = 0; i < Size(); i++)
	{
		C = Object(i);
		switch (C->Type)
		{
		case CHARS_SHIP:
		case CHARS_SUPPLYSHIP:
		case CHARS_CIRCLESHIP:
			if (C->Alive())
				return true;
		}//Switch
	}//For
	return false;
}

/**
*	@fn void SISceneGraph::SetEndingCriteria(bool ByTime, bool ByFrame)
*	Set the conditions to finish the game
*/
void SISceneGraph::SetEndingCriteria(bool ByTime, bool ByFrame)
{
	CExplosiveChar *C;

	unsigned int	i;

	for (i = 0; i < Size(); i++)
	{
		C = (CExplosiveChar*)Object(i);
		switch (C->Type)
		{
		case CHARS_BONUS:
		case CHARS_BONUS_MNGR:
		case CHARS_BRICK:
		case CHARS_CIRCLESHIP:
		case CHARS_LASER:
		case CHARS_PLAYER:
		case CHARS_REACTOR:
		case CHARS_SHIP:
		case CHARS_SHOOT:
		case CHARS_SUPPLYSHIP:
		case CHARS_SHOOT_MNGR:
			C->EndByTime	= ByTime;
			C->EndByFrame	= ByFrame;
		}//Switch
	}//For

	EndByTime	= ByTime;
	EndByFrame	= ByFrame;
}

/**   
   * @fn SISceneGraph::Update
   * Updates all elements within the Scene Graph
*/
void SISceneGraph::Update ()	///Performs all the geometric transformations on every ship alive in the navy
{
#if defined(XM_UPD_TIME_DISC) || defined(XM_UPD_TIME_CONT)
	TimerManager.GetTimer(SIGLBT_UPDATE_TIMING)->InitCounting();
#endif

	CCharacter *C;
	
	unsigned int i;

	for (i = 0; i < Size(); i++)
		Object(i)->Update();
	
#if	defined(XM_UPD_TIME_DISC) || defined(XM_UPD_TIME_CONT)
	TimerManager.EndAccCounting(SIGLBT_UPDATE_TIMING);
#endif
}

/**   
   * @fn SISceneGraph::ChangeRenderMode(CHAR_RENDER_MODE Mode)
   * @param [in] CHAR_RENDER_MODE Mode represents the mode
   * Changes the rendering mode to all the objects in the scene graph
*/
void SISceneGraph::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	for (unsigned int i = 0; i < Size(); i++)
		Object(i)->ChangeRenderMode(Mode);
}

void SISceneGraph::ChangeRenderMode(CHAR_RENDER_MODE Mode, GCHARS_CharacterType CharType)
{
	CCharacter *C;

	for (unsigned int i = 0; i < Size(); i++)
	{
		C = Object(i);
		if (CharType == C->Type)
			C->ChangeRenderMode(Mode);
	}//For
}

void SISceneGraph::SetGameRef(CCharacter *Game)
{
	CExplosiveChar * EC;
	
	unsigned int	i;

	for (i = 0; i < Size(); i++)
	{
		EC = (CExplosiveChar*)Object(i);
		switch (EC->Type)
		{
			//All these characters are Explosive Chars
		case CHARS_BONUS:
		case CHARS_BONUS_MNGR:
		case CHARS_BRICK:
		case CHARS_CIRCLESHIP:
		case CHARS_LASER:
		case CHARS_PLAYER:
		case CHARS_REACTOR:
		case CHARS_SHIP:
		case CHARS_SUPPLYSHIP:
		case CHARS_SHOOT:			
			EC->AssignTMG(Game);
		}//Switch
	}//For
}


