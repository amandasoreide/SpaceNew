/*  
	File: shaderManager.h
	Author: Eduardo Caselles
	--------------------------

	This file is part of Shaders Manager.

    ShadersManager is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ShadersManager is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Shaders Manager.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "shader.h"						// SHADERS API

/** 
* \class shaderManager
*
* \brief Manages all the shaders associated to the current program

* \author Eduardo Caselles
* 
*/

class shaderManager {
	/**
	 * OpenGL program's ID
	 */
	GLuint programId; // OpenGL program's ID

	/**
	 * Number of shaders
	 */
	int nShaders;

	/**
	 * Index to the current shader
	 */
	int shindex;

public:

	shaderManager(int s);
	void useShaders(bool s);
	int link();
	GLuint getProgramId();
	int del();
	int addShader(UGKS_String filename, GLuint shtype);

	/**
	 * Array of shaders (dynamic memory)
	 */
	shader** shaders; // Array of associated shaders 

};

