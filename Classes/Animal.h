#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include "cocos2d.h"

class Animal : public cocos2d::Sprite
{
public:
	static Animal* createByType();


};

#endif