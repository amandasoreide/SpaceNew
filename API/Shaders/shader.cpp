/*  
	File: shader.cpp
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

#include "shader.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

using std::cout;
using std::cerr;
using std::string;
using std::endl;
using std::setw;

/*******************
	NUEVO
*******************/

// Loads and initializes the Shader
int shader::load(UGKS_String filename, GLuint shtype){
	
	std::ifstream file;
	string st;
//	Strings ssrc;
//	GLchar* *src;
//	GLsizei nlines;
//	GLuint id;


	type = shtype;

	file.open(filename);
	if (!file) {
		cerr << "Could not open source file" << filename << endl;
		return 0;
	} else 
		cerr << endl << "Reading " << filename << "..." << endl;

	while (!file.eof()) {
		getline(file, st);
		st += '\n';
		scode.push_back(st);
	}
	file.close();

//	cerr << endl << this->getId() << endl;

/*
//GLchar* *src;

	scode=new GLchar* [ssrc.size()];
	if (scode==NULL) 
		return 0;

	nl=(GLsizei)ssrc.size();
	for (int i=0; i<nl; i++)
		scode[i]=(GLchar* )ssrc[i].c_str();

#ifdef _DEBUG
	cerr << endl << "Shader's source code: "<< endl;
	for (GLsizei j=0; j<nl; j++)
		cerr << setw(3) << j+1 << " |" << scode[j]; // << endl;
#endif
*/
	return 1;
}

// Builds the shader
int shader::build(){
	
	GLint compiled;
//	GLsizei nlines;

		GLchar* *src;

	src=new GLchar* [scode.size()];
	if (src==NULL) 
		return 0;

	nl=(GLsizei)scode.size();
	for (int i=0; i<nl; i++)
		src[i]=(GLchar* )scode[i].c_str();

#ifdef _DEBUG
	cerr << endl << "Shader's source code: "<< endl;
	for (GLsizei j=0; j<nl; j++)
		cerr << setw(3) << j+1 << " |" << src[j]; // << endl;
#endif



	id=glCreateShader(type);

	//nlines=(GLsizei)scode.size();

	glShaderSource(id, nl, (const GLchar* *)src, NULL);
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);

	if(compiled == GL_FALSE) {
		GLint length;
		GLchar* log;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		log=new GLchar [length];
		glGetShaderInfoLog(id, length, &length, log);
		cerr << "Error compilando el shader:" << endl;
		cerr << log << endl;
		delete [] log;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

// Updates the shader, re-builds it
int shader::update(UGKS_String filename, const GLuint shadertype){
		int res = 1;	
		
		res=load(filename, shadertype);
		if(!res) return 0;
		
		res=build();
		if(!res) return 0;

		return 1;
	}

// Deletes the shader and all its content
int shader::del(){

	glDeleteShader(id);
	scode.clear();
	
	return 0;
}

// Returns the Shader's Id
GLuint shader::getId(){
	return id;
}