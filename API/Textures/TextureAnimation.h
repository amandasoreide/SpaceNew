/* 
	Author: Vicente Broseta Toribio
	Last update: 2013-07
*/

#ifndef TEXTUREANIMATION_H
#define TEXTUREANIMATION_H

#include <UGKTexture.h>
#include <GlobalTiming.h>
#include <vector>

using namespace UGK; 
using namespace std;

#define ANI_DEFAULT_INDEXES	-1

typedef enum{
	ANITYPE_CICLIC,
	ANITYPE_PINGPONG,
	ANITYPE_SERIAL,
	MAX_NUM_ANI_TYPES
}ANI_ANIMATION_TYPE;

typedef struct{
	int IndexInMngr;
	UGKS_String Name;
}AnimationId;

class CTextureAnimation {
public:
	vector <CTexture*>	Photograms;
	unsigned int		AniTimerIndex;
	cHRTimer			*AniTimer;
	ANI_ANIMATION_TYPE	Type;
	AnimationId			Id;

	CTextureAnimation	();
	~CTextureAnimation	();

	inline void AddPhotogram	(CTexture *dest){Photograms.push_back(dest);}
	void AddPhotogram			(CTexture *dest, unsigned int photogramNum);
	void UpdateAnimation		(void);
	void SetCurrentPhotogram	(void);

	inline void		SetPeriod		(double period)	{AniPeriod = AniTimer->ms2Ticks(period);}
	inline double	GetPeriod		(void)			{return AniTimer->Ticks2ms(AniPeriod);}
	inline void		Start			(void)			{AnimationCounter=0; running = true; AniTimer->InitCounting();}
	inline void		Pause			(void)			{running = !running;}
	inline void		Stop			(void)			{running = false; AniTimer->Reset();}

private:
	unsigned int AnimationCounter;
	HRT_Time AniPeriod;
	bool running;
};

#endif // TEXTUREANIMATION.H