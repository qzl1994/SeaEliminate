#include "LoadingScene.h"

USING_NS_CC;
using namespace CocosDenshion;

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

	auto label = Label::createWithSystemFont("Loading...", "Arial", 48);
	label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(label);

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
	TextureCache::getInstance()->addImageAsync("texture/bonus.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/bonusbar.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/bonusbar_fill.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/gameover.png", addTextureCallback);
	TextureCache::getInstance()->addImageAsync("texture/home.png", addTextureCallback);

	// 预加载音效
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("BGM.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("WBGM.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("gameover.mp3");

	// 开启加载进度检测
	this->schedule(schedule_selector(LoadingLayer::onTextureLoading));
   
    return true;
}

void LoadingLayer::onTextureLoading(float dt)
{
	// 图片加载完毕，进入游戏场景
	if (m_texture_num == 15)
	{
		this->unschedule(schedule_selector(LoadingLayer::onTextureLoading));

		auto call = CallFunc::create([](){
			auto scene = HomeLayer::createScene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
		});

		// 延时进入
		this->runAction(Sequence::create(DelayTime::create(0.5), call, nullptr));
	}
}