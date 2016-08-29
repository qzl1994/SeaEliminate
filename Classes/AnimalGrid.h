#ifndef __ANIMAL_GRID_H__
#define __ANIMAL_GRID_H__

#include "cocos2d.h"
#include "Animal.h"

#include "SystemHeader.h"
#include <vector>

using namespace std;

class AnimalGrid : public cocos2d::Node
{
private:
	int m_row;	// 行数
	int m_col;	// 列数

	Animal* m_animalSelected; // 当前选择的动物
	Animal* m_animalSwapped; // 欲交换的动物
	
	vector<vector<Animal*>> m_AnimalGrid; // 存放动物对象的容器

	Animal* createAnimal(int x, int y); // 根据坐标创建一个动物
	bool isAnimalLegal(Animal* animal, int x, int y); // 判断创建的动物是否合法

	void swapAnimals(Animal *animalA, Animal *animalB); // 交换两个动物
	void swapAnimalToNewPos(Animal* animal); // 移动到新位置

	// 事件响应部分
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

public:
	static AnimalGrid* create(int row, int col);
	bool init(int row, int col);

	int getRow() { return m_row; }
	int getCol() { return m_col; }
};

#endif