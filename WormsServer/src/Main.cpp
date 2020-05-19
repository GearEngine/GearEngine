#include "wsvpch.h"

int ConnectCount;
std::mutex mtx;

void AssignStart(TCPSocketPtr clientSocket)
{
	StartPacket sp;
	sp.start = true;

	char buffer[PACKET_MAX] = { 0, };
	OutputMemoryStream out;
	sp.Write(out);
	memcpy(buffer, out.GetBufferPtr(), out.GetLength());
	clientSocket->Send(buffer, out.GetLength());
}

void gameLogic(TCPSocketPtr servsock, TCPSocketPtr clientSocket)
{
	std::cout << "클라이언트 접속!" << std::endl;
	char buffer[PACKET_MAX] = { 0, };

	while (ConnectCount < 2)
	{
	}
	AssignStart(clientSocket);

	while (true) 
	{
		Command receiveData;
		char* buffer = static_cast<char*>(malloc(PACKET_MAX));
		int size = clientSocket->Receive(buffer, PACKET_MAX);
		
		if (size < 0) break; // End logic

		InputMemoryStream in(buffer, size);
		receiveData.Read(in);
		std::cout << receiveData.KeyType << " " << receiveData.keyCode << std::endl;

		memset(buffer, 0, PACKET_MAX);
		OutputMemoryStream out;
		memcpy(buffer, out.GetBufferPtr(), out.GetLength());
		clientSocket->Send(buffer, out.GetLength());
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
		++ConnectCount;
		
		std::thread clientThread(gameLogic, ServSock, ClientSocket);
		clientThread.detach();
	}
	SocketUtil::CleanUp();
}