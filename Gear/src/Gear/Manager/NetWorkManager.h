#pragma once

#include "../../vendor/KSNetwork/include/HeaderShared.h"

namespace Gear {

	class PacketAble
	{
	public:
		unsigned int m_Type;

		virtual void Write(OutputMemoryStream& stream) = 0;
		virtual void Read(InputMemoryStream& stream) = 0;
	};

	class NetWorkManager
	{
	private:
		NetWorkManager()
		{
			SocketUtil::StaticInit();
		}

	public:
		static void Init();
		static void Destroy();
		static NetWorkManager* Get();

	public:
		void ConnectServer(const std::string& ip);

		template<typename T>
		bool TypeCheck()
		{
			bool isPacketable = std::is_base_of<PacketAble, T>::value;
			GR_CORE_ASSERT(isPacketable, "NetWorkManager::TypeCheck {0} Type is't derived from PacketAble!", typeid(T).name());
			return isPacketable;
		}

		template<typename T> 
		void Send(T& data)
		{
			if (onceSend)
			{
				GR_CORE_TRACE("Aleady Send!");
				return;
			}
			if (onceReceive)
			{
				onceReceive = false;
			}

			onceSend = true;
			TypeCheck<T>();

			OutputMemoryStream out;
			data.Write(out);

			char* Buffer = static_cast<char*>(malloc(1470));
			memcpy(Buffer, out.GetBufferPtr(), out.GetLength());

			m_ClientSock->Send(Buffer, out.GetLength());
			GR_CORE_TRACE("Send! {0}", typeid(T).name());
		}

		template<typename T>
		T Receive()
		{
			char* Buffer = static_cast<char*>(malloc(1470));
			int size = m_ClientSock->Receive(Buffer, 1470);

			InputMemoryStream in(Buffer, size);
			T data;
			data.Read(in);

			return data;
		}

		std::pair<char*, int> ReceiveInputStream()
		{
			if (onceReceive)
			{
				GR_CORE_TRACE("Already Receive");
				return {nullptr, 0};
			}
			if (onceSend)
			{
				onceSend = false;
			}

			onceReceive = true;
			char* Buffer = static_cast<char*>(malloc(1470));
			int size = m_ClientSock->Receive(Buffer, 1470);
			GR_CORE_TRACE("Receive!");

			return { Buffer, size };
		}

		void Update()
		{
			onceReceive = false;
			onceSend = false;
		}
		
	private:

		static bool onceSend;
		static bool onceReceive;
		static NetWorkManager* s_Inst;
		TCPSocketPtr m_ClientSock; 
	};


}