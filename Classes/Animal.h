#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include "cocos2d.h"

class Animal : public cocos2d::Sprite
{
private:
	// 类型
	int m_type;
	// 坐标
	int m_x;
	int m_y;

public:
	// 根据动物类型以及坐标创建动物
	static Animal* createByType(int type, int x, int y);

	bool init(int type, int x, int y);

	int getType() { return m_type; }
	int getX() { return m_x; }
	int getY() { return m_y; }

};

#endif