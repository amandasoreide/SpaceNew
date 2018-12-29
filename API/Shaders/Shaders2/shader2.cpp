/**	Definition of the class CShader

*	Prefix: CSHD_

*	@author Ramon Molla
*	@version 2012-09
*/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "shader2.h"

using std::cout;
using std::cerr;
using std::string;
using std::endl;
using std::setw;


CShader::CShader	()
{
	State		= CSHD_FREE;
	ShaderId	= CSHD_NO_SHADER;
	ProgId		= CSHD_NO_SHADER_PROG;
};

CShader::~CShader	()
{
	if (CSHD_NO_SHADER_PROG != ProgId)
		glDeleteProgram(ProgId);

	if (CSHD_NO_SHADER != ShaderId)
		glDeleteShader(ShaderId);	
};

/**
	* \fn void CShader::Reset()
	* Reinitialize the content of the shader releasing every program or shader if there were before
	* and reseting the internal shader internal states
*/
void CShader::Reset()
{
	if (CSHD_NO_SHADER_PROG != ProgId)
		glDeleteProgram(ProgId);

	ProgId		= CSHD_NO_SHADER_PROG;

	if (CSHD_NO_SHADER != ShaderId)
		glDeleteShader(ShaderId);

	ShaderId	= CSHD_NO_SHADER;
	State		= CSHD_FREE;
};

/**
	* \fn void CShader::Compile()
	* Loads the shader source code from file "FileName" and compiles it
    * Return the compiled shader identifier, or CSHD_NO_SHADER in the attribute ShaderId
*/
void CShader::Compile()
{
	std::ifstream file;
	std::string st;
	CSHD_String_VECTOR ssrc;
	GLchar* *src;
	GLsizei nlines;
	GLint compiled;

	Reset();

	file.open(FileName);
	if (!file) {
		cerr << "No se ha podido abrir el fichero fuente " << FileName << endl;
		ShaderId = CSHD_NO_SHADER;
	} else 
		cerr << endl << "Compilando " << FileName << "..." << endl;

	while (!file.eof()) {
		getline(file, st);
		st.push_back('\n');
		ssrc.push_back(st);
	}
	file.close();

	src=new GLchar* [ssrc.size()];
	if (src==NULL) 
		ShaderId = CSHD_NO_SHADER;

	nlines=(GLsizei)ssrc.size();
	for (int i=0; i<nlines; i++)
		src[i]=(GLchar* )ssrc[i].c_str();

#ifdef _DEBUG
	cerr << endl << "Codigo fuente del CShader: "<< endl;
	for (GLsizei j=0; j<nlines; j++)
		cerr << setw(3) << j+1 << " |" << src[j]; // << endl;
#endif

	ShaderId = glCreateShader(Type);

	glShaderSource(ShaderId, nlines, (const GLchar* *)src, NULL);
	glCompileShader(ShaderId);
	glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &compiled);

	delete []src;

	if(GL_FALSE == compiled) {
		GLint length;
		GLchar* log;
		glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &length);
		log=new GLchar [length];
		glGetShaderInfoLog(ShaderId, length, &length, log);
		cerr << "Error compilando el shader:" << endl;
		cerr << log << endl;
		delete [] log;
		glDeleteShader(ShaderId);
		State		= CSHD_FREE;
		ShaderId	= CSHD_NO_SHADER;
	}

	State		= CSHD_COMPILED;
}
