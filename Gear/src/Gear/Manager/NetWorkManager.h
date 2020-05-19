#pragma once

namespace Gear {

	class OutputMemoryStream {};
	class InputMemoryStream {};
	class TCPSocketPtr {};

	class PacketAble
	{
	public:
		virtual void Write(OutputMemoryStream& stream) = 0;
		virtual void Read(const InputMemoryStream& stream) = 0;
	};

	class NetWorkManager
	{
	private:
		NetWorkManager() = default;
		NetWorkManager(const std::string& serverIp)
			: m_ServerIp(serverIp)
		{
		}

	public:
		static void Init();
		static void Destroy();
		static NetWorkManager* Get();

	public:

		template<typename T> 
		void Send(const T& data)
		{
			OutputMemoryStream out;
			//data.Write(out);

			/*char* Buffer = static_cast<char*>(malloc(1470));
			memcpy(Buffer, out.GetBufferPtr(), out.GetLength());

			clientSock->Send(Buffer, out.GetLength());*/
		}

		template<typename T>
		T Reseive()
		{
			T data;

			/*char* Buffer = static_cast<char*>(malloc(1470));
			int size = clientSock->Receive(Buffer, 1470);
			InputMemoryStream in(Buffer, size);
			data.Read(in);*/

			return data;
		}

	private:
		static NetWorkManager* s_Inst;
		std::string m_ServerIp;
		TCPSocketPtr m_ClientSock; 
	};


}