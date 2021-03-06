#include "grpch.h"
#include "NetWorkManager.h"

#include "Gear/Component/Controller.h"

namespace Gear {

	NetWorkManager* NetWorkManager::s_Inst = nullptr;
	bool NetWorkManager::onceReceive = false;
	bool NetWorkManager::onceSend = false;

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

	void NetWorkManager::Update()
	{
		if (!m_ClientSock)
		{
			return;
		}

		onceReceive = false;
		onceSend = false;
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

