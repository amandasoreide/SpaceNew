/**	Definition of the class CShader

*	Prefix: CSHD_

*	@author Ramon Molla
*	@version 2012-09
*/

#ifndef CSHD_SHADER
#define CSHD_SHADER


#include <vector>
#include <UGKString.h>
#include <glee/glee.h>
#include <glut.h>


#define CSHD_NO_SHADER		0
#define CSHD_NO_SHADER_PROG	0

typedef std::vector<std::string> CSHD_String_VECTOR;
typedef std::vector<GLuint> CSHD_Uint_VECTOR;

typedef enum CSHD_SHADER_STATES
{
	CSHD_FREE,		//No shader 
	CSHD_COMPILED,
	CSHD_LINKED,
	CSHD_MAX_STATES
};

class CShader
{
public:
	UGKS_String	FileName;	///< Name of the file with shader source code
	GLuint		Type,		///< GL_VERTEX_SHADER / GL_FRAGMENT_SHADER
				ShaderId,	///< CShader identifier
				ProgId;		///< Program identifier attached to the shader

	CSHD_SHADER_STATES State;

	CShader			();
	~CShader		();

	void Reset		();
	void Compile	();
};

#endif
