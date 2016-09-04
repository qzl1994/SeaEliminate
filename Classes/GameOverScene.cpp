#include "GameOverScene.h"

USING_NS_CC;

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

	// ����ͼƬ
	auto bg = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("texture/gameover.png"));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, 0);

	// ������Ϸ��ť
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
	// ������Ϸ
	auto scene = GameLayer::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(2, scene));
}