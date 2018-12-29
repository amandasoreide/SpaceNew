/*	Definition of the class Scene

	Prefix: CSC_

	@author Ramon Molla
	@version 2011-10
*/

#include "Scene.h"

using namespace UGK;

void CScene::Init()
{
	Angle.v[XDIM]	= Angle.v[YDIM] = 0.0;
}

CScene::~CScene()
{
}
