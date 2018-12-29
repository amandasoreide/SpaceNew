/**	Definition of the class Bonus

	Prefix: CBK_

	@author Ramon Molla
	@version 2011-07
*/

#include <Shoot.h>
#include <Bunker.h>
#include <UGKTexturesManager.h>
#include <GameCharacters.h>
#include <SITexturesResources.h>

UGKS_String CBNK_TypeNames[CBNK_MAXTYPE] =
{
	"BLANK",
	"HOME",
	"USER"
};

/// Bunker class default initialization
void CBunker::Init (void)
{
	Vector Pos;
	Pos.Reset();
	
	Init(CBNK_BLANK, Pos);

	SubType = CBNK_BLANK;
};

#ifdef CHAR_USE_QUADTREE
void CBunker::SetQTRoot(QuadtreeRoot *QTR)
{
	QTRoot = QTR;

	for (int row = 0; row<CBNK_MAX_LINES; row++)
		for(int col=0;col<CBNK_MAX_COLUMNS;col++)
			Brick[row][col].SetQTRoot(QTR);
}
#elif CHAR_USE_AABB
#endif

/**
*	Performs a complete line of bricks at the position Pos
*	\param line	The horizontal line of bricks to write on the screen
*	\param Pos	Position where to start the line of bricks for the bunker
*/
void CBunker::CreateBrickLine(unsigned int line, Vector &Pos)
{
	Vector CurrentPos = Pos;

	CurrentPos.v[YDIM] += line*CB_BRICK_LENGTH;
	
	for (unsigned int i = 0; i<CBNK_MAX_COLUMNS;i++)
	{
		Brick[line][i].Activate();
		Brick[line][i].Column=i;
		Brick[line][i].Row=line;
		Brick[line][i].MoveTo(CurrentPos);
		CurrentPos.v[XDIM] += CB_BRICK_WIDTH;
	}
};

void CBunker::TranslateBunker	(Vector &Pos){

	Vector	DeltaPos = {0.0f, 0.0f, 0.0f},
			NewPos;
	unsigned int  i;

	Activate();
	NewPos = Position;

	for (i = XDIM; i<WDIM; i++)
		if (FLT_MAX != Pos.v[i])
		{
			DeltaPos.v[i]	= Pos.v[i] - NewPos.v[i];
			NewPos.v[i]		= Pos.v[i];
		}	

	MoveRelTo(DeltaPos);

	for(int row=0;row<CBNK_MAX_LINES;row++)
		for(int col=0;col<CBNK_MAX_COLUMNS;col++)
			Brick[row][col].MoveRelTo(DeltaPos);
}

///Performs a default Bunker at position Pos
void CBunker::Init(CBNK_BUNKER_TYPE T, Vector &Pos)
{
	Vector BrickPosition;
	unsigned int row, col, delta = 4;

	IndTexture2D = CTM_BRICK;
	IndTexture3D = CTM_BRICK3D;

	///A bunker has no physical representation. Every brick it has is visible, but not the bunker
	PhysicMode	= UGKPHY_INVISIBLE;
	Type		= CHARS_BUNKER;

	SubType = T;

	// Every bunker is 14 bricks Width and 10 lines high
	// Put the auxiliar vector BricksPosition at the upper left corner
	BrickPosition.Set(Pos.v[XDIM]-(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value*0.5f)+(CB_HLF_BRICK_WIDTH),
					   Pos.v[YDIM]+(CharAABB.AABB[CHAR_BBSIZE][YDIM].Value*0.5f)-(CB_HLF_BRICK_LENGTH),
					   CB_BRICK_Z_POS);

	switch (T)
	{
	case CBNK_BLANK:
		break;
	case CBNK_HOME:	

		for(row=0;row<CBNK_MAX_LINES;row++){
			for(col=0;col<CBNK_MAX_COLUMNS;col++)
			{			
				if (col<delta || col > CBNK_MAX_COLUMNS - delta)
				{
					Brick[row][col].SetDefault();
					Brick[row][col].Health = 0.0f;
				}
				else
				{
					Brick[row][col].Activate();
					Brick[row][col].Health = 1.0f;
				}
				Brick[row][col].MoveTo(BrickPosition);
				BrickPosition.v[XDIM] += CB_BRICK_WIDTH;
			}

			BrickPosition.v[XDIM] = Pos.v[XDIM]-(CharAABB.AABB[CHAR_BBSIZE][XDIM].Value*0.5f)+(CB_BRICK_WIDTH*0.5f);
			BrickPosition.v[YDIM] -= CB_BRICK_LENGTH;
			delta--;
		}
		break;
	case CBNK_USER_DEFINED:
		break;
	default:
		break;
	}
	
#ifdef CHAR_USE_AABB
	CCharacter::UpdateAABB(CB_BRICK_WIDTH*CBNK_MAX_COLUMNS, CB_BRICK_LENGTH*CBNK_MAX_LINES, CB_BRICK_HEIGHT);
#endif
	//Move Bunker to its final position
	Position.Set(Pos.v[XDIM],
				 Pos.v[YDIM],
				 Pos.v[ZDIM]);
};

void CBunker::Init(float x, float y, float z)
{
	Vector Pos;

	Pos.Set(x, y, z);

	Init (CBNK_HOME, Pos);
}

//////////////////////////////////////////////////////////////////////////////////////
//
/// Initialization of a bunker following a given pattern
//
//
void CBunker::Init(Vector &Pos, bool BrickMatrix[CBNK_MAX_LINES][CBNK_MAX_COLUMNS])
{
	int		row, col;
	Vector	P = Pos; 
	PositionPrev=Position;
	P.v[YDIM] += 0.1f;

	UpdateAABB();

	Init(CBNK_USER_DEFINED, Pos);

	for (row=0;row<CBNK_MAX_LINES;row++)
	{
		P.v[XDIM] = Pos.v[XDIM] - 0.02f;	//X reset
		P.v[YDIM] -= 0.01f;					//New Y position
		for(col=0;col<CBNK_MAX_COLUMNS;col++)
		{
			if (BrickMatrix[row][col])
				 Brick[row][col].Activate();
			else Brick[row][col].SetDefault();
			Brick[row][col].MoveTo(P);
		} //Inner for
	}//External  for
}

/**
*	@fn void CBunker::AssignSounds2Bricks()
*	Used when all the sounds are just loaded and assigned to the Bunker
*/
void CBunker::AssignSounds2Bricks()
{
	unsigned int	row, col, s;

	for (row = 0; row<CBNK_MAX_LINES; row++)
	for (col = 0; col < CBNK_MAX_COLUMNS; col++)
	{
		Brick[row][col].SetSoundsAmount(Sound.size());
		for (s = 0; s < Sound.size(); s++)
			Brick[row][col].SetSound(Sound[s], s);
	}
}
///Moving to an absolute position. It does not matter where the bunker is
void CBunker::MoveBricksTo (Vector &Move)
{
	int	row, col;
	Vector RelMove;

	RelMove.v[XDIM] = Move.v[XDIM] - Position.v[XDIM];
	RelMove.v[YDIM] = Move.v[YDIM] - Position.v[YDIM];
	RelMove.v[ZDIM] = Move.v[ZDIM] - Position.v[ZDIM];

	MoveTo(Move);

	for (row=0;row<CBNK_MAX_LINES;row++)
		for(col=0;col<CBNK_MAX_COLUMNS;col++)
			Brick[row][col].MoveRelTo(RelMove);
}

void CBunker::Render(void)
{
	unsigned int row, col;
	CTexturesManager * TextMngr = (CTexturesManager *)Directory[CHARS_TEXTURES_MNGR_REF];
	GLboolean switchBlend = glIsEnabled(GL_BLEND);

	switch(RenderMode)
	{
	case CHAR_NO_RENDER:			//No render for this character
		return;
	case CHAR_2D:
	case CHAR_LINES3D:
	case CHAR_3D:
		if (Alive())
		{
			if (switchBlend)
				glDisable(GL_BLEND);
			glEnable (GL_TEXTURE_2D);
			glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

			glEnable(GL_LIGHTING);			// activate lights on
											//Disable the players lighting
			glEnable(SIGLB_PLAYER_LIGHT);
			//Disable the navy ships lighting
			glDisable(SIGLB_SHIP_LIGHT);
			glDisable(SIGLB_SHIP2_LIGHT);

			switch(RenderMode)
			{
			case CHAR_NO_RENDER:			//No render for this character
				break;
			case CHAR_2D:
			case CHAR_LINES3D:
				TextMngr->Textures[IndTexture2D]->SetTexture();
				break;
			case CHAR_3D:
				TextMngr->Textures[IndTexture3D]->SetTexture();
				break;
			default: break;
			}

			for (row=0;row<CBNK_MAX_LINES;row++)
				for(col=0;col<CBNK_MAX_COLUMNS;col++)
					Brick[row][col].Render();

			if (switchBlend) glEnable(GL_BLEND);
		}
		break;
	default: return;
	}
}

void CBunker::ChangeRenderMode(CHAR_RENDER_MODE Mode)
{
	unsigned int row, col;

	RenderMode = Mode;

	//Bunkers
	for (row=0;row<CBNK_MAX_LINES;row++)
		for(col=0;col<CBNK_MAX_COLUMNS;col++)
			if (Brick[row][col].Active())	Brick[row][col].ChangeRenderMode(RenderMode);
}

//Physics
void CBunker::Collided (CCharacter *CollidedChar)
{
	int row, col;
	CShoot *AuxShoot;

	col = ((CollidedChar->Position.v[XDIM] - (Position.v[XDIM] - (CharAABB.AABB[CHAR_BBSIZE][XDIM].Value * 0.5f))) / CB_BRICK_WIDTH);
	row = ((CollidedChar->Position.v[YDIM] - (Position.v[YDIM] - (CharAABB.AABB[CHAR_BBSIZE][YDIM].Value * 0.5f))) / CB_BRICK_LENGTH);

	if (CHARS_COLLISION_TABLE[CHARS_BUNKER][CollidedChar->Type])	
		switch (CollidedChar->Type)
		{
		case CHARS_SHOOT:
			AuxShoot = (CShoot*) CollidedChar;

			switch (AuxShoot->SubType)
			{
			case CSH_PLAYER2D:			///<Shoot sent by the player by default in 2D mode
			case CSH_PLAYER3D:			///<Shoot sent by the player in 3D mode
			case CSH_PLAYER3D_CHEVRON:	///<Shoot sent by the player
			case CSH_AUX_LASER:			///<Shoot sent by the right and left laser auxiliar player ships
			case CSH_AUX_LASER_CHEVRON:	///<Shoot sent by the right and left laser auxiliar player ships when the player has a Chevron shooting
				//Check bottom-up
				for (row = CBNK_MAX_LINES - 1; row >= 0; row--)
					if (Brick[row][col].Alive())
					{
						Brick[row][col].Type = CHARS_BRICK;
						CollidedChar->Collided(&(CCharacter)Brick[row][col]);
						Brick[row][col].Collided(CollidedChar);
						return;
					}
				break;
			case CSH_SHIP:				///<Shoot sent by a enemy ship. This kind of shoot has to be always after all player's shootings
			case CSH_SUPPLY_SHIP:		///<Shoot sent by a enemy Supply Ship. This kind of shoot has to be always after all player's shootings
			case CSH_CIRCLE_SHIP:
				//Check top-down
				for (row = 0; row < CBNK_MAX_LINES; row++)
					if (Brick[row][col].Alive())
					{
						Brick[row][col].Type = CHARS_BRICK;
						CollidedChar->Collided(&(CCharacter)Brick[row][col]);
						Brick[row][col].Collided(CollidedChar);
						return;
					}
			}
			break;
		default:
			//Detects which brick has been touched
			if (Brick[row][col].Alive())
			{
				Brick[row][col].Collided(CollidedChar);
				CollidedChar->Collided(&(CCharacter)Brick[row][col]);
				return;
			}
		}
}	//End of method

#ifdef CHAR_USE_AABB
///Recalculates its AABB after traversing all the alive bricks
void CBunker::UpdateAABB()
{
	unsigned int file, col;

	ResetAABB();
	for (file=0;file<CBNK_MAX_LINES;file++)
		for (col=0;col<CBNK_MAX_COLUMNS;col++)
			if (Brick[file][col].Alive())
				UpdateAABB(Brick[file][col]);
	
	SetAABBSize(CharAABB.AABB[CHAR_BBMAX][XDIM].Value - CharAABB.AABB[CHAR_BBMIN][XDIM].Value,
				CharAABB.AABB[CHAR_BBMAX][YDIM].Value - CharAABB.AABB[CHAR_BBMIN][YDIM].Value,
				CharAABB.AABB[CHAR_BBMAX][ZDIM].Value - CharAABB.AABB[CHAR_BBMIN][ZDIM].Value);

	CharAABB.AABB[CHAR_BBCENTER][XDIM].Value = CharAABB.AABB[CHAR_BBSIZE][XDIM].Value * 0.5f;
	CharAABB.AABB[CHAR_BBCENTER][YDIM].Value = CharAABB.AABB[CHAR_BBSIZE][YDIM].Value * 0.5f;
	CharAABB.AABB[CHAR_BBCENTER][ZDIM].Value = CharAABB.AABB[CHAR_BBSIZE][ZDIM].Value * 0.5f;

	UpdateCollisionDetection();
}

///Recalculates its AABB given a brick
void CBunker::UpdateAABB(CBrick Brick)
{
	unsigned int i;

	for (i=XDIM;i<UGKALG_DIMENSIONS;i++)
	{
		if (CharAABB.AABB[CHAR_BBMIN][i].Value > Brick.CharAABB.AABB[CHAR_BBMIN][i].Value)
			//Update min value on this dimension
			CharAABB.AABB[CHAR_BBMIN][i].Value = Brick.CharAABB.AABB[CHAR_BBMIN][i].Value;

		if (CharAABB.AABB[CHAR_BBMAX][i].Value < Brick.CharAABB.AABB[CHAR_BBMAX][i].Value)
			//Update max value on this dimension
			CharAABB.AABB[CHAR_BBMAX][i].Value = Brick.CharAABB.AABB[CHAR_BBMAX][i].Value;
	}
}
#endif