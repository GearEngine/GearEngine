#include "wsvpch.h"

int ConnectCount = 0;
std::mutex mtx;
unsigned int netID = 0;
std::vector<TCPSocketPtr> clientSocks;
unsigned int turnCompleteCount = 0;
bool receivedTurnChange = false;

unsigned int sendCount = 0;

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

	std::cout << "Send null" << std::endl;
}

void gameLogic(TCPSocketPtr servsock, TCPSocketPtr clientSocket)
{
	std::cout << ConnectCount << "클라이언트 접속!" << std::endl;
	int netID = ConnectCount;

	bool localAreadySend = false;
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
		char* buffer2 = static_cast<char*>(malloc(PACKET_MAX));
		int size = clientSocket->Receive(buffer, PACKET_MAX);
		memcpy(buffer2, buffer, size);

		if (size < 0) break; // End logic

		InputMemoryStream typeCheck(buffer, size);
		InputMemoryStream in(buffer2, size);
		tc.Read(typeCheck);
		
		if (turnCompleteCount == 2)
		{
			mtx.lock();
			receivedTurnChange = false;

			Event e2;
			e2.packetType = PacketType::event;
			e2.eventType = Event::EventType::TurnChange;

			char* buffer3 = static_cast<char*>(malloc(PACKET_MAX));
			OutputMemoryStream out;
			e2.Write(out);

			memcpy(buffer3, out.GetBufferPtr(), out.GetLength());
			for (int i = 0; i < 2; ++i)
			{
				clientSocks[i]->Send(buffer3, size);
				std::cout << "Send new TurnChange event!" << std::endl;
			}
			
			turnCompleteCount = 0;
			mtx.unlock();
			continue;
		}

		if (tc.packetType == PacketType::event)
		{
			Event e;
			e.Read(in);
			if (e.eventType == Event::EventType::TurnChange)
			{
				mtx.lock();
				++turnCompleteCount;
				receivedTurnChange = true;
				mtx.unlock();
				std::cout << "Receive TurnChange! on : " << netID << " " << turnCompleteCount << std::endl;
			}
			else
			{
				for (int i = 0; i < 2; ++i)
				{
					clientSocks[i]->Send(buffer, size);
				}
			}
		}

		if (receivedTurnChange)
		{
			std::cout << "Now On turnChanging" << std::endl;
			for (int i = 0; i < 2; ++i)
			{
				initSend(clientSocks[i]);
			}
		}
		else
		{
			for (int i = 0; i < 2; ++i)
			{
				clientSocks[i]->Send(buffer, size);
			}
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