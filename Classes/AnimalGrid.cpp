#include "AnimalGrid.h"

USING_NS_CC;
using namespace CocosDenshion;

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

	m_animalSelected = nullptr;
	m_animalSwapped = nullptr;

	// 根据行列初始化一个空的宝石容器大小
	m_AnimalGrid.resize(m_row);
	for (auto &vec : m_AnimalGrid)
	{
		vec.resize(m_col);
	}

	// 根据布局大小创建出阵列
	for (int x = 0; x < m_row; x++)
	{
		for (int y = 0; y < m_col; y++)
		{
			// 透明格子背景
			auto node = Sprite::create("texture/grid.png");
			node->setAnchorPoint(Vec2(0, 0));
			node->setPosition(x*GRID_WIDTH, y*GRID_WIDTH);
			node->setOpacity(100);
			this->addChild(node, NODE_ZORDER);

			// 为动物容器填充动物对象
			m_AnimalGrid[x][y] = createAnimal(x, y);
		}
	}

	while (isDeadMap())
	{
		updateMap();
	}

	// 加入触摸监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(AnimalGrid::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(AnimalGrid::onTouchMoved, this);

	// 添加触摸事件监听器
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

// 判断布局是否是死图
bool AnimalGrid::isDeadMap()
{
	// 模拟交换，判断交换后是否能消除，如不能，那么就是个死图
	auto swap = [](Animal** a, Animal** b)
	{
		auto temp = *a;
		*a = *b;
		*b = temp;
	};

	bool isDeadMap = true;

	// 遍历每一个动物
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			// 跟左边的交换
			if (x > 0)
			{
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x - 1][y]);
				if (canCrush())
				{
					isDeadMap = false;
				}
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x - 1][y]);
			}

			// 跟右边的交换
			if (x < m_col - 1)
			{
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x + 1][y]);
				if (canCrush())
				{
					isDeadMap = false;
				}
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x + 1][y]);
			}

			// 跟上面的交换
			if (y < m_row - 1)
			{
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x][y + 1]);
				if (canCrush())
				{
					isDeadMap = false;
				}
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x][y + 1]);
			}

			// 跟下面的交换
			if (y > 0)
			{
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x][y - 1]);
				if (canCrush())
				{
					isDeadMap = false;
				}
				swap(&m_AnimalGrid[x][y], &m_AnimalGrid[x][y - 1]);
			}
		}
	}

	m_crushAnimalGrid.clear();

	return isDeadMap;
}

// 刷新布局
void AnimalGrid::updateMap()
{
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row; y++)
		{
			m_AnimalGrid[x][y]->removeFromParent();
			m_AnimalGrid[x][y] = createAnimal(x, y);
		}
	}
}

// 随机创建一个动物
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

	this->addChild(animal, ANIMAL_ZORDER);

	return animal;
}

// 判断布局合法
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

// 触摸事件
bool AnimalGrid::onTouchBegan(Touch* touch, Event* event)
{
	// 将触摸点坐标转化为模型坐标
	auto pos = this->convertToNodeSpace(touch->getLocation());

	// 是否按在布局上
	if (Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos))
	{
		// 得到布局坐标
		int x = pos.x / GRID_WIDTH;
		int y = pos.y / GRID_WIDTH;

		// 得到当前选中的动物
		m_animalSelected = m_AnimalGrid[x][y];
		
		return true;
	}
	else
	{
		return false;
	}
}

void AnimalGrid::onTouchMoved(Touch* touch, Event* event)
{
	if (!m_animalSelected)
	{
		return;
	}

	// 已选择动物的坐标
	int startX = m_animalSelected->getX();
	int startY = m_animalSelected->getY();

	// 触摸点的布局坐标
	auto pos = this->convertToNodeSpace(touch->getLocation());
	int touchX = pos.x / GRID_WIDTH;
	int touchY = pos.y / GRID_WIDTH;

	// 如果触摸点不在布局内，或者触摸点布局坐标和已选动物布局坐标一样，那么返回
	if (!Rect(0, 0, m_col*GRID_WIDTH, m_row*GRID_WIDTH).containsPoint(pos) || 
		Vec2(touchX, touchY) == Vec2(startX, startY))
	{
		return;
	}

	// 判断已选动物的布局坐标与触摸点的布局坐标是否直角相隔一个单位
	if (abs(startX - touchX) + abs(startY - touchY) != 1)
	{
		return;
	}

	// 余下的情况，触摸点上面的动物就是欲进行交换的动物
	// 获取欲交换的动物
	m_animalSwapped = m_AnimalGrid[touchX][touchY];

	// 交换动物
	swapAnimals(m_animalSelected, m_animalSwapped);

	// 开启交换状态捕捉函数
	this->schedule(schedule_selector(AnimalGrid::onAnimalsSwaping));

}

// 交换动物
void AnimalGrid::swapAnimals(Animal *animalA, Animal *animalB)
{
	_eventDispatcher->pauseEventListenersForTarget(this); // 交换开始，关闭触摸监听

	auto temp = m_AnimalGrid[animalA->getX()][animalA->getY()];
	m_AnimalGrid[animalA->getX()][animalA->getY()] = m_AnimalGrid[animalB->getX()][animalB->getY()];
	m_AnimalGrid[animalB->getX()][animalB->getY()] = temp;

	auto tempX = animalA->getX();
	animalA->setX(animalB->getX());
	animalB->setX(tempX);

	auto tempY = animalA->getY();
	animalA->setY(animalB->getY());
	animalB->setY(tempY);

	swapAnimalToNewPos(animalA);
	swapAnimalToNewPos(animalB);
}

// 动物移动位置
void AnimalGrid::swapAnimalToNewPos(Animal* animal)
{
	// 设置动物交换状态为真，移动结束再设置为假
	animal->setSwapingState(true);

	auto move = MoveTo::create(MOVE_SPEED, Vec2(animal->getX() * GRID_WIDTH, animal->getY() * GRID_WIDTH));
	auto call = CallFunc::create([animal](){
		animal->setSwapingState(false);
	});

	animal->runAction(Sequence::create(move, call, nullptr));
}

// 判断消除
bool AnimalGrid::canCrush()
{
	int count = 0; // 连续数
	Animal *AnimalBegin = nullptr; // 起始遍历的动物
	Animal *AnimalNext = nullptr; // 从起始动物开始往前遍历的动物

	// 遍历每一列
	for (int x = 0; x < m_col; x++)
	{
		for (int y = 0; y < m_row - 1;)
		{
			count = 1;
			AnimalBegin = m_AnimalGrid[x][y];
			AnimalNext = m_AnimalGrid[x][y + 1];

			// 如果连续出现同类型
			while (AnimalBegin->getType() == AnimalNext->getType())
			{
				count++;
				int nextIndex = y + count;
				if (nextIndex > m_row - 1)
				{
					break;
				}
				AnimalNext = m_AnimalGrid[x][nextIndex];
			}
			// 如果连续数大于等于3，那么遍历的这些动物应当消除，把它们存入消除动物盒子
			if (count >= 3)
			{
				for (int n = 0; n < count; n++)
				{
					auto animal = m_AnimalGrid[x][y + n];
					m_crushAnimalGrid.pushBack(animal);
				}
			}
			y += count;
		}
	}

	// 遍历每一行
	for (int y = 0; y < m_row; y++)
	{
		for (int x = 0; x < m_col - 1;)
		{
			count = 1;
			AnimalBegin = m_AnimalGrid[x][y];
			AnimalNext = m_AnimalGrid[x + 1][y];

			while (AnimalBegin->getType() == AnimalNext->getType())
			{
				count++;
				int nextIndex = x + count;
				if (nextIndex > m_col - 1)
					break;
				AnimalNext = m_AnimalGrid[nextIndex][y];
			}

			if (count >= 3)
			{
				for (int n = 0; n < count; n++)
				{
					auto animal = m_AnimalGrid[x + n][y];

					// 有可能有动物同时行列可消除，那么不能重复存储到消除动物盒子，故需添加一次判断
					if (m_crushAnimalGrid.find(animal) != m_crushAnimalGrid.end())
					{
						continue;
					}
					m_crushAnimalGrid.pushBack(animal);
				}
			}
			x += count;
		}
	}

	// 如果消除动物盒子不为空，那么说明该阵列可消除，返回真
	if (!m_crushAnimalGrid.empty())
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 标记传入的动物消除状态
void AnimalGrid::singeAnimal(Animal* animal)
{
	int row = animal->getX();
	int col = animal->getY();

	m_crushAnimalH.pushBack(animal);
	m_crushAnimalV.pushBack(animal);

	// 判断动物左边是否相同
	while (col>1)
	{
		col = col - 1;

		auto animal_left = m_AnimalGrid[animal->getX()][col];

		if (animal_left && animal_left->getType() == animal->getType())
		{
			m_crushAnimalH.pushBack(animal_left);
		}
		else
		{
			break;
		}
	}

	// 判断动物右边
	if (animal->getY() != m_col)
	{
		for (int i = animal->getY() + 1; i < m_col; i++)
		{
			auto animal_right = m_AnimalGrid[animal->getX()][i];

			if (animal_right && animal_right->getType() == animal->getType())
			{
				m_crushAnimalH.pushBack(animal_right);
			}
			else
			{
				break;
			}
		}
	}

	// 判断动物上面
	if (animal->getX() != m_row)
	{
		for (int i = animal->getX() + 1; i < m_row; i++)
		{
			auto animal_up = m_AnimalGrid[i][animal->getY()];

			if (animal_up && animal_up->getType() == animal->getType())
			{
				m_crushAnimalV.pushBack(animal_up);
			}
			else
			{
				break;
			}
		}

	}

	// 判断动物下面
	while (row > 1)
	{
		row = row - 1;

		auto animal_down = m_AnimalGrid[row][animal->getY()];

		if (animal_down && animal_down->getType() == animal->getType())
		{
			m_crushAnimalV.pushBack(animal_down);
		}
		else
		{
			break;
		}
	}

	// 判断横向容器，标记待消除动物
	if (m_crushAnimalH.size() < 3)
	{
		m_crushAnimalH.clear();
	}
	else
	{

		for (auto animalH : m_crushAnimalH)
		{
			animalH->setClean(true);
		}
	}
	// 判断纵向容器，标记待消除动物
	if (m_crushAnimalV.size() < 3)
	{
		m_crushAnimalV.clear();
	}
	else
	{

		for (auto animalV : m_crushAnimalV)
		{
			animalV->setClean(true);
		}
	}

	// 判断4消
	if (m_crushAnimalH.size() == 4 || m_crushAnimalV.size() == 4)
	{
		log("4xiao");
		bool crush = true;

		for (auto animalH : m_crushAnimalH)
		{
			if (animalH->getSpecial() > 0)
			{
				crush = false;
			}
		}

		for (auto animalV : m_crushAnimalV)
		{
			if (animalV->getSpecial() > 0)
			{
				crush = false;
			}
		}

		if (crush)
		{
			if (m_crushAnimalH.size() == 4 && m_crushAnimalV.size() < 4)
			{
				animal->setSpeicial(1);
			}
			else if (m_crushAnimalV.size() == 4 && m_crushAnimalH.size() < 4)
			{
				animal->setSpeicial(2);
			}
		}

	}

	// 判断T,L型消
	if (m_crushAnimalH.size() + m_crushAnimalV.size() >= 6)
	{
		log("Txiao");
		for (auto animalH : m_crushAnimalH)
		{
			if (animalH->getSpecial() > 0)
			{
				animalH->setSpeicial(0);
			}
		}

		for (auto animalV : m_crushAnimalV)
		{
			if (animalV->getSpecial() > 0)
			{
				animalV->setSpeicial(0);
			}
			
		}

		animal->setSpeicial(3);
	}

	// 判断5消
	if (m_crushAnimalH.size() == 5 || m_crushAnimalV.size() == 5)
	{
		log("5xiao");
		for (auto animalH : m_crushAnimalH)
		{
			if (animalH->getSpecial() > 0)
			{
				animalH->setSpeicial(0);
			}
		}

		for (auto animalV : m_crushAnimalV)
		{
			if (animalV->getSpecial() > 0)
			{
				animalV->setSpeicial(0);
			}

		}

		animal->setSpeicial(4);
	}

	// 遍历横向容器，为有特殊消标记的动物做处理
	for (auto animalH : m_crushAnimalH)
	{
		if (animalH->isNeedClean() && (animalH->getSpecial() > 0))
		{
			this->specialSinged(animalH);
		}
	}
	// 遍历纵向容器，为有特殊消标记的动物做处理
	for (auto animalV : m_crushAnimalV)
	{
		if (animalV->isNeedClean() && (animalV->getSpecial() > 0))
		{
			this->specialSinged(animalV);
		}
	}

	m_crushAnimalH.clear();
	m_crushAnimalV.clear();

}

// 特殊消标记
void AnimalGrid::specialSinged(Animal* animal)
{
	// 消除一整列
	if (animal->getSpecial() == 1)
	{
		for (int i = 0; i < m_row; i++)
		{
			auto animal_V = m_AnimalGrid[i][animal->getY()];
			animal_V->setClean(true);

			if (animal_V->getSpecial()>0)
			{
				specialSinged(animal_V);
			}
		}
	}

	// 消除一整行
	if (animal->getSpecial() == 2)
	{
		for (int i = 0; i < m_col; i++)
		{
			auto animal_H = m_AnimalGrid[animal->getX()][i];
			animal_H->setClean(true);

			if (animal_H->getSpecial() > 0)
			{
				specialSinged(animal_H);
			}
		}
	}

	// 菱形消除
	if (animal->getSpecial() == 3)
	{
		for (int x = 0; x < m_row; x++)
		{
			for (int y = 0; y < m_col; y++)
			{
				auto animal_L = m_AnimalGrid[x][y];

				if ((abs(animal_L->getX() - animal->getX()) + 
					abs(animal_L->getY() - animal->getY())) < 3)
				{
					animal_L->setClean(true);

					if (animal_L->getSpecial() > 0)
					{
						specialSinged(animal_L);
					}
				}
			}
		}
	}

	// 消除同一类型
	if (animal->getSpecial() == 4)
	{
		for (int x = 0; x < m_row; x++)
		{
			for (int y = 0; y < m_col; y++)
			{
				auto animalF = m_AnimalGrid[x][y];

				if (animalF && animalF->getType() == animal->getType())
				{
					animalF->setClean(true);
				}
			}
		}
	}
}

// 遍历全局，是否有消除标记的动物
bool AnimalGrid::checkGridClean()
{
	for (int i = 0; i < m_row; i++)
	{
		for (int j = 0; j < m_col; j++)
		{
			if (m_AnimalGrid[i][j]->isNeedClean())
			{
				return true;
			}
		}
	}

	return false;
}


//	开始消除
void AnimalGrid::goCrush()
{
	// 遍历消除动物盒子
	for (auto animal : m_crushAnimalGrid)
	{
		// 生成新的动物，初始位置在布局外一格
		auto newAnimal = Animal::createByType(random(1, ANIMAL_NUM), animal->getX(), m_row);
		newAnimal->setPosition(newAnimal->getX()*GRID_WIDTH, newAnimal->getY()*GRID_WIDTH);
		this->addChild(newAnimal);

		// 将新动物放到新动物盒子内，等待加入布局
		m_newAnimalGrid.pushBack(newAnimal);

		// 动物盒子内应当刷新的动物暂时置为空
		m_AnimalGrid[animal->getX()][animal->getY()] = nullptr;

		// 原有动物对象消除
		animal->crush();
	}
}

// 刷新布局
void AnimalGrid::refreshAnimalGrid()
{
	// 遍历列，如果该列有空位，那么应当刷新
	for (int x = 0; x < m_col; x++)
	{
		int empty_count = 0; // 一列总的空格子数

		for (int y = 0; y < m_row; y++)
		{
			// 根据坐标索引动物盒子内的动物指针，如果为空，那么说明该坐标位置为空
			auto animal = m_AnimalGrid[x][y];
			if (!animal)
			{
				empty_count++;
			}		
		}

		if (empty_count)
		{
			// 找到有空位的列，刷新该列的动物
			refreshAnimalsToNewPos(x);
		}
	}
}

// 动物掉落
void AnimalGrid::refreshAnimalsToNewPos(int col)
{
	// 刷新该列上面的动物
	int n = 0; // 当前遍历到的空位数
	auto p_animalBox = &m_AnimalGrid; // 保存一个动物盒子的指针，这是为了让其能传入lamda

	// 先让现有的动物下落
	for (int y = 0; y < m_row; y++)
	{
		auto animal = m_AnimalGrid[col][y];

		if (!animal)
		{
			n++;
			continue;
		}
		else if (n != 0)
		{
			animal->setY(animal->getY() - n);
			auto move = MoveBy::create(0.2, Vec2(0, -n*GRID_WIDTH));
			auto call = CallFunc::create([p_animalBox, animal](){
				// 更新动物盒子内的数据
				(*p_animalBox)[animal->getX()][animal->getY()] = animal;
			});

			animal->runAction(Sequence::create(move, call, nullptr));
		}
	}

	// 再让新动物下落
	int i = n;
	int delta = 1;

	for (auto animal : m_newAnimalGrid)
	{
		if (animal->getX() == col)
		{
			animal->setY(m_row - i);

			auto delay = DelayTime::create(0.2);
			// 后下落的速度设置慢一些
			auto move = MoveBy::create(0.2*delta++, Vec2(0, -i--*GRID_WIDTH));
			auto call = CallFunc::create([animal, p_animalBox, this](){
				(*p_animalBox)[animal->getX()][animal->getY()] = animal;
				// 从新动物盒子中移除该动物
				m_newAnimalGrid.eraseObject(animal);
			});

			animal->runAction(Sequence::create(delay, move, call, nullptr));
		}
	}
}

// 捕捉交换状态
void AnimalGrid::onAnimalsSwaping(float dt)
{
	// 捕捉两个正在交换的动物的交换动作是否已经停止，如果没停止，返回继续捕捉
	if (m_animalSelected->isSwaping() || m_animalSwapped->isSwaping())
	{
		return;
	}
	else
	{
		// 停止捕捉
		this->unschedule(schedule_selector(AnimalGrid::onAnimalsSwaping));

		// 判断当前状态是否可以消除
		if (canCrush())
		{
			m_animalSelected = nullptr;

			SimpleAudioEngine::getInstance()->playEffect("cursh.mp3");
			goCrush();
			this->schedule(schedule_selector(AnimalGrid::onAnimalsCrushing));
		}
		else
		{
			// 不能消除，交换回去，开启交换返回时的捕捉函数
			swapAnimals(m_animalSelected, m_animalSwapped);
			this->schedule(schedule_selector(AnimalGrid::onAnimalsSwapingBack));
		}
	}
}

// 复位
void AnimalGrid::onAnimalsSwapingBack(float dt)
{
	// 捕捉两个正在交换的动物的交换动作是否已经停止，如果没停止，返回继续捕捉
	if (m_animalSelected->isSwaping() || m_animalSwapped->isSwaping())
	{
		return;
	}
	else
	{
		this->unschedule(schedule_selector(AnimalGrid::onAnimalsSwapingBack)); // 停止捕捉

		m_animalSelected = nullptr;

		_eventDispatcher->resumeEventListenersForTarget(this); // 重新开始触摸监听
	}
}

void AnimalGrid::onAnimalsCrushing(float dt)
{
	// 捕捉动物消除状态，如果有动物还在消除，那么继续捕捉
	for (auto animal : m_crushAnimalGrid)
	{
		if (animal->isCrushing())
		{
			return;
		}
	}

	// 如果全部动物已经消除完毕，停止捕捉函数
	unschedule(schedule_selector(AnimalGrid::onAnimalsCrushing));

	// 清空消除动物盒子
	m_crushAnimalGrid.clear(); 

	// 刷新动物阵列，并开启刷新状态捕捉函数
	refreshAnimalGrid();
	this->schedule(schedule_selector(AnimalGrid::onAnimalsRefreshing));
}

void AnimalGrid::onAnimalsRefreshing(float dt)
{
	// 捕捉动物刷新状态，如果新动物盒子还有动物，那么继续捕捉
	if (m_newAnimalGrid.size() != 0)
	{
		return;
	}
	else
	{
		unschedule(schedule_selector(AnimalGrid::onAnimalsRefreshing));

		if (canCrush())
		{
			// 如果能消除，那么继续消除
			goCrush();
			this->schedule(schedule_selector(AnimalGrid::onAnimalsCrushing));
		}
		else
		{

			// 判断是否为死图，如果是，提示即将更新地图
			if (isDeadMap())
			{
				auto winSize = Director::getInstance()->getWinSize();
				auto label = Label::createWithTTF("Cant Crush Any More, Change!", "fonts/Marker Felt.ttf", 24);
				label->setTextColor(Color4B::BLACK);
				label->setPosition(winSize.width / 2, winSize.height / 2);
				label->setOpacity(0);
				this->getParent()->addChild(label,3);

				// 提示文字淡入淡出后，更新地图，再开启触摸监听
				auto fadein = FadeIn::create(0.5);
				auto fadeout = FadeOut::create(0.5);

				auto call = CallFunc::create([this, label](){
					do
					{
						updateMap();
					} while (isDeadMap());

					label->removeFromParent();

					_eventDispatcher->resumeEventListenersForTarget(this);
				});

				label->runAction(Sequence::create(fadein, DelayTime::create(2), fadeout, call, nullptr));
			}
			else
			{
				// 如果不是死图，那么就直接开启触摸监听，等待下一轮的交互操作
				_eventDispatcher->resumeEventListenersForTarget(this);
			}
		}
	}
}