/**	Definition of the class Score Manager

*Prefix: CSCM_

*	@author Ramon Molla
*	@author Nuria Martinez
*	@author refactoring and inclusion in UGK Ramon Molla
*	@version 2014-09
*/

#ifndef CSCM_SCOREMANAGER
#define CSCM_SCOREMANAGER

#define CSCM_MAX_SCORES_LIST 10

#include <UGKScore.h>

using namespace UGK;

class CScoresManager
{
	//Atributes
	CScore *scoresList[CSCM_MAX_SCORES_LIST]; ///List of the 10 best scores Usar vector STL y de CScore no de CScore*
	
public:

	
	//Methods
	void Init();
    
	///Constructor of the class
	CScoresManager	(){Init();};

    ///Destructor of the class
	~CScoresManager	();

	///Returns the highest score
	int GetHiscore();

	///Sets the highest score
	void SetHiscore(int score/*(CScore *score)*/);

	///Returns the scores list
	CScore* GetScoresList();

	///Update the list of the best scores
	bool InsertScore(CScore *score);
};

#endif