/**
 * @file SIMessage.h
 * User Message struct definition
 * Copyright(C) 2016
 *
 * Prefix: UMSG_

 * @Author:	Dr. Ramón Mollá Vayá
 * @Date:	21/05/2016
 * @Version: 2.0
 *
 * There is no need for a class since no methods are attached for managing this data container
 * Update:
 * Date:	
 * Version: 
 * Changes:
 *
 */

#ifndef RTDESK_CMsg_H
#define RTDESK_CMsg_H

#include <RTDeskMsgFactory.h>
#include <GL/gl.h>
#include <UGKAlgebra.h>

typedef enum
{
	UMSG_MSG_BASIC_TYPE,
	UMSG_MSG_TEXTGL,		//Type 1
	UMSG_MSG_GAME,			//Type 2
	UMSG_MSG_NAVY,			//Type 3
	UMSG_MSG_MAX_TYPE
} UMSG_MSG_TYPE;

// Type 1 ****************************************
typedef struct{
	GLfloat r;
	GLfloat g;
	GLfloat b;
} rgbText;

typedef enum{
	COMMAND_UPD,
	COMMAND_2DDRAW,
	COMMAND_3DDRAW,
	COMMAND_SETCOLOR,
	COMMAND_SETPOSI,
	COMMAND_SETSCALE
} UMSG_COMMAND_MSG;

typedef struct{
	int		x;
	int		y;
	char	*txt;
	int		anchoV;
	int		altoV;
	GLfloat *color;
} param2DText;

class cMsgTxtGL : public RTDESK_CMsg
{
public:
	UMSG_COMMAND_MSG Command;
	UGK::UGKALG_SimpleVector	xyz;
	rgbText	rgb;
	char	*text;
	param2DText	txt2D;
};

// Type 2 ****************************************
typedef enum 
{
	UMSG_UPDATE,
	UMSG_RENDER,
	UMSG_AI,
	UMSG_COLLISION,
	UMSG_PHYSICS,
	UMSG_GENERIC_MSG_MAX_TYPE
} UMSG_MSG_TYPE_GAME;

class cMsgGame : public RTDESK_CMsg{
public:
	UMSG_MSG_TYPE_GAME	SubType;
	int					integer;
};

// Type 3 ****************************************
typedef enum {
	UMSG_UPDSHIPS,
	UMSG_UPDSSHIPS,
	UMSG_UPDCSHIPS,
	UMSG_TURNALL
} UMSG_MSG_TYPE_NAVY;

class cMsgNavy : public RTDESK_CMsg
{
public:
	UMSG_MSG_TYPE_NAVY	SubType;
	int					integer;
};

/********************* MSG Factory **********************/

class CMsgFactory : public RTDeskMsgCFactory
{
	RTDESK_CMsg* CreateNewMsg(int MsgType);
};


#endif  // UserMESSAGE.H
