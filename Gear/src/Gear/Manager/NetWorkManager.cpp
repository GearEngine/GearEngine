#include "grpch.h"
#include "NetWorkManager.h"

namespace Gear {

	NetWorkManager* NetWorkManager::s_Inst = nullptr;

	void NetWorkManager::Init()
	{
		s_Inst = new NetWorkManager;
	}

	void NetWorkManager::Destroy()
	{
		if (s_Inst)
		{
			delete s_Inst;
		}
	}

	NetWorkManager * NetWorkManager::Get()
	{
		if (!s_Inst)
		{
			Init();
		}
		return s_Inst;
	}
}

