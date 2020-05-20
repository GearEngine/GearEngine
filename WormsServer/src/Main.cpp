#include "wsvpch.h"

int ConnectCount = 0;
std::mutex mtx;
unsigned int netID = 0;
std::vector<TCPSocketPtr> clientSocks;

void AssignStart(TCPSocketPtr clientSocket)
{
	StartPacket sp;
	sp.start = true;
	mtx.lock();
	sp.netID = netID++;
	mtx.unlock();

	char buffer[PACKET_MAX] = { 0, };
	OutputMemoryStream out;
	sp.Write(out);
	memcpy(buffer, out.GetBufferPtr(), out.GetLength());
	clientSocket->Send(buffer, out.GetLength());
}

void initSend(TCPSocketPtr clientSocket)
{
	Command cmd;
	cmd.packetType = PacketType::cmd;
	cmd.KeyType = 0xffffffb;
	cmd.keyCode = 0xffffffb;

	char buffer[PACKET_MAX] = { 0, };
	OutputMemoryStream out;
	cmd.Write(out);
	memcpy(buffer, out.GetBufferPtr(), out.GetLength());
	clientSocket->Send(buffer, out.GetLength());
}

void gameLogic(TCPSocketPtr servsock, TCPSocketPtr clientSocket)
{
	std::cout << ConnectCount << "클라이언트 접속!" << std::endl;
	int netID = ConnectCount;
	while (1)
	{
		if (ConnectCount >= 2)
		{
			break;
		}
		else
		{
			std::cout << netID << " : " << ConnectCount << std::endl;
		}
	}

	std::cout << "assign connect " << netID << std::endl;
	AssignStart(clientSocket);
	initSend(clientSocket);

	TypeChecker tc;
	while (true) 
	{
		char* buffer = static_cast<char*>(malloc(PACKET_MAX));
		int size = clientSocket->Receive(buffer, PACKET_MAX);
		//std::cout << "Recieve" << cmd.packetType << std::endl;

		if (size < 0) break; // End logic

		InputMemoryStream in(buffer, size);

		tc.Read(in);
		
		if (tc.packetType == 2)
		{
			Event e;
			e.Read(in);
		}

		for (int i = 0; i < 2; ++i)
		{
			clientSocks[i]->Send(buffer, size);
		}
	}
	mtx.lock();
	--ConnectCount;
	mtx.unlock();
	std::cout << "클라이언트 접속해제!" << std::endl;
}

int main()
{
	TCPSocketPtr ServSock;
	SocketUtil::StaticInit();
	SocketAddress servAddr(INADDR_ANY, 9190);
	ServSock = SocketUtil::CreateTCPSocket(INET);
	ServSock->Bind(servAddr);

	ServSock->Listen(5);
	while (true)
	{
		TCPSocketPtr ClientSocket;
		SocketAddress clientAddr;
		ClientSocket = ServSock->Accept(clientAddr);
		mtx.lock();
		++ConnectCount;
		mtx.unlock();
		clientSocks.push_back(ClientSocket);

		std::thread clientThread(gameLogic, ServSock, ClientSocket);
		clientThread.detach();
	}
	SocketUtil::CleanUp();
}