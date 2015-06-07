#include "3ngine.Core.h"

#pragma region Static Variables
int CMulticastClient::g_iReferences = 0;
#pragma endregion

#pragma region Properties
bool CMulticastClient::Get_IsConnected()
{
	return this->m_bConnected;
}

SOCKET CMulticastClient::Get_Socket()
{
	return this->m_sSocket;
}

int CMulticastClient::Get_ReceiveBufferSize()
{
	return this->m_iReceiveBufferSize;
}

void CMulticastClient::Set_ReceiveBufferSize(int value)
{
	this->m_iReceiveBufferSize = value;
	this->m_iReceiveBufferOffset = 0;

	if(this->m_pReceiveBuffer != NULL)
		delete [] this->m_pReceiveBuffer;

	this->m_pReceiveBuffer = new char[this->m_iReceiveBufferSize];
}

int CMulticastClient::Get_SendBufferSize()
{
	return this->m_iSendBufferSize;
}

void CMulticastClient::Set_SendBufferSize(int value)
{
	this->m_iSendBufferSize = value;
	this->m_iSendBufferOffset = 0;

	if(this->m_pSendBuffer != NULL)
		delete [] this->m_pSendBuffer;
	
	this->m_pSendBuffer = new char[this->m_iSendBufferSize];
}

bool CMulticastClient::Get_AutoFlush()
{
	return this->m_bAutoFlush;
}

void CMulticastClient::Set_AutoFlush(bool value)
{
	this->m_bAutoFlush = value;
}
#pragma endregion

#pragma region Constructors and Finalizers
CMulticastClient::CMulticastClient()
{
	CMulticastClient::g_iReferences++;
	if(CMulticastClient::g_iReferences == 1)
		CMulticastClient::InitializeSockets();

	this->m_sSocket = INVALID_SOCKET;

	this->m_pReceiveBuffer = NULL;
	this->m_pSendBuffer = NULL;

	this->Set_ReceiveBufferSize(1024);
	this->Set_SendBufferSize(1024);
}

CMulticastClient::~CMulticastClient()
{
	this->Disconnect();

	CMulticastClient::g_iReferences--;
	if(CMulticastClient::g_iReferences == 0)
		::WSACleanup();
}
#pragma endregion

#pragma region Methods
bool CMulticastClient::CreateSocket(ADDRINFO* info)
{
	// Create a SOCKET for connecting to server
	this->m_sSocket = socket(info->ai_family, info->ai_socktype, 0);
	if (this->m_sSocket == INVALID_SOCKET)
		return false;

	// Set TTL of multicast packet
	DWORD ttl = 7;
	if ( setsockopt(this->m_sSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(ttl)) != 0 )
		return false;

	return true;
}

bool CMulticastClient::JoinMulticast(ADDRINFO* multicast)
{
	struct ip_mreq mRequest;

    /* Specify the multicast group */
    memcpy(&mRequest.imr_multiaddr, &((struct sockaddr_in*)(multicast->ai_addr))->sin_addr, sizeof(mRequest.imr_multiaddr));

    /* Accept multicast from any interface */
    mRequest.imr_interface.s_addr = htonl(INADDR_ANY);

	//char loop = 0x0;
	//if(setsockopt(this->m_sSocket, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)))
	//	return false;

    /* Join the multicast address */
    if (setsockopt(this->m_sSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mRequest, sizeof(mRequest)) != 0 )
		return false;
}

bool CMulticastClient::DropMulticast(ADDRINFO* multicast)
{
	struct ip_mreq mRequest;

    /* Specify the multicast group */
    memcpy(&mRequest.imr_multiaddr, &((struct sockaddr_in*)(multicast->ai_addr))->sin_addr, sizeof(mRequest.imr_multiaddr));

    /* Accept multicast from any interface */
    mRequest.imr_interface.s_addr = htonl(INADDR_ANY);

	if(setsockopt(this->m_sSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP,(char*)&mRequest, sizeof(mRequest)))
		return false;
	return true;
}

bool CMulticastClient::Connect(char* host, char* port)
{
	// Resolve the multicast group address
	ADDRINFO hints = { 0 };
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_NUMERICHOST;
	if (getaddrinfo(host, port, &hints, &this->m_sMulticastAddr) != 0 )
	{
		int err = WSAGetLastError();
        printf("Network Error: [%i]Error resolving addresses.\n",err);
		return false;
	}

	// Get a local address with the same family as our multicast group
    hints.ai_family   = this->m_sMulticastAddr->ai_family;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags    = AI_PASSIVE;
    if (getaddrinfo(NULL, port, &hints, &this->m_sLocalAddr) != 0 )
    {
		int err = WSAGetLastError();
        printf("Network Error: [%i]Error resolving addresses.\n",err);
		return false;
    }

	this->CreateSocket(this->m_sMulticastAddr);

	if(this->m_sSocket == INVALID_SOCKET)
	{
		printf("Network Error: No socket available for use.\n");
		return false;
	}

	// bind to the multicast port
    if (bind(this->m_sSocket, this->m_sLocalAddr->ai_addr, this->m_sLocalAddr->ai_addrlen) != 0 )
    {
        printf("Network Error: [%i]Failed to bind to local device.\n",WSAGetLastError());
		return false;
    }

	return this->JoinMulticast(this->m_sMulticastAddr);
}

void CMulticastClient::Disconnect()
{
	this->DropMulticast(this->m_sMulticastAddr);
	closesocket(this->m_sSocket);
}

int CMulticastClient::Send(char* buffer, int offset, int size)
{
	if(this->m_iSendBufferOffset + size > this->m_iSendBufferSize)
	{
		if(!this->m_bAutoFlush)
			return 0;
		this->Flush();
	}

	memcpy(this->m_pSendBuffer + this->m_iSendBufferOffset, buffer + offset, size);
	this->m_iSendBufferOffset += size;
	return size;
}

int CMulticastClient::Receive(char* buffer, int offset, int size)
{
	char* recvBuffer = this->m_pReceiveBuffer + this->m_iReceiveBufferOffset;
	int recvSize = this->m_iReceiveBufferSize - this->m_iReceiveBufferOffset;

	// keep trying to fill up the buffer
	int result = recvfrom(this->m_sSocket, recvBuffer, recvSize, MSG_PEEK, NULL, 0);
	if(result > 0)
	{
		result = recvfrom(this->m_sSocket, recvBuffer, recvSize, 0, NULL, 0);
		this->m_iReceiveBufferOffset += result;

		Packet* packet = (Packet*)(this->m_pReceiveBuffer + this->m_iReceiveBufferOffset - size);
		if(packet->Command > 32 || strlen(packet->Identifier) > 32)
			printf("Error");
	}
	
	// move buffered data out of the receive buffer
	if(this->m_iReceiveBufferOffset < size)
		return 0;

	memcpy(buffer + offset, this->m_pReceiveBuffer + this->m_iReceiveBufferOffset - size, size);
	
	Packet* packet = (Packet*)(this->m_pReceiveBuffer + this->m_iReceiveBufferOffset - size);
	if(packet->Command > 32 || strlen(packet->Identifier) > 32)
		printf("Error");

	packet = (Packet*)(buffer + offset);
	if(packet->Command > 32 || strlen(packet->Identifier) > 32)
		printf("Error");

	this->m_iReceiveBufferOffset -= size;
	return size;
}

void CMulticastClient::Flush()
{
	if(this->m_iSendBufferOffset == 0)
		return;

	int result = sendto(this->m_sSocket, this->m_pSendBuffer, this->m_iSendBufferOffset, 0, this->m_sMulticastAddr->ai_addr, this->m_sMulticastAddr->ai_addrlen);
	if (result == SOCKET_ERROR)
		printf("Network Error: [%i]Unable to flush Send buffer.\n", WSAGetLastError());
	else
		this->m_iSendBufferOffset = 0;
}
#pragma endregion

#pragma region Static Methods
bool CMulticastClient::InitializeSockets()
{	
	WSADATA wsaData;
	if ( WSAStartup(MAKEWORD(2, 0), &wsaData ) != 0 )
		return false;

	if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0 )
	{
		WSACleanup();
		return FALSE;
	}
}
#pragma endregion
