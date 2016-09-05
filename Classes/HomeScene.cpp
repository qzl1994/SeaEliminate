#include "HomeScene.h"

USING_NS_CC;

Scene* HomeLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = HomeLayer::create();

	scene->addChild(layer);

	return scene;
}

bool HomeLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	// ±³¾°Í¼Æ¬
	auto bg = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("texture/home.png"));
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(bg, 0);

	auto label = Label::createWithTTF("Tap the Screen to Play", "fonts/Marker Felt.ttf", 36);
	label->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(label);

	// ×¢²á´¥ÃþÊÂ¼þ¼àÌýÆ÷
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [](Touch* touch, Event* event){

		auto scene = TransitionFade::create(1.0f, GameLayer::createScene());

		Director::getInstance()->replaceScene(scene);

		return false;
	};

	// Ìí¼Ó´¥ÃþÊÂ¼þ¼àÌýÆ÷
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void HomeLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("WBGM.mp3", true);
}

void HomeLayer::onExit()
{
	Layer::onExit();

	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}