#include "wmpch.h"
#include "Item.h"

namespace InGame {

	ItemPool* ItemPool::s_Inst = nullptr;

	ItemPool::ItemPool(const InitiateData & initData)
	{
		m_Bazooka.reset(new Bazooka(initData));
		m_Grenade.reset(new Grenade(initData));
		m_Banana.reset(new Banana(initData));
	}

	ItemPool::~ItemPool()
	{
		m_Bazooka.reset();
		m_Grenade.reset();
		m_Banana.reset();
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
		case ItemInfo::Banana:
			return m_Banana;
		}
	}

}

