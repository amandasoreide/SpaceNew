/*	Definition of the class Fonts Manager



	Prefix: CFM_

	@author Ramon Molla
	@version 2013-03
*/

#ifndef CFM_FONTSMNG //Se borra la macro Font manager
#define CFM_FONTSMNG //Se define la macro Font manager

#include <glTexFont.h>

/**
	* This class defines the Font Manager
    * @param int variable FontSize  for the font size of the game
    * @param int variable FontVerticalSpace space points
    * @param int variable FontAlignLeft  for the font alignment on the leftside of the game
    * @param int variable FontAlignLeftUp  for the font alignment on the leftside up of the game
	* @param int variable FontAlignRight  for the font alignment on the rightside of the game
    * @param int variable FontAlignRightUp  for the font alignment on the rightside up of the game

    * @param char variable ErrorMsg error messages

*/
class CFontsManager
{
	
protected:

	int		FontSize;			// FONT SIZE
	int		FontVerticalSpace;	// FONT VERTICAL SPACE

	int		FontAlignLeft;
	int		FontAlignLeftUp;
	int		FontAlignRight;
	int		FontAlignRightUp; 

public:
	//Methods
	void Init (void);

	CFontsManager(){Init();}	///Constructor of the class
	inline void SetFontSize (void)				{ fontSize (FontSize);}
	inline void SetFontSize (int Size)			{ fontSize (Size);}
	inline void SetFontVerticalSpace (int Size)	{ FontVerticalSpace = Size;}
	
	inline int GetFontSize(void)				{ return FontSize;}
	inline int GetFontVerticalSpace(void)		{ return FontVerticalSpace;}

	inline void SetFontAlignLeft(int Size)		{ FontAlignLeft = Size;}
	inline void SetFontAlignLeftUp(int Size)	{ FontAlignLeftUp = Size;}
	inline void SetFontAlignRight(int Size)		{ FontAlignRight = Size;}
	inline void SetFontAlignRightUp(int Size)	{ FontAlignRightUp = Size;}

	inline int GetFontAlignLeft(void)			{ return FontAlignLeft;}
	inline int GetFontAlignLeftUp(void)			{ return FontAlignLeftUp;}
	inline int GetFontAlignRight(void)			{ return FontAlignRight;}
	inline int GetFontAlignRightUp(void)		{ return FontAlignRightUp;}

	inline void DefaultFontColor (void)			{ fontColor(1.0f, 1.0f, 1.0f);} //Set font as white
	inline void SetFontColor(float c)			{fontColor(c,c,c);} ///Set to every color the same value
	inline void SetFontColor(float r, float g, float b) {fontColor(r,g,b);}
	inline void SetFontColorA(float r, float g, float b, float a) {fontColorA(r,g,b,a);}
		
	void DrawFont(int xpos, int ypos, char *s, ...) { fontDrawString (xpos, ypos, s);}
 
	static UGKS_String ErrorMsg(const unsigned int);
};
#endif
