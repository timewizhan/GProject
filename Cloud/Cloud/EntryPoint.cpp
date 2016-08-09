#include "EntryPoint.h"
#include "NewEPThread.h"
#include "RMThread.h"

CEntryPoint::CEntryPoint()
{
	m_pHelpServer = new CHelpServer();
}

CEntryPoint::~CEntryPoint()
{
	if (m_pHelpServer)
		delete m_pHelpServer;
}

VOID CEntryPoint::InitEPNetworking(std::string &refstrAddress, DWORD dwPort)
{
	DWORD dwRet;
	dwRet = m_pHelpServer->InitServerSock(m_stServerContext);
	if (dwRet != E_RET_SUCCESS) {
		throw std::exception("Fail to init bot server");
	}

	dwRet = m_pHelpServer->InitServerBind(m_stServerContext, refstrAddress, dwPort);
	if (dwRet != E_RET_SUCCESS) {
		throw std::exception("Fail to init server bind");
	}
}

VOID CEntryPoint::DeleteSelectedConnection(DWORD dwSelectedValue)
{
	VecConnection::iterator iterVec;
	DWORD dwCurrent = 0;
	for (iterVec = m_vecConnection.begin(); iterVec != m_vecConnection.end(); iterVec++, dwCurrent++) {
		if (dwCurrent != dwSelectedValue) {
			continue;
		}
		m_vecConnection.erase(iterVec);
		DebugLog("Delete Manager Connection [%d]", m_vecConnection.size());
	}
}

DWORD CEntryPoint::StartEPNetworking()
{
	FD_SET stReadset, substReadset;

	FD_ZERO(&stReadset);
	FD_SET(m_stServerContext.stServerInfo.hServerSock, &stReadset);

	DWORD dwRet = E_RET_SUCCESS;
	DWORD dwConnection = 0;
	BOOL bContinue = TRUE;
	while (bContinue) {
		substReadset = stReadset;
		::select(0, &substReadset, NULL, NULL, NULL);

		if (FD_ISSET(m_stServerContext.stServerInfo.hServerSock, &substReadset)) {
			ST_CLIENT_SOCKET stClientSocket;
			dwRet = m_pHelpServer->AcceptServer(m_stServerContext, stClientSocket);
			if (dwRet != E_RET_SUCCESS) {
				ErrorLog("Fail to accept manager connection");
				continue;
			}
			dwConnection++;

			DebugLog("User Connection is created [%d]", dwConnection);

			m_vecConnection.push_back(stClientSocket);
			FD_SET(stClientSocket.hClientSock, &stReadset);
			continue;
		}

		for (unsigned i = 0; i < m_vecConnection.size(); i++) {
			SOCKET ClientSock = m_vecConnection[i].hClientSock;

			if (FD_ISSET(ClientSock, &substReadset)) {
				ST_THREAD_PARAM stThreadParam;
				stThreadParam.hServerSock = ClientSock;
				
				ExcuteNewEPThread(stThreadParam);
				DeleteSelectedConnection(i);
				FD_CLR(ClientSock, &substReadset);
			}
		}

		// forever loop until admin shutdown
	}

	return dwRet;
}

VOID CEntryPoint::ExcuteNewEPThread(ST_THREAD_PARAM &refstThreadParam)
{
	HANDLE hThread = NULL;
	hThread = (HANDLE)_beginthreadex(NULL, 0, WorkerEPThread, (void *)&refstThreadParam, 0, NULL);
	if (!hThread) {
		DebugLog("New EP Thread is not created");
		return;
	}

	DebugLog("New EP Thread is created");
}

VOID CEntryPoint::ExcuteNewRMThread()
{
	HANDLE hThread = NULL;
	hThread = (HANDLE)_beginthreadex(NULL, 0, WorkerRMThread, NULL, 0, NULL);
	if (!hThread) {
		DebugLog("New RM Thread is not created");
		return;
	}

	DebugLog("New RM Thread is created");
}

DWORD CEntryPoint::StartEntryPoint(std::string &refstrAddress, DWORD dwPort)
{
	ExcuteNewRMThread();
	InitEPNetworking(refstrAddress, dwPort);

	DWORD dwRet;
	dwRet = StartEPNetworking();
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}


unsigned int WINAPI WorkerEPThread(void *pData)
{
	ST_THREAD_PARAM stThreadParam = *((ST_THREAD_PARAM *)pData);
	CEPThread EPThread;

	DWORD dwRet;
	dwRet = EPThread.StartEPThread(stThreadParam);
	if (dwRet != E_RET_SUCCESS) {
		ErrorLog("EP Thread is abnormally finished");
		return dwRet;
	}

	DebugLog("EP Thread is successfully finished");
	return dwRet;
}

unsigned int WINAPI WorkerRMThread(void *pData)
{
	CRMThread RMThread;

	DWORD dwRet;
	dwRet = RMThread.StartRMThread();
	if (dwRet != E_RET_SUCCESS) {
		ErrorLog("RM Thread is abnormally finished");
		return dwRet;
	}

	DebugLog("RM Thread is successfully finished");
	return dwRet;
}