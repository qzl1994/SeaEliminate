#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* GameOverLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = GameOverLayer::create();

	scene->addChild(layer);

	return scene;
}

bool GameOverLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	// 背景图片
	auto bg = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("texture/gameover.png"));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, 0);

	// 返回游戏按钮
	auto backlabel = Label::createWithTTF("Try again", "fonts/Marker Felt.ttf", 36);
	backlabel->setColor(Color3B::BLACK);
	auto backItem = MenuItemLabel::create(backlabel);
	backItem->setTarget(this, menu_selector(GameOverLayer::onBackItemCallback));
	auto menu = Menu::create(backItem, nullptr);
	this->addChild(menu);

	return true;
}

void GameOverLayer::onBackItemCallback(Ref* pSender)
{
	// 返回游戏
	auto scene = GameLayer::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}

void GameOverLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("gameover.mp3", true);
}

void GameOverLayer::onExit()
{
	Layer::onExit();

	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}