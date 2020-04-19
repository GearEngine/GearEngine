#include "wmpch.h"
#include "Item.h"

namespace InGame {

	ItemPool* ItemPool::s_Inst = nullptr;

	ItemPool::ItemPool(const InitiateData & initData)
	{
		m_Bazooka.reset(new Bazooka(initData));
		m_Grenade.reset(new Grenade(initData));
	}

	ItemPool::~ItemPool()
	{
		m_Bazooka.reset();
		m_Grenade.reset();
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
		case ItemInfo::Bazooka:
			return m_Bazooka;
		case ItemInfo::Grenade:
			return m_Grenade;
		}
	}

}

