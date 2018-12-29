/**	Definition of the class CShader Manager

*	Prefix: CSHM_

*	@author Ramon Molla
*	@version 2012-09
*/

#ifndef CSHM_SHADER_MANAGER
#define CSHM_SHADER_MANAGER


#include "shader2.h"

class CShaderManager
{

	std::vector<CShader> Shaders;	///< Vector of all the shaders the manager can control

public:

	CShaderManager	();
	~CShaderManager	();

	void Compile	(const CSHD_String_VECTOR &vshaders, const CSHD_String_VECTOR &fshaders, CSHD_Uint_VECTOR &vshids, CSHD_Uint_VECTOR &fshids);
	void LinkProgram(const CSHD_Uint_VECTOR &shids);
};

#endif
