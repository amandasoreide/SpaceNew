/**
 * @file SIMessage.cpp
 * User Message struct definition
 * Copyright(C) 2013
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

#include <SIMessage.h>

RTDESK_CMsg* CMsgFactory::CreateNewMsg(int MsgType)
{
	switch (MsgType)
	{
		case UMSG_MSG_BASIC_TYPE:
			//Default empty message
			return new RTDESK_CMsg;
			break;
		case UMSG_MSG_TEXTGL:		//Type 1
			return new cMsgTxtGL;
			break;
		case UMSG_MSG_GAME:			//Type 2
			return new cMsgGame;
			break;
		case UMSG_MSG_NAVY:			//Type 3
			return new cMsgNavy;
			break;
		default:
			return new RTDESK_CMsg;
	}
}