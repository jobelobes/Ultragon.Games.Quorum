#include "3ngine.Core.h"


enum PacketCommand : unsigned short
{
	Empty = 0,
	KeepAlive = 1,

	SetIsReady = 2,
	ResetReady = 3,

	CreateGame = 4,
	JoinGame = 5,
	StartGame = 6,
	EndGame = 7,
	DestroyGame = 8,
	
	UpdateSession = 9,
    UpdateGame = 10,

	SendData = 11
};

enum PacketIdentifier : unsigned int
{
	Broadcast	= 0x0000,
	GameState	= 0x1000
};


struct Packet
{
	PacketCommand Command;
	unsigned int Identifier;
	char Data[128];

	Packet() 
	{ 
		this->Command = PacketCommand::Empty;
		this->Identifier = PacketIdentifier::Broadcast;
		this->SetData("\0", 1);
	}

	Packet(PacketCommand command, int identifier, char* data, int size) 
	{ 
		this->Command = command;
		this->Identifier = identifier;
		this->SetData(data, size);
	}

	void SetData(char* value, int count)
	{
		memcpy(this->Data, value, count);
	}

	void SetData(char* value, int offset, int count)
	{
		memcpy(this->Data + offset, value, count);
	}
};