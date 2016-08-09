#ifndef _ENTRY_POINT_
#define _ENTRY_POINT_

#include "..\Common\Common.h"
#include "..\HelpTool\HelpServer.h"
#include "Protocol.h"
#include "InMemory.h"

struct ST_SERVER_INFO
{
	SOCKET			hServerSock;
	SOCKADDR_IN		stServerAddrIn;
	ST_SERVER_BIND	stServerBind;
};

struct ST_SERVER_CONTEXT
{
	WSADATA			stWSAData;
	ST_SERVER_INFO	stServerInfo;
};

struct ST_THREAD_PARAM
{
	SOCKET			hServerSock;
};

struct ST_PROTO
{
	ST_PROTO_UB stProtoUB;
	ST_PROTO_BU stProtoBU;
	ST_PROTO_CB stProtoCB;
};

typedef std::vector<ST_CLIENT_SOCKET> VecConnection;

class CEntryPoint
{
	ST_SERVER_CONTEXT	m_stServerContext;
	VecConnection		m_vecConnection;
	CHelpServer			*m_pHelpServer;

	CInMemory			m_InMemory;

	VOID InitEPNetworking(std::string &refstrAddress, DWORD dwPort);
	VOID DeleteSelectedConnection(DWORD dwSelectedValue);
	DWORD StartEPNetworking();

	DWORD ProcessTask(SOCKET ClientSock);
	DWORD AnalyzeRecvData(std::string &refstrRecv, ST_PROTO refstProto);
	DWORD BranchByTaskAction(DWORD dwAction, ST_PROTO &refstProto, SOCKET ClientSock);
	DWORD BuildSendMsg(std::string &refstrJsonData, ST_PROTO &refstProto);
public:
	CEntryPoint();
	~CEntryPoint();

	DWORD StartEntryPoint(std::string &refstrAddress, DWORD dwPort);
};

#endif