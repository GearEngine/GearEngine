#pragma once
#include "ItemEnum.h"

#include "Bazooka/Bazooka.h"
#include "Grenade/Grenade.h"

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
		static ItemPool* s_Inst;
		
		Gear::Ref<Bazooka> m_Bazooka;
		Gear::Ref<Grenade> m_Grenade;
	};
	
	#define ITEM_POOL ItemPool::Get()

}