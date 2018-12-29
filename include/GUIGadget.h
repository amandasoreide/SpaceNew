/**
*	@file GUIGadget.h
*	Definition of the class Gadget of a GUI

*	Prefix: CGUIG_

*	@author Ramon Molla
*	@version 2018-02
*/

#ifndef CGUIG_GADGET
#define CGUIG_GADGET

#include <UGKCharacter.h>

#define CGUIG_MAX_NUMBERS 7

using namespace UGK;

class CGUIGadget: public CCharacter
{
protected:
	void RenderQuad(int T, float HalfWidth, float HalfHeight);
	void RenderQuad(float HalfWidth, float HalfHeight);

public:
	//Attributes
	float		Fading,				///<Interface Fading percentage
				LoadingBMPPosition;	///<Unary percentage of the position of the element to put on the screen. May affect any dimension
	bool		Passing2D23D,		///< Flag that alerts from moving from global 2D rendering scene mode to a 3D one
				Blending,
				Alpha;
	char		Number[CGUIG_MAX_NUMBERS] = UGKS_EMPTY_STRING;
	int			NumberLength,
				NumberTexture;

	//Constructor
	CGUIGadget() {Init();}

	//Methods
	void Init ();

	///Shows the user interface on the screen
	void Render(void);
	void DisplayCube(); //Show a cube in the space
	void DisplayCube(float size); //Show a cube in the space
};
#endif
