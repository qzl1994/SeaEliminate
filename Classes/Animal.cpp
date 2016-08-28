#include "Animal.h"

USING_NS_CC;

Animal* Animal::createByType(int type, int x, int y)
{
	auto animal = new Animal();

	if (animal && animal->init(type, x, y))
	{
		animal->autorelease();

		return animal;
	}
	else
	{
		CC_SAFE_DELETE(animal);

		return nullptr;
	}
}

bool Animal::init(int type, int x, int y)
{
	Sprite::init();

	m_type = type;
	m_x = x;
	m_y = y;

	//������Դ����ʼ������
	char name[100] = { 0 };
	sprintf(name, "texture/animal%d.png", m_type);
	this->initWithTexture(TextureCache::getInstance()->getTextureForKey(name));

	this->setAnchorPoint(Vec2(0, 0)); //���½�Ϊê��

	return true;
}