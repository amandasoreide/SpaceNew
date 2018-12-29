/**	Declaration of all the general game methods
*	@author Ramon Molla
*	@version 2012-10
*/

#include <Game.h>
#include <UGKString.h>

void CGame::Reset()
{
	if (!Version.empty())
		Version = UGKS_EMPTY_STRING;
	if (Title.empty())
		Title = UGKS_EMPTY_STRING;

	RenderMode		= CHAR_NO_RENDER;
	
	Score = HiScore = Level = 0;

	for (unsigned int i = CG_AI_DIR;i<CG_MAX_DIR;i++) Directories[i] = UGKS_EMPTY_STRING;
}

CGame::~CGame()
{
}

void CGame::Save (UGKS_String  fileName) {}; ///<Saves the game to a file - Rodrigo Aldecoa
