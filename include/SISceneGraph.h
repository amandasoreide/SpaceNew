/**	Definition of the class Space Invaders Scene Graph

*	Prefix: CSG_

*	@author Ramon Molla
*	@version 2018-02
*/

#ifndef CSG_SCENE_GRAPH
#define CSG_SCENE_GRAPH

#include <UGKSceneGraph.h>
#include <GameCharacters.h>

using namespace UGK;

/**
*	\class SISceneGraph
*	
*/
class SISceneGraph : public CSceneGraph
{	
public:

	//Attributes
	bool	EndByTime,
			EndByFrame;

	CHAR_RENDER_MODE RenderMode;

	//Methods
	void Init ();
	void SetQTRoot(QuadtreeRoot *QTR);
	bool AliveShips();
	void SetEndingCriteria(bool ByTime, bool ByFrame);
	void Update();
	///Change the way the scene graph is going to render all the objects on the screen
	void ChangeRenderMode(CHAR_RENDER_MODE Mode);
	void ChangeRenderMode(CHAR_RENDER_MODE , GCHARS_CharacterType);
	//Setters
	void SetGameRef(CCharacter *Game);
};

#endif