/** Cpp: Contains functions for work with Class ISO639
*
*	@author Charco Aguirre, Jorge
*	@version 2015-11
*/

#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <Global.h>
#include "UGKLanguages_ISO639.h"
#include "LanguageDirectory.h"


using namespace std;

using namespace UGK;

map<std::string, std::string> code_iso;		//Variable Global, contain ISO639 of idioms
HWND hWndComboLanguage;
#define PATH_LANGUAGE "\\languages\\*"		//Path that contains the different languages that could be in selection of idiom 

//Convert a string to lowercase
string convertLower(string &cadena)
{
	for (int i = 0; cadena[i]; i++)
		cadena[i] =
		tolower(cadena[i]);
	return cadena;
}

//Convert a string to uppercase
string convertUpper(string cadena)
{
	for (int i = 0; cadena[i]; i++)
		cadena[i] =
		toupper(cadena[i]);
	return cadena;
}


void Language_Directory::readDirectory()
{
	WIN32_FIND_DATA findFileData;
	HANDLE          hFind;
	char dir[MAX_PATH];
	char path[MAX_PATH] = PATH_LANGUAGE;
	DWORD cchCurDir = MAX_PATH;

	GetCurrentDirectoryA(cchCurDir, dir);
	strcat_s(dir, _countof(dir), path);				// Join path + folder that contain file of configuration

	/*------------------------------------------
	Load CODE ISO639
	-------------------------------------------*/
	ISO639 ISO639Map;
	if (ISO639Map.loadISO())
	{
		code_iso = ISO639Map.getISO639();

		/*---------------------------------------------------------
		Convert PATH of directory to LPCWSTR for read de folder
		--------------------------------------------------------*/
		int c = MultiByteToWideChar(0, 0, dir, -1, NULL, 0);
		wchar_t *tmp = new wchar_t[c];
		MultiByteToWideChar(0, 0, dir, -1, tmp, c);
		LPCWSTR dir1 = tmp;
		//----------------------------------------------------------

		hFind = FindFirstFile(dir1, &findFileData);		// Get file of the folder language	
		if (hFind == INVALID_HANDLE_VALUE)
			std::cout << "Ruta incorrecta";
		else
		{	// List all files of folder		
			while (FindNextFile(hFind, &findFileData) != 0)
			{
				/* ----------------------------------------
				Get name of file and convert a type string
				-----------------------------------------*/
				std::wstring tmp(findFileData.cFileName);
				std::string file(tmp.begin(), tmp.end());
				//---------------------------------------

				int pos = file.find(".");
				string extension = file.substr(pos + 1, file.size() - pos);	//Get of extension of language - default is html	
				if (extension == "html")
				{
					string name = convertLower(file.substr(0, pos));		//Get name of language inside folder Languages - Code ISO639
					auto search = code_iso.find(name);						//Find ISO of file Language inside global variable
					if (search != code_iso.end())
					{
						string item = "(" + convertUpper(search->first) + ") - " + convertUpper(search->second);
						/*---------------------------------------------------------
						Convert language of game to TCHAR for combobox
						--------------------------------------------------------*/
						int c = MultiByteToWideChar(0, 0, item.c_str(), -1, NULL, 0);
						TCHAR *tmp = new TCHAR[c];
						MultiByteToWideChar(0, 0, item.c_str(), -1, tmp, c);
						//----------------------------------------------------------
						SendMessage(hWndComboLanguage, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)tmp);
					}
				}
			}
		}
	}
}
