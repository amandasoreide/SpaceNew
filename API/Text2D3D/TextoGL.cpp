#include "TextoGL.h"


cTextoGL::cTextoGL(){
	color.r = 0.0f;
	color.g = 1.0f;
	color.b = 0.0f;
	posicion.x = 0.0f;
	posicion.y = 0.0f;
	posicion.z = 0.0f;
	escalado.x = 1.0f;
	escalado.y = 1.0f;
	escalado.z = 1.0f;
}

cTextoGL::~cTextoGL(){

}

void cTextoGL::Draw3D(char* txt){
	wchar_t unicode[256];

	MultiByteToWideChar(CP_ACP, 0, txt, -1, unicode, 256);
	glPushMatrix();
	glColor3f(color.r, color.g, color.b);
	glScalef(escalado.x, escalado.y, escalado.z);
	glTranslatef(posicion.x, posicion.y, posicion.z);
	glPopMatrix();
}
void cTextoGL::Draw3D_RTdesk(char* txt){
	wchar_t unicode[256];

	MultiByteToWideChar(CP_ACP, 0, txt, -1, unicode, 256);
	glPushMatrix();
	glColor3f(color.r, color.g, color.b);
	glScalef(escalado.x, escalado.y, escalado.z);
	glTranslatef(posicion.x, posicion.y, posicion.z);
	glPopMatrix();

	//¡¡¡¡¡¡¡¡¡Enviar el mensaje¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡
	/*RTDESK_CMsg *pMsg;
	pMsg = GetMessageToFill();
	pMsg->Command = MSG_DIBUJA;
	SendMsg(pMsg,*this,timeGdeskMsg);*/
}

void cTextoGL::setColor(GLfloat red, GLfloat green, GLfloat blue){
	color.r = red;
	color.g = green;
	color.b = blue;
}

void cTextoGL::setPosicion(GLfloat x, GLfloat y, GLfloat z){
	posicion.x = x;
	posicion.y = y;
	posicion.z = z;
}

void cTextoGL::setEscalado(GLfloat x, GLfloat y, GLfloat z){
	escalado.x = x;
	escalado.y = y;
	escalado.z = z;
}

void cTextoGL::text2D_draw(unsigned int x, unsigned int y, char* text, int anchoVentana, int altoVentana, GLfloat *color)
{
	//glPushAttrib(GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);
	//glDisable(GL_LIGHTING);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	//glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, anchoVentana, 0, altoVentana);
	//glMatrixMode(GL_MODELVIEW);

	//glPushMatrix();
	//glLoadIdentity();
	if (color==NULL) {
		GLfloat red[3]={1.0f, 0.0f, 0.0f};
		glColor3fv(red);
	} else
		glColor3fv(color);
	glRasterPos2f(x, altoVentana-y);
	while (*text) 
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);

	glPopMatrix();
	/*glMatrixMode(GL_PROJECTION);
	glPopMatrix();*/
	glMatrixMode(GL_MODELVIEW);
	//glPopAttrib();
}

void cTextoGL::ReceiveMessage(RTDESK_CMsg *pMsg){
	switch (pMsg->Type)
	{
	case UMSG_MSG_TEXTGL:
			cMsgTxtGL *auxMsg;
			auxMsg = (cMsgTxtGL*)pMsg;

			switch (auxMsg->Command)
			{
			case COMMAND_3DDRAW:
				Draw3D_RTdesk(auxMsg->text);
				break;
			case COMMAND_2DDRAW:
				text2D_draw(auxMsg->txt2D.x, auxMsg->txt2D.y, auxMsg->txt2D.txt, auxMsg->txt2D.anchoV, auxMsg->txt2D.altoV, auxMsg->txt2D.color);
				break;
				case COMMAND_SETCOLOR:
				setColor(auxMsg->rgb.r,auxMsg->rgb.g,auxMsg->rgb.b);
				break;
				case COMMAND_SETPOSI:
				posicion.v = auxMsg->xyz.v;
				break;
				case COMMAND_SETSCALE:
				escalado.v = auxMsg->xyz.v;
				break;
			}
		break;
	}

}