/*	Definition of the class Scene
*	@file Scene.h

*	Prefix: CSC_

*	@author Ramon Molla
*	@version 2011-10
*/

#ifndef CSC_SCENE
#define CSC_SCENE

#include <UGKAlgebra.h>

class CScene
{
	//Atributes
public:

	bool Moving;
	UGK::Vector	Start,		///< For scene rotation
		Angle;

	//Methods
	void Init (void);

	CScene() {Init();}
	~CScene();	
};

//Definitions for the game


#endif