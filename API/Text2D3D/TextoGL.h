/* 
	Author: Vicente Broseta Toribio
	Last update: 2012-10
*/


#ifndef TEXTOGL_H
#define TEXTOGL_H

#include <RTDeskEntity.h>
#include <glut.h>
#include <SIMessage.h>

using namespace UGK;

class cTextoGL : public RTDESK_CEntity{
public:

	rgbText color;
	UGKALG_SimpleVector posicion;
	UGKALG_SimpleVector escalado;


	float timeGdeskMsg;						//Tiempo Mensaje GDESK
	cMsgTxtGL *msg;
	UMSG_COMMAND_MSG Command;

	cTextoGL(void);
	~cTextoGL();

	void Draw3D(char* txt);
	void Draw3D_RTdesk(char* txt);
	void setColor(GLfloat red, GLfloat green, GLfloat blue);
	void setPosicion(GLfloat x, GLfloat y, GLfloat z);
	void setEscalado(GLfloat x, GLfloat y, GLfloat z);
	inline void setPosicion(UGKALG_SimpleVector v) { posicion.v = v.v; };
	inline void setEscalado(UGKALG_SimpleVector v) { escalado.v = v.v; };
	void text2D_draw(unsigned int x, unsigned int y, char* text, int anchoVentana, int altoVentana, GLfloat *color);

private:
	void ReceiveMessage(RTDESK_CMsg *pMsg);
};

#endif // TEXTOGL.H