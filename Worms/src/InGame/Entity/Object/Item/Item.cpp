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

	Gear::Ref<ItemBullet> ItemPool::GetItem(Item::Name name)
	{
		switch (name)
		{
		case InGame::Item::Bazooka:
			return m_bazooka;
			break;
		case InGame::Item::ShotGun:
			break;
		case InGame::Item::Grenade:
			break;
		case InGame::Item::ItemMax:
			break;
		}
	}

}

