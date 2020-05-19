#pragma once
#include "MemoryStream.h"

struct CommandPacket
{
	std::vector<char> command;

	void Write(OutputMemoryStream& out, int size)
	{
		out.Write(size);
		for (int i = 0; i < size; i++)
			out.Write(command[i]);
	}
	void Read(InputMemoryStream& in)
	{
		int size;
		in.Read(size);
		for (int i = 0; i < size; i++) {
			char tmp;
			in.Read(tmp);
			command.push_back(tmp);
		}
	}
};

struct CommandSizePacket
{
	int m_size;

	void Write(OutputMemoryStream& out)
	{
		out.Write(m_size);
	}
	void Read(InputMemoryStream& in)
	{
		in.Read(m_size);
	}
};

struct PlotterUserSettingPacket
{
	std::string PlotSerialPath;
	int BaudRate;
	int ByteSize;
	int StopBits;
	int Parity;
	int flowControl;

	void Write(OutputMemoryStream& out)
	{
		out.Write(PlotSerialPath);
		out.Write(BaudRate);
		out.Write(ByteSize);
		out.Write(StopBits);
		out.Write(Parity);
		out.Write(flowControl);
	}
	void Read(InputMemoryStream& in)
	{
		in.Read(PlotSerialPath);
		in.Read(BaudRate);
		in.Read(ByteSize);
		in.Read(StopBits);
		in.Read(Parity);
		in.Read(flowControl);
	}
};