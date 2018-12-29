/* 
	Author: Vicente Broseta Toribio
	Last update: 2013-07
*/

#include <TextureAnimation.h>
#include <SIGame.h>

extern CSIGame	*Game; // The game singleton

CTextureAnimation::CTextureAnimation(){
	Id.IndexInMngr = ANI_DEFAULT_INDEXES;
	Id.Name.clear();
	AnimationCounter = 0;
	AniPeriod = HRT_NOTIME;
	AniTimerIndex = TimerManager.NewTimer();
	AniTimer = TimerManager.GetTimer(AniTimerIndex);
	AniTimer->Activate();
	Type = ANITYPE_CICLIC;
	running = false;
}

CTextureAnimation::~CTextureAnimation(){
	AniTimer->DeActivate();
}


void CTextureAnimation::AddPhotogram(CTexture *dest, unsigned int photogramNum){

	if(photogramNum < Photograms.size()){	
		Photograms[photogramNum] = dest;
	}else if(photogramNum == Photograms.size()){
		Photograms.push_back(dest);
	}else{
		Photograms.resize(photogramNum);
		Photograms.push_back(dest);
	}

}

bool goingUp = true;
void CTextureAnimation::UpdateAnimation(void){

	if(running){

		AniTimer->EndCounting();
		if(AniTimer->GetLastPeriod() >= AniPeriod){

			AniTimer->Reset();
			AniTimer->InitCounting();

			switch (Type)
			{
			case ANITYPE_CICLIC:
				AnimationCounter = (AnimationCounter+1)%Photograms.size();
				break;
			case ANITYPE_PINGPONG:
				if(goingUp){
					if(AnimationCounter < Photograms.size()-1)
						AnimationCounter++;
					else{
						AnimationCounter--;
						goingUp = false;
					}
				}else{
					if(AnimationCounter > 0)
						AnimationCounter--;
					else{
						AnimationCounter++;
						goingUp = true;
					}
				}
				break;
			case ANITYPE_SERIAL:
				if(AnimationCounter < Photograms.size()-1)
					AnimationCounter++;
				break;
			}//switch Type
		}// If Timer
	}//If running	
}

void CTextureAnimation::SetCurrentPhotogram(void){
	if(Photograms[AnimationCounter]->Loaded)
		Photograms[AnimationCounter]->SetTexture();
}
