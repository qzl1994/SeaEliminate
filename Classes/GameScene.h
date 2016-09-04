#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"

#include "AnimalGrid.h"
#include "SystemHeader.h"
#include "GameOverScene.h"

using namespace ui;

class GameLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	static void addBonus(int bonus);

	virtual void onEnterTransitionDidFinish();
	virtual void onExit();

	CREATE_FUNC(GameLayer);

private:

	AnimalGrid* m_AnimalGrid;

	static LoadingBar* m_bonusbar; // ��ʱ��
	static Label* m_scorelabel;
	static int m_score; // ����

	void onReducingBonus(float dt); //��������ʱ
	void publishScore(); // �洢��Ϸ����
};

#endif