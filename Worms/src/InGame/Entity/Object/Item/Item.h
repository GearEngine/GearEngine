#pragma once
#include "ItemEnum.h"

#include "Bazooka/Bazooka.h"
#include "Grenade/Grenade.h"
#include "Banana/Banana.h"
#include "Donkey/Donkey.h"
#include "Hos/Hos.h"
#include "Hos/BabyHos.h"

namespace InGame {

	class ItemPool
	{
	private:
		ItemPool(const InitiateData& initData);
		~ItemPool();

	public:
		static void init(const InitiateData& initData);
		Gear::Ref<ItemBullet> GetItem(ItemInfo::Number number);
		static inline ItemPool* Get() { return s_Inst; }
		inline static void Destory() { 
			if(s_Inst) 
				delete s_Inst; 
			s_Inst = nullptr; 
		}
	private:
		Gear::Ref<BabyHos> GetBabyHos();

	public:
		void MakeExplosion(const ExplosionData& data, const glm::vec2 pos, Explosion::Text text, const std::string& sound = "");

	private:
		static ItemPool* s_Inst;
		
		Gear::Ref<Bazooka> m_Bazooka;
		Gear::Ref<Grenade> m_Grenade;
		Gear::Ref<Banana> m_Banana;
		Gear::Ref<Donkey> m_Donkey;
		Gear::Ref<Hos> m_Hos;

		std::vector<Gear::Ref<BabyHos>> m_BabyHos;
		const int m_BabyHosMax = 50;
		int m_BabyHosPtr = 0;
	};
	
	#define ITEM_POOL ItemPool::Get()

}