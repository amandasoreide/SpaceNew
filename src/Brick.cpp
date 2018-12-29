/**
*	Brick declarations

*	@author Ramon Molla
*	@version 2012-06
*/


#include <SITexturesResources.h>
#include <UGKTexturesManager.h>
#include <Brick.h>
#include <GameSounds.h>
#include <GameCharacters.h>
#include <GlobalDefs.h>

void CBrick::Init()
{
	Health = MaxHealth	=	CB_HEALTH_DEFAULT;
	Hit_duration		=	CB_MAX_MAX_HIT_DUR;
	Type				=	CHARS_BRICK;
	SubType				=	CB_DEFAULT;
	ResetTransformations();
	Rotation.v[YDIM] = 180.0;

	UpdateAABB(CB_BRICK_WIDTH, CB_BRICK_LENGTH, CB_BRICK_HEIGHT);
}

///For physics
void CBrick::Collided (CCharacter *CollidedChar)
{	
	
	if (Active())
	{
		switch (CollidedChar->Type)
		{
		case CHARS_SHOOT:	//Any shoot from the player			
			Sound[CB_TOUCH_SND]->Play(UGKSND_LOW_VOLUME);
			AI_Die();
			break;
		}
	}	//If brick active
}

void CBrick::Render(void)
{
	if (Active())
	switch (RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character: camera, collision objects,...
		return;
	case CHAR_LINES3D:
	case CHAR_3D:
		// correct a pixel bug which display a Brick anywhere - needs to be fixed
		if (Position.v[ZDIM] || Position.v[YDIM] > -3.00f)
			Revive();	//It is not active but alive
		Render2D();
	case CHAR_2D:
		Render2D();
		break;
	default: return;
	}
}