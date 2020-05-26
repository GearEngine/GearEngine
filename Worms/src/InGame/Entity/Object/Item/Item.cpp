#include "wmpch.h"
#include "Item.h"

#include "InGame/Layer/ObjectLayer.h"

namespace InGame {

	ItemPool* ItemPool::s_Inst = nullptr;

	ItemPool::ItemPool(const InitiateData & initData)
	{
		m_Bazooka.reset(new Bazooka(initData));
		m_Grenade.reset(new Grenade(initData));
		m_Banana.reset(new Banana(initData));
		m_Donkey.reset(new Donkey(initData));
		m_Hos.reset(new Hos(initData));

		m_BabyHos.resize(m_BabyHosMax);
		for (int i = 0; i < m_BabyHosMax; ++i)
		{
			m_BabyHos[i].reset(new BabyHos(initData));
		}
	}

	ItemPool::~ItemPool()
	{
		m_Bazooka.reset();
		m_Grenade.reset();
		m_Banana.reset();
		m_Donkey.reset();
		m_Hos.reset();
	}

	void ItemPool::init(const InitiateData& initData)
	{
		if (s_Inst)
		{
			delete s_Inst;
		}
		s_Inst = new ItemPool(initData);
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
		case ItemInfo::Donkey:
			return m_Donkey;
		case ItemInfo::Hos:
			return m_Hos;
		case ItemInfo::BabyHos:
			return GetBabyHos();
		}
		return nullptr;
	}

	Gear::Ref<BabyHos> ItemPool::GetBabyHos()
	{
		while (1)
		{
			if (!Gear::EntitySystem::IsEntityActivated(m_BabyHos[m_BabyHosPtr]->GetID()))
			{
				return m_BabyHos[m_BabyHosPtr];
			}
			++m_BabyHosPtr;
			if (m_BabyHosPtr >= m_BabyHosMax)
			{
				m_BabyHosPtr = 0;
			}
		}
		return nullptr;
	}

	void ItemPool::MakeExplosion(const ExplosionData& data, const glm::vec2 pos, Explosion::Text text, const std::string& sound)
	{
		Gear::EventSystem::DispatchEvent(EventChannel::Explosion, Gear::EntityEvent(EventType::Explosion, data));
		auto explosion = EffectPool::GetExplosion(data.Size, text);
		explosion->init(pos, sound);
		ObjectLayer::s_Explosion.push_back(explosion);

		auto smoke = EffectPool::GetExplosionSmoke(data.Size);
		smoke->init(pos);
		ObjectLayer::s_ExplosionSmoke.push_back(smoke);

		auto flame = EffectPool::GetFlame(data.Size);
		flame->init(pos);
		ObjectLayer::s_Flames.push_back(flame);
	}

}

