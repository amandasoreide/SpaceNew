/**	Definition of the class Shoot

*	Prefix: CSHM_

*	@author Ramon Molla
*	@version 2011-11
*
*	Last modified: 2014-2015 by Miguel Domingo
*	Previous modified: 2012-2013 by Vicente Perez
*/

#ifndef CSHM_SHOOTSMNG
#define CSHM_SHOOTSMNG

#include <Shoot.h>
#include <UGKCharactersPool.h>
#include <ExplosiveChar.h>

using namespace UGK;

typedef deque<CShoot*> CSHM_SHOOT_LIST;

/// Manages all the shoots available during the playing time
class UGK_API CShootsManager : public CExplosiveChar
{
public:

	CSHM_SHOOT_LIST	ShootsAct;		///<List of active shoots

	//RT-DESK messages
	RTDESK_CMsg *msg;							//RTDESK Message
	RTDESK_CMsg *msgUpd;						//RTDESK Message
	double		msUpdShoot;
	unsigned int defaultMessageType;

	//Time controling
	RTDESK_TIME	TimeUpdShoot, TimeRTdeskMsg;	//RTDESK Message Time

	bool	EndByTime,
			EndByFrame;
	//Methods
	void Init(void);

	CShootsManager(){ Init(); }												///<Constructor of the class
	void NewShoot(CSH_SHOOT_TYPE SType, Vector &Pos, Vector &Spd);			///<Generates a new shoot
	void SetEndingCriteria(bool ByTime, bool ByFrame);

	///Pass all active shoots to the character pool. After its execution there are no shoots at all in the playground 
	void ReleaseAllShoots();
	void Maintenance(void);													///<Lists shoot maintenance

	void ReceiveMessage(RTDESK_CMsg *pMsg);

	void Render(void);		///<Shows all the shoots on the screen
	void Update(void);

	///Change the way all the shoots are going to be rendered on the screen
	void ChangeRenderMode(CHAR_RENDER_MODE);

	void AssignSounds2Shoots();
	void SetLocalTimers();
};
#endif