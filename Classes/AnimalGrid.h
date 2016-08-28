#ifndef __ANIMAL_GRID_H__
#define __ANIMAL_GRID_H__

#include "cocos2d.h"
#include "Animal.h"

#include "SystemHeader.h"
#include <vector>

using namespace std;

class Animal;

class AnimalGrid : public cocos2d::Node
{
private:
	int m_row;	// 行数
	int m_col;	// 列数

	Animal* m_animalSelected; //当前选择的宝石
	
	vector<vector<Animal*>> m_AnimalGrid; // 存放宝石对象的容器

	Animal* createAnimal(int x, int y); // 根据坐标创建一个动物
	bool isAnimalLegal(Animal* animal, int x, int y); // 判断创建的动物是否合法


	// 事件响应部分
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);


public:
	static AnimalGrid* create(int row, int col);
	bool init(int row, int col);

	int getRow() { return m_row; }
	int getCol() { return m_col; }

};

#endif