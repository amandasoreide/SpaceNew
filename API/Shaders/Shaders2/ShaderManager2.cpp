/**	Definition of the class CShader Manager

*	Prefix: CSHM_

*	@author Ramon Molla
*	@version 2012-09
*/

#include <fstream>
#include <iostream>
#include <iomanip>

#include "ShaderManager2.h"

using std::cout;
using std::cerr;
using std::endl;
using std::setw;


CShaderManager::CShaderManager	()
{
};

CShaderManager::~CShaderManager	()
{
	Shaders.clear();
};

/**
	* \fn void CShaderManager::Compile(const CSHD_String_VECTOR &vshaders, const CSHD_String_VECTOR &fshaders, CSHD_Uint_VECTOR &vshids, CSHD_Uint_VECTOR &fshids)
	* Compiles multiple shader files simultaneously. Returns corresponding identifiers
	* \param[in] const CSHD_String_VECTOR &vshaders	Names of the Vertex Shaders
	* \param[in] const CSHD_String_VECTOR &fshaders	Names of the Fragment Shaders
	* \param[in] CSHD_Uint_VECTOR &vshids	Vertex Shaders ids vector
	* \param[in] CSHD_Uint_VECTOR &fshids	Fragment Shaders ids vector
*/

/***********TODO
void CShaderManager::Compile(const CSHD_String_VECTOR &vshaders, const CSHD_String_VECTOR &fshaders, CSHD_Uint_VECTOR &vshids, CSHD_Uint_VECTOR &fshids)
{
	GLuint t;

	for (unsigned int i=0; i<vshaders.size(); i++) {
		t=Compile(vshaders[i], GL_VERTEX_SHADER);
		if (t>0)
			vshids.push_back(t);
	}

	for (unsigned int i=0; i<fshaders.size(); i++) {
		t=Compile(fshaders[i], GL_FRAGMENT_SHADER);
		if (t>0) 
			fshids.push_back(t);
	}
}


/**
	* \fn void CShaderManager::LinkProgram(const CSHD_Uint_VECTOR &shids)
	* Binds a program consisting of one or more shaders.
	* \param[in] std:: vector with identifiers bound to shaders
*
void CShaderManager::LinkProgram(const CSHD_Uint_VECTOR &shids)
{
	GLint linked;
	
	ProgId = glCreateProgram();
	for (unsigned int i=0; i<shids.size(); i++)
		glAttachShader(ProgId, shids[i]);
	glLinkProgram(ProgId);
	
	glGetProgramiv(ProgId, GL_LINK_STATUS, &linked);

	if (linked == GL_FALSE) {
		GLint length;
		GLchar* log;
		glGetProgramiv(ProgId, GL_INFO_LOG_LENGTH, &length);
		log=new GLchar [length];
		glGetProgramInfoLog(ProgId, length, &length, log);
		cerr << "Error enlazando el programa:" << endl;
		cerr << log << endl;
		delete [] log;
		glDeleteProgram(ProgId);
		ProgId	= CSHD_NO_SHADER_PROG;
	}
	State		= CSHD_LINKED;
}
*/
