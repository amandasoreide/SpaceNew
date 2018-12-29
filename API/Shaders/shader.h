/*  
	File: shader.h
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


/*
#ifndef _SHADERS_H
#define _SHADERS_H 2008
*/

#include <vector>
#include <UGKString.h>
#include <glee/glee.h>

typedef std::vector<std::string> Strings;
typedef std::vector<GLuint> Uints; 

//enum {VERTEX=GL_VERTEX_SHADER, FRAGMENT=GL_FRAGMENT_SHADER}; // Kind of shader (Vertex or Fragment Shader)



/** 
* \class shader
*
* \brief To work with OpenGLSL shaders

* \author Eduardo Caselles
* 
*/

class shader {
	/**
	 * OpenGL shader's ID
	 */
	GLuint id; // OpenGL shader's ID
	//GLchar* scode; // shader's source code, stored by lines
	
	/**
	 * shader's source code, stored by lines
	 */
	Strings scode;
	
	/**
	 * source code number of lines (size of scode)
	 */
	int nl; // source number of lines (size of scode)
	
	/**
	 * type of shader (Vertex or Fragment)
	 */
	int type; // type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)

public:

	/**
	 * Default constructor
	 */
	shader(){} // Default constructor

	/**
	 * Constructor with the filename and the shader type. 
	 * Initializes the class and loads the shader
	 *
	 * @param filename Path to the shader file
	 * @param shadertype type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
	 *
	 * @see load()
	 *
	 */
	shader(const UGKS_String filename, const GLuint shadertype) { 
		load(filename, shadertype);
	}

	/**
	 * Loads and initializes the Shader
	 * @param filename Path to the shader file
	 * @param shadertype type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
	 * 
	 * @return 0 if there was some error, 1 otherwise
	 */
	int load(const UGKS_String filename, const GLuint shadertype);
	
	/**
	 * Builds the shader, compiling it
	 * 
	 * Calls: <em>glCreateShader(), glShaderSource()</em> and <em>glCompileShader()</em>
	 *
	 * @return 0 if there was some error, 1 otherwise
	 */
	// Builds the shader
	int build();
	
	/**
	 * Builds the shader, compiling it
	 *
	 * @return 0 if there was some error, 1 otherwise
	 */
	// Links the shader to the Program
//	int link(GLuint pid);
	
	/**
	 * Builds the shader, compiling it
	 * 
	 * @return 0 if there was some error, 1 otherwise
	 */
	// Disables the shader, deleting the content
//	int disable();
	
	/**
	 * Updates the shader.
	 * 
	 * It loads the file and re-builds it (compiles the new source code).
	 * 
	 * @return 0 if there was some error, 1 otherwise
	 * @see load()
	 * @see build()
	 */
	int update(UGKS_String filename, const GLuint shadertype);

	/**
	 * Deletes the shader and all its content.
	 *
	 * It Deletes the OpenGL shader and frees the allocated memory
	 * for the source code (scode array).
	 *
	 * Calls: <em>glDeleteShader()</em> and <em>STL: vector::clear()</em>
	 *
	 * @return 0 if there was some error, 1 otherwise
	 */
	// Deletes the shader and all its content
	int del();
	
	/**
	 * Get the Shader's Id
	 * 
	 * @return the Shader's Id
	 */
	// Returns the Shader's Id
	GLuint getId();
};

//#endif
