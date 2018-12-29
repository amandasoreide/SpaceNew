/**
*	Defines the class of a background that draws a background for the scene
*	@file Background.h

*	Prefix: CBG_

*	@author Ramón Mollá
*	@version 2018-02
*/

#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <GUIGadget.h>
#include <UGKTexturesManager.h>

#define CBG_ROTATE		true
#define CBG_DONT_ROTATE false

using namespace UGK;

class CBackground: public CGUIGadget
 {
 public:
	 ///Constructor of the class
	 CBackground();

	 ///Destructor of the class
	 ~CBackground() {};

	 ///Renders the background
	 void Render(bool Rotate);
 };
#endif