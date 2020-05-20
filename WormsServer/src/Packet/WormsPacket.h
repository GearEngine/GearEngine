#pragma once

enum PacketType : unsigned int
{
	cmd,
	start,
	event
};

struct StartPacket
{
	StartPacket()
	{
		packetType = PacketType::start;
	}
	unsigned int packetType;
	bool start;
	unsigned int netID;

	void Read(InputMemoryStream& stream)
	{
		stream.Read(packetType);
		stream.Read(start);
		stream.Read(netID);
	}
	void Write(OutputMemoryStream& stream)
	{
		stream.Write(packetType);
		stream.Write(start);
		stream.Write(netID);
	}
};

struct TypeChecker
{
	unsigned int packetType;
	void Read(InputMemoryStream& stream)
	{
		stream.Read(packetType);
	}
};

struct Event
{
	Event()
	{
		packetType = PacketType::event;
	}
	enum EventType : unsigned int
	{
		TurnChange,
		ItemSelect
	};

	unsigned int packetType;
	unsigned int eventType;
	int entityID;
	int customIData1[20];
	float customFData1[20];

	virtual void Read(InputMemoryStream& stream)
	{
		stream.Read(packetType);
		stream.Read(eventType);
		stream.Read(entityID);
		for (int i = 0; i < 20; ++i)
		{
			stream.Read(customIData1[i]);
		}
		for (int i = 0; i < 0; ++i)
		{
			stream.Read(customFData1[i]);
		}
	}
	virtual void Write(OutputMemoryStream& stream)
	{
		stream.Write(packetType);
		stream.Write(eventType);
		stream.Write(entityID);
		for (int i = 0; i < 20; ++i)
		{
			stream.Write(customIData1[i]);
		}
		for (int i = 0; i < 0; ++i)
		{
			stream.Write(customFData1[i]);
		}
	}
};

struct Command
{
	Command()
	{
		packetType = PacketType::cmd;
	}
	unsigned int packetType;
	unsigned int KeyType;
	int keyCode;

	void Read(InputMemoryStream& stream)
	{
		stream.Read(packetType);
		stream.Read(KeyType);
		stream.Read(keyCode);
	}

	void Write(OutputMemoryStream& stream)
	{
		stream.Write(packetType);
		stream.Write(KeyType);
		stream.Write(keyCode);
	}
};