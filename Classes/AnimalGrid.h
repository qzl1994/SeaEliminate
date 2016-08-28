#ifndef __ANIMAL_GRID_H__
#define __ANIMAL_GRID_H__

#include "cocos2d.h"
#include "Animal.h"

#include <vector>

using namespace std;


#define GRID_WIDTH 60 // һ����������Ϊ60


class AnimalGrid : public cocos2d::Node
{
private:
	int m_row;	// ����
	int m_col;	// ����

	vector<vector<Animal*>> m_AnimalGrid; //��ű�ʯ���������



	Animal* createAnimal(int x, int y); // �������괴��һ������

public:
	static AnimalGrid* create(int row, int col);

	bool init(int row, int col);


	int getRow() { return m_row; }
	int getCol() { return m_col; }

};

#endif