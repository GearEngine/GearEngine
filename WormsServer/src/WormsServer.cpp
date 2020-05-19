#include <thread>
#include "../../Gear/vendor/KSNetwork/include/HeaderShared.h"
#define PACKET_MAX 1470

struct Command
{
	unsigned int KeyType;
	int keyCode;

	void Read(InputMemoryStream& stream)
	{
		stream.Read(KeyType);
		stream.Read(keyCode);
	}

	void Write(OutputMemoryStream& stream)
	{
		stream.Write(KeyType);
		stream.Write(keyCode);
	}
};

void gameLogic(TCPSocketPtr servsock, TCPSocketPtr clientSocket)
{
	std::cout << "클라이언트 접속!" << std::endl;
	while (true) 
	{
		Command receiveData;
		char* buffer = static_cast<char*>(malloc(PACKET_MAX));
		int size = clientSocket->Receive(buffer, PACKET_MAX);
		
		if (size < 0) break; // End

		InputMemoryStream in(buffer, size);
		receiveData.Read(in);
		std::cout << receiveData.KeyType << " " << receiveData.keyCode << std::endl;

		OutputMemoryStream out;
		receiveData.Write(out);
		char* Buffer2 = static_cast<char*>(malloc(PACKET_MAX));
		memcpy(Buffer2, out.GetBufferPtr(), out.GetLength());
		clientSocket->Send(Buffer2, out.GetLength());
	}
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
		SocketAddress clientAddr;
		TCPSocketPtr ClientSocket;
		ClientSocket = ServSock->Accept(clientAddr);

		std::thread clientThread(gameLogic, ServSock, ClientSocket);
		clientThread.detach();
	}


}