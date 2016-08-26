#include "LoadingScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

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

	auto label = Label::createWithSystemFont("Loading...", "Arial", 48);
	label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(label);

	// 初始化加载纹理数
	m_texture_num = 0;
	int *ptexture_num = &m_texture_num;

	//加载完毕回调
	auto addTextureCallback = [ptexture_num](Texture2D* texture)
	{
		(*ptexture_num)++;
	};

	// 异步预加载纹理图片
	TextureCache::getInstance()->addImageAsync("texture/background.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/grid.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal1.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal2.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal3.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal4.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal5.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal6.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal7.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/animal8.png", addTextureCallback);

	// 开启加载进度检测
	this->schedule(schedule_selector(LoadingLayer::onTextureLoading));

	// 预加载音效
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGM.mp3");
    
    return true;
}


void LoadingLayer::onTextureLoading(float dt)
{
	// 一旦图片加载完毕，那么进入游戏场景
	if (m_texture_num == 10)
	{
		this->unschedule(schedule_selector(LoadingLayer::onTextureLoading));

		auto call = CallFunc::create([](){
			auto scene = GameLayer::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
		});

		// 等待一会儿，进入
		this->runAction(Sequence::create(DelayTime::create(0.51), call, nullptr));
	}
}