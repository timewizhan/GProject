#ifndef _RMTHREAD_
#define _RMTHREAD_

#include "..\Common\Common.h"
#include "..\HelpTool\HelpClient.h"
#include "ResourceManager.h"

struct ST_SERVER_ADDR
{
	std::string strIPAddress;
	DWORD dwPort;
};

class CRMThread
{
	ST_CLIENT_CONTEXT	m_stClientContext;
	ST_SERVER_ADDR		m_stServerAddr;
	CHelpClient			*m_pHelpClient;

	VOID InitServerConnection();
	BOOL ConnectToServer();
	DWORD BuildSendMsg(std::string &refstrJsonData, ST_RM_DATA &refstRMData);

public:
	CRMThread();
	~CRMThread();

	DWORD StartRMThread();
};

#endif