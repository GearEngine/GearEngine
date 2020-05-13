#include "grpch.h"
#include "JsonManager.h"

namespace Gear {

	JsonManager* JsonManager::s_Inst = nullptr;

	JsonManager * JsonManager::Get()
	{
		if (!s_Inst)
		{
			Init();
		}
		return s_Inst;
	}

	void JsonManager::Init()
	{
		if (!s_Inst)
		{
			s_Inst = new JsonManager;
		}
	}

	void JsonManager::Destroy()
	{
		delete s_Inst;
	}

}