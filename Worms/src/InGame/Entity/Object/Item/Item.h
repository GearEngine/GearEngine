#pragma once
#include "ItemEnum.h"

#include "Bazooka/Bazooka.h"

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

	private:
		static ItemPool* s_Inst;
		
		Gear::Ref<Bazooka> m_bazooka;
	};
	
	#define ITEM_POOL ItemPool::Get()

}