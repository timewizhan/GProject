#ifndef _ENTRY_POINT_
#define _ENTRY_POINT_

#include "..\Common\Common.h"
#include "..\HelpTool\HelpServer.h"

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

typedef std::vector<ST_CLIENT_SOCKET> VecConnection;

class CEntryPoint
{
	ST_SERVER_CONTEXT	m_stServerContext;
	VecConnection		m_vecConnection;
	CHelpServer			*m_pHelpServer;

	VOID InitEPNetworking(std::string &refstrAddress, DWORD dwPort);
	VOID DeleteSelectedConnection(DWORD dwSelectedValue);
	DWORD StartEPNetworking();

	VOID ExcuteNewEPThread(ST_THREAD_PARAM &refstThreadParam);
	VOID ExcuteNewRMThread();
public:
	CEntryPoint();
	~CEntryPoint();

	DWORD StartEntryPoint(std::string &refstrAddress, DWORD dwPort);
};

unsigned int WINAPI WorkerEPThread(void *pData);
unsigned int WINAPI WorkerRMThread(void *pData);

#endif