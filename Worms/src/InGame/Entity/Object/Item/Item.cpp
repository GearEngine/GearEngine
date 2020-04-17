#include "wmpch.h"
#include "Item.h"

namespace InGame {

	ItemPool* ItemPool::s_Inst = nullptr;

	ItemPool::ItemPool(const InitiateData & initData)
	{
		m_bazooka.reset(new Bazooka(initData));
	}

	ItemPool::~ItemPool()
	{
	}

	void ItemPool::init(const InitiateData& initData)
	{
		if (!s_Inst)
		{
			s_Inst = new ItemPool(initData);
		}
	}

	Gear::Ref<ItemBullet> ItemPool::GetItem(ItemInfo::Number number)
	{
		switch (number)
		{
		case InGame::ItemInfo::Bazooka:
			return m_bazooka;
		}
	}

}

