/* 
	Author: Vicente Broseta Toribio
	Last update: 2013-06
*/

#include <TextureAnimationManager.h>


CTextureAnimationManager::CTextureAnimationManager(){
	
}

CTextureAnimationManager::~CTextureAnimationManager(){
	
}

int CTextureAnimationManager::CreateAnimation(double period, ANI_ANIMATION_TYPE type){

	CTextureAnimation *aniAux;
	aniAux = new (CTextureAnimation);

	aniAux->SetPeriod(period);
	aniAux->Type = type;
	aniAux->Id.IndexInMngr = Animations.size();
	aniAux->Id.Name.clear();

	Animations.push_back(aniAux);

	return aniAux->Id.IndexInMngr;
}

int CTextureAnimationManager::CreateAnimation(double period, ANI_ANIMATION_TYPE type, unsigned int indexList){

	CTextureAnimation *aniAux;
	aniAux = new (CTextureAnimation);

	aniAux->SetPeriod(period);
	aniAux->Type = type;
	aniAux->Id.IndexInMngr = indexList;
	aniAux->Id.Name.clear();

	if(indexList < Animations.size()){	
		Animations[indexList] = aniAux;
	}else if(indexList == Animations.size()){
		Animations.push_back(aniAux);
	}else{
		Animations.resize(indexList);
		Animations.push_back(aniAux);
	}

	return aniAux->Id.IndexInMngr;
}

int CTextureAnimationManager::CreateAnimation(UGKS_String name, double period, ANI_ANIMATION_TYPE type)
{
	CTextureAnimation *aniAux;
	aniAux = new (CTextureAnimation);

	aniAux->SetPeriod(period);
	aniAux->Type = type;
	aniAux->Id.IndexInMngr = Animations.size();

	aniAux->Id.Name = name;
	Animations.push_back(aniAux);

	return aniAux->Id.IndexInMngr;
}

int CTextureAnimationManager::CreateAnimation(UGKS_String name, double period, ANI_ANIMATION_TYPE type, unsigned int indexList)
{
	CTextureAnimation *aniAux;
	aniAux = new (CTextureAnimation);

	aniAux->SetPeriod(period);
	aniAux->Type = type;
	aniAux->Id.IndexInMngr = indexList;

	aniAux->Id.Name = name;

	if(indexList < Animations.size()){	
		Animations[indexList] = aniAux;
	}else if(indexList == Animations.size()){
		Animations.push_back(aniAux);
	}else{
		Animations.resize(indexList);
		Animations.push_back(aniAux);
	}

	return aniAux->Id.IndexInMngr;
}

int CTextureAnimationManager::SearchIndOfName(UGKS_String name)
{
	vector<CTextureAnimation*>::iterator it;
	UGKS_String s2;

	//s1 = name;
	for(it=Animations.begin();it!=Animations.end();++it){
		if(!(*it)->Id.Name.empty()){
			s2 = (*it)->Id.Name;
			if(0 == s2.compare(name)) return (*it)->Id.IndexInMngr;
		}
	}

	return ANI_DEFAULT_INDEXES;
}

