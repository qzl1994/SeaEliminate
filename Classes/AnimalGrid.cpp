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

	// 根据行列初始化一个空的宝石容器大小
	m_AnimalGrid.resize(m_row);
	for (auto &vec : m_AnimalGrid)
	{
		vec.resize(m_col);
	}

	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			auto node = Sprite::create("texture/grid.png");
			node->setAnchorPoint(Vec2(0, 0));
			node->setPosition(x*GRID_WIDTH, y*GRID_WIDTH);
			node->setOpacity(100);
			this->addChild(node, 0);


			m_AnimalGrid[x][y] = createAnimal(x, y);


		}
	}



	return true;
}


Animal* AnimalGrid::createAnimal(int x, int y)
{
	Animal* animal = nullptr;

	animal = Animal::createByType(random(1, 8), x, y);

	animal->setPosition(x*GRID_WIDTH, y*GRID_WIDTH);

	this->addChild(animal, 1);

	return animal;
}