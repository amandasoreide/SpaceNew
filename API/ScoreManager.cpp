/** Definition of the class Score Manager
*  General class to manage all the scores
*  Prefix: CSCM_

	@author Ramon Molla
	@author Nuria Martinez
	@version 2012-06
*/

#include <ScoreManager.h>

#ifndef WINDOWS_LOADED
#define WINDOWS_LOADED
#include <windows.h>
#endif

void CScoresManager::Init()
{
	for(int i=0; i<10; i++) {
		scoresList[i] = new CScore();
	}
}
/**
* Destroys the object
*/
CScoresManager::~CScoresManager()
{
}

///Returns the highest score
int CScoresManager::GetHiscore()
{
	// READING IN hiscore REGISTRY
	HKEY key;
	DWORD disposition;

	// check key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, LPWSTR("SOFTWARE\\SpaceInvaders-OpenGL\\"), 0, KEY_READ, &key) == ERROR_SUCCESS)
	{
		// read key value
		int  nombre = 0;

		DWORD type = REG_DWORD;
		DWORD size = sizeof(DWORD);

		RegQueryValueEx(key, LPWSTR("hiscore"), NULL, &type, (LPBYTE)&nombre, &size);
		return nombre;
	}
	else
	{
		// create key
		RegCreateKeyEx(HKEY_LOCAL_MACHINE, LPWSTR("SOFTWARE\\SpaceInvaders-OpenGL\\"), 0, UGKS_EMPTY_LPWSTR, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, &disposition);
		return 0;
	}
	RegCloseKey(key);
}

///Sets the highest score
void CScoresManager::SetHiscore(int score/*(CScore *score)*/) 
{
	// BACK IN THE hiscore REGISTRY
	HKEY key;	

	int current_hiscore = 0;
	current_hiscore = GetHiscore();

	// @author Nuria Martinez
	// note: ScoreTemp is a temporary variable to compile the program. It has to happen in this function a CScore and not an int
	// When so, delete the following two lines and uncomment /*score*/
	CScore *scoreTemp = new CScore();
	scoreTemp->SetPoints(score);

	// update the score list
	bool actualizarListaScores = InsertScore(/*score*/scoreTemp);

	if (!actualizarListaScores) // if it is not update then "score < current_hiscore"
		return;

	// check key
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, LPWSTR("SOFTWARE\\SpaceInvaders-OpenGL\\"), 0, KEY_WRITE, &key) == ERROR_SUCCESS)
	{
		RegSetValueEx(key, LPWSTR("hiscore"), 0, REG_DWORD, (LPBYTE)&score, sizeof(DWORD));
	}
	RegCloseKey(key);
}

///Returns the scores list
CScore* CScoresManager::GetScoresList()
{
	return *scoresList;
}


///Update the list of the best scores
bool CScoresManager::InsertScore(CScore *score)
{
	int current_hiscore = 0;
	current_hiscore = GetHiscore();

	if(score->GetPoints() > scoresList[9]->GetPoints()) // If score is among the top 10
	{
		// If the new score is higher than Hiscore, add it in the first position of the list
		if(score->GetPoints() > current_hiscore)
		{
			for(int i = 0; i < sizeof(scoresList)-1; i++)
			{
				scoresList[i+1] = scoresList[i];
			}
			
			
			scoresList[0] = score;
			return true;
		}
		else // we find his new location
		{
			for(int i = 1; i < sizeof(scoresList)-1; i++)
			{
				if(score->GetPoints() > scoresList[i]->GetPoints())
				{
					for( int j=i; j < sizeof(scoresList)-1; j++)
					  scoresList[j+1] = scoresList[j];
					
					
					scoresList[i] = score;	//ACHTUNG: Esto funcionará mal siempre por ser un puntero a un pa´rametro. En cuanto se cierre la función, se hab´ra quedado con un puntero inválido. Hay que copiar contenido.
					return true;
				}

			}
		}
		
	}

		return false;
}
