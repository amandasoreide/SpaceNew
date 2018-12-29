/** Header that contain Read Directory of all language for game

*	@author Charco Aguirre, Jorge
*	@version 2015-11
*/

#ifndef Languages_Directory_H
#define Languages_Directory_H
#include <map>

using namespace std;

/**
* Language class is defined
* Where you define the language file loaded from the language directory
* The same containing text for the labels in the game depending on which option is chosen
* In addition to the errors generated when loading
*/

namespace UGK
{
	class Language_Directory
	{
		public:
			//Function for load and read of ISO639 and Languages
			inline void readDirectory();

	};
}
#endif;