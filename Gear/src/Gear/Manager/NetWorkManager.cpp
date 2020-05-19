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

	void NetWorkManager::ConnectServer(const std::string& ip)
	{
		SocketAddress addr = *SocketAddressFactory::CreateIPv4FromString(ip);
		m_ClientSock = SocketUtil::CreateTCPSocket(INET);
		m_ClientSock->Connect(addr);
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

