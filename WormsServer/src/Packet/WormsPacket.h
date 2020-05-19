#pragma once

enum PacketType : unsigned int
{
	Cmd,
	Start,
	TurnChange,
	ReWind
};

struct StartPacket
{
	StartPacket()
	{
		packetType = PacketType::Start;
	}
	unsigned int packetType;
	bool start;
	void Read(InputMemoryStream& stream)
	{
		stream.Read(packetType);
		stream.Read(start);
	}
	void Write(OutputMemoryStream& stream)
	{
		stream.Write(packetType);
		stream.Write(start);
	}
};

struct Command
{
	Command()
	{
		packetType = PacketType::Cmd;
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