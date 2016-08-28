#include "AnimalGrid.h"

USING_NS_CC;

AnimalGrid* AnimalGrid::create(int row, int col)
{
	auto animalGrid = new AnimalGrid();

	if (animalGrid&&animalGrid->init(row, col))
	{
		animalGrid->autorelease();

		return animalGrid;
	}
	else
	{
		CC_SAFE_DELETE(animalGrid);

		return nullptr;
	}
}

bool AnimalGrid::init(int row, int col)
{
	Node::init();

	m_row = row;
	m_col = col;

	// �������г�ʼ��һ���յı�ʯ������С
	m_AnimalGrid.resize(m_row);
	for (auto &vec : m_AnimalGrid)
	{
		vec.resize(m_col);
	}

	// ���ݲ��ִ�С����������
	for (int x = 0; x < m_row; x++)
	{
		for (int y = 0; y < m_col; y++)
		{
			// ͸�����ӱ���
			auto node = Sprite::create("texture/grid.png");
			node->setAnchorPoint(Vec2(0, 0));
			node->setPosition(x*GRID_WIDTH, y*GRID_WIDTH);
			node->setOpacity(100);
			this->addChild(node, 0);

			// Ϊ����������䶯�����
			m_AnimalGrid[x][y] = createAnimal(x, y);
		}
	}

	// ���봥������
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(AnimalGrid::onTouchBegan, this);

	// ��Ӵ����¼�������
	EventDispatcher * eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

// �������һ������
Animal* AnimalGrid::createAnimal(int x, int y)
{
	Animal* animal = nullptr;

	while (1)
	{
		animal = Animal::createByType(random(1, ANIMAL_NUM), x, y);

		if (isAnimalLegal(animal, x, y))
		{
			break;
		}
	}

	animal->setPosition(x*GRID_WIDTH, y*GRID_WIDTH);

	this->addChild(animal, 1);

	return animal;
}

// �жϲ��ֺϷ�
bool AnimalGrid::isAnimalLegal(Animal* animal, int x, int y)
{
	bool isXLegal = true;
	bool isYLegal = true;

	if (x > 1)
	{
		if (animal->getType() == m_AnimalGrid[x - 1][y]->getType() && 
			animal->getType() == m_AnimalGrid[x - 2][y]->getType())
		{
			isXLegal = false;
		}
		else{
			isXLegal = true;
		}
	}

	if (y > 1)
	{
		if (animal->getType() == m_AnimalGrid[x][y - 1]->getType() &&
			animal->getType() == m_AnimalGrid[x][y - 2]->getType())
		{
			isYLegal = false;
		}
		else{
			isYLegal = true;
		}
	}

	return isXLegal && isYLegal;
}

bool AnimalGrid::onTouchBegan(Touch* touch, Event* event)
{
	// ������������ת��Ϊģ������
	auto pos = this->convertToNodeSpace(touch->getLocation());

	// �Ƿ��ڲ�����
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos))
	{
		// �õ���������
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;

		// �õ���ǰѡ�еĶ���
		m_animalSelected = m_AnimalGrid[x][y];

		log("touch coordinate: x=%d,y=%d animal's type:%d", x+1, y+1, m_animalSelected->getType());

		return true;
	}
	else
	{
		return false;
	}
}