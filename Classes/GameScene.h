#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "AnimalGrid.h"

#define ROW 8
#define COL 8

class GameLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onEnterTransitionDidFinish();

	CREATE_FUNC(GameLayer);


private:

	AnimalGrid* m_AnimalGrid;

};

#endif