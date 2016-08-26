#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "GameScene.h"

class LoadingLayer : public cocos2d::Layer
{
private:
	int m_texture_num;	// 当前加载的纹理数

	void loadingTextureCallback(cocos2d::Texture2D* texture);

	void onTextureLoading(float dt);
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onExit();
    
	CREATE_FUNC(LoadingLayer);
};

#endif
