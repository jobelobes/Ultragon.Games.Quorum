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


struct Packet
{
	PacketCommand Command;
	char Identifier[32];
	char Data[128];

	Packet() 
	{ 
		this->Command = PacketCommand::KeepAlive;
		this->SetIdentifier("\0");
		this->SetData("\0", 1);
	}

	Packet(PacketCommand command, char* identifier, char* data, int size) 
	{ 
		this->Command = command;
		this->SetIdentifier(identifier);
		this->SetData(data, size);
	}


	void SetIdentifier(char* value)
	{
		memcpy(this->Identifier, value, strlen(value) + 1);
	}

	void SetData(char* value, int count)
	{
		memcpy(this->Data, value, count);
	}
};