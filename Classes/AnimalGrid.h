#ifndef __ANIMAL_GRID_H__
#define __ANIMAL_GRID_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Animal.h"
#include "SystemHeader.h"
#include <vector>

using namespace std;

class AnimalGrid : public cocos2d::Node
{
private:
	int m_row;	// ����
	int m_col;	// ����

	Animal* m_animalSelected; // ��ǰѡ��Ķ���
	Animal* m_animalSwapped; // �������Ķ���
	
	vector<vector<Animal*>> m_AnimalGrid; // ��Ŷ�����������

	// ���ڴ�������ķ���
	Animal* createAnimal(int x, int y); // �������괴��һ������
	bool isAnimalLegal(Animal* animal, int x, int y); // �жϴ����Ķ����Ƿ�Ϸ�

	// ���ڲ�������ķ���
	void swapAnimals(Animal *animalA, Animal *animalB); // ������������
	void swapAnimalToNewPos(Animal* animal); // �ƶ�����λ��

	// �¼���Ӧ����
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);

	// ��Ϸ�߼�����
	bool canCrush(); // �жϵ�ǰ״̬�Ķ��������Ƿ�������
	void goCrush(); // ��ʼ����

	// ��׽��������׽���������Ƿ����
	void onAnimalsSwaping(float dt);
	void onAnimalsSwapingBack(float dt);

public:
	static AnimalGrid* create(int row, int col);
	bool init(int row, int col);

	int getRow() { return m_row; }
	int getCol() { return m_col; }
};

#endif