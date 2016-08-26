#include "LoadingScene.h"

USING_NS_CC;

Scene* LoadingLayer::createScene()
{
    auto scene = Scene::create();
    
	auto layer = LoadingLayer::create();

    scene->addChild(layer);

    return scene;
}

bool LoadingLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithSystemFont("Loading...", "Arial", 36);
	label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(label);

	// ��ʼ������������
	m_texture_num = 0;

	// ��������ͼƬ
	Director::getInstance()->getTextureCache()->addImageAsync("sea_sheet.png",
		CC_CALLBACK_1(LoadingLayer::loadingTextureCallback, this));

	// �첽Ԥ���ر���ͼƬ
	TextureCache::getInstance()->addImage("background.png");
	TextureCache::getInstance()->addImage("board.png");

    
    return true;
}

// ��������ͼƬ��Ӧ֡����
void LoadingLayer::loadingTextureCallback(Texture2D* texture)
{
	switch (m_texture_num++)
	{
	case 0:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("sea_sheet.plist", texture);

		this->schedule(schedule_selector(LoadingLayer::onTextureLoading), 1, 1, 3);

		break;
	default:
		break;
	}
}

void LoadingLayer::onTextureLoading(float dt)
{
	auto scene = GameLayer::createScene();

	Director::getInstance()->replaceScene(scene);
}

void LoadingLayer::onExit()
{
	Layer::onExit();

	this->unschedule(schedule_selector(LoadingLayer::onTextureLoading));
}