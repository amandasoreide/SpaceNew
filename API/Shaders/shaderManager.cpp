
#include "shaderManager.h"

#include <fstream>
#include <iostream>
#include <iomanip>

using std::cout;
using std::cerr;
using std::string;
using std::endl;
using std::setw;

/*******************
	NUEVO
*******************/

/**
 * @brief Constructor with the number of shaders
 *
 * Initializes the shaderManager object, allocates memory for 
 * the array of shaders dynamically from the specified size.
 *
 * Calls: <em>glCreateProgram()</em>.
 *
 * @param s desired size for the shaders array (number of shaders)
 */
shaderManager::shaderManager(int s){
	
	programId = glCreateProgram();
	//shaders = (shader*)malloc(sizeof(shader)*tam);
	shaders = (shader**)malloc(sizeof(shader*)*s);
	nShaders = s;
	shindex = 0;
	
}

/**
 * Enables or disables the usage of shaders
 *
 * Calls: <em>glUseProgram()</em>
 *
 * @param s true: enable shaders, false: disable them
 * 
 */
void shaderManager::useShaders(bool s){

	if(programId>=0){
		if(s)
			glUseProgram(programId);
		else 		glUseProgram(0); // Disables all the shaders
	}
	
}

/**
 * Links the shaders
 *
 * Links all the shaders of the shaderManager to the Program.
 * Calls: <em>glAttachShader(), glLinkProgram()</em> and <em>glGetProgram()</em>
 * 
 * @return 0 if there was some error, 1 otherwise
 */
int shaderManager::link(){
	GLint linked;

	for (int i=0; i<nShaders; i++)
		glAttachShader(programId, shaders[i]->getId());
	glLinkProgram(programId);
	
	glGetProgramiv(programId, GL_LINK_STATUS, &linked);

	if (linked == GL_FALSE) {
		GLint length;
		GLchar* log;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
		log=new GLchar [length];
		glGetProgramInfoLog(programId, length, &length, log);
		cerr << "Error linking the program:" << endl;
		cerr << log << endl;
		delete [] log;
		glDeleteProgram(programId);
		return 0;
	
	}

	return 1;
	
}


/**
 * Get the Program Id
 * 
 * @return programId value
 */
GLuint shaderManager::getProgramId(){
	return programId;
}

/**
 * Adds a new shader
 *
 * Adds a new shader to the shaderManager's array (shaders).
 * It loads its source code and builds it.
 *
 * @param filename Path to the shader file
 * @param shtype type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
 * 
 * @see shader::load()
 * @see shader::build()
 *
 * @return 0 if there was some error, 1 otherwise
 */
int shaderManager::addShader(UGKS_String filename, GLuint shtype){
	int res;

	shaders[shindex] = new shader();

	// we load it and read the source file
	res = shaders[shindex]->load(filename, shtype);
	if(!res) return 0;

	// we build it
	res = shaders[shindex]->build();
	if(!res) return 0;

	shindex++;
	return 1;

}

/*  
	File: shaderManager.cpp
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

/**
 * Deletes the shaderManager.
 *
 * It calls the delete function of every shader from the shaders array,
 * freeing the allocated memory and finally, deletes the OpenGL program.
 *
 * Calls to <em>glDeleteProgram()</em>
 *
 * @see shader::del(); 
 * @return 0 if there was some error, 1 otherwise
 */

int shaderManager::del(){

	for (int i=0; i<nShaders; i++)
		shaders[i]->del();

	delete [] shaders;
	glDeleteProgram(programId);

	return 0;
}

