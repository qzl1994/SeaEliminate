#include "GameScene.h"

USING_NS_CC;

Scene* GameLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = GameLayer::create();

	scene->addChild(layer);

	return scene;
}

bool GameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// ¼ÓÔØ±³¾°Í¼
	auto bg = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("background.png"));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg);



	return true;
}