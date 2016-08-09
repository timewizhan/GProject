#include "RMThread.h"
#include "json\json.h"
#include "NetworkAPI.h"

CRMThread::CRMThread()
{
	m_pHelpClient = new CHelpClient();

	// static data for the test
	m_stServerAddr.strIPAddress = "192.168.0.1";
	m_stServerAddr.dwPort = 1234;
}

CRMThread::~CRMThread()
{
	if (m_pHelpClient)
		delete m_pHelpClient;
}

VOID CRMThread::InitServerConnection()
{
	m_pHelpClient->InitClientSock(m_stClientContext, m_stServerAddr);
}

BOOL CRMThread::ConnectToServer()
{
#define SECOND 1000
	BOOL bContinue = TRUE;

	DWORD dwRet;
	DWORD dwTimeCount = 1;
	while (bContinue) {
		dwRet = m_pHelpClient->ConnectToServer(m_stClientContext);
		if (dwRet != E_RET_SUCCESS) {
			::Sleep(SECOND * dwTimeCount);

			if (dwTimeCount > 64)
				dwTimeCount = 1;

			dwTimeCount *= 2;
			continue;
		}
		bContinue = FALSE;
	}

	DebugLog("Success to create connection with MM Health Server");
	return TRUE;
}

DWORD CRMThread::BuildSendMsg(std::string &refstrJsonData, ST_RM_DATA &refstRMData)
{
	Json::Value jsonRoot;

	jsonRoot["action"] = 1;
	jsonRoot["number"] = 1;

	Json::StyledWriter jsonWriter;

	std::string strJsonData;
	strJsonData = jsonWriter.write(jsonRoot);
	refstrJsonData = strJsonData;

	return E_RET_SUCCESS;
}


DWORD CRMThread::StartRMThread()
{
	InitServerConnection();
	ConnectToServer();

	CResourceManager RM;
	bool bContinue = true;
	DWORD dwRet;
	while (bContinue) {
		ST_RM_DATA stRMData;
		RM.GetResourceData(stRMData);

		std::string strJsonData;
		BuildSendMsg(strJsonData, stRMData);
		dwRet = SendDataToOther(m_stClientContext.stServerInfo.hServerSock, strJsonData);
		if (dwRet != E_RET_SUCCESS) {
			ErrorLog("Wrong execution of send function");
		}
		DebugLog("Success to send resource data to broker");

		::Sleep(1000);
		// forever until admin shutdown
	}
}


