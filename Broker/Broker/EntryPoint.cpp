#include "EntryPoint.h"
#include "NetworkAPI.h"

#include "json\json.h"

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
				ProcessTask(ClientSock)
				
				DeleteSelectedConnection(i);
				FD_CLR(ClientSock, &substReadset);
			}
		}

		// forever loop until admin shutdown
	}

	return dwRet;
}

DWORD CEntryPoint::ProcessTask(SOCKET ClientSock)
{
	DWORD dwRet;
	std::string strRecv;
	dwRet = RecvDataFromOther(ClientSock, strRecv);
	if (dwRet != E_RET_SUCCESS) {
		ErrorLog("Fail to receive data from other");
		return dwRet;
	}

	ST_PROTO stProto;
	DWORD dwAction;
	dwAction = AnalyzeRecvData(strRecv, stProto);
	if (dwAction != E_RET_SUCCESS) {
		ErrorLog("Fail to analyze json data");
		return E_RET_FAIL;
	}

	dwRet = BranchByTaskAction(dwAction, stProto);
	if (dwRet != E_RET_SUCCESS) {
		ErrorLog("Fail to analyze json data");
		return dwRet;
	}

	closesocket(ClientSock);
	return dwRet;
}

DWORD CEntryPoint::BranchByTaskAction(DWORD dwAction, ST_PROTO &refstProto, SOCKET ClientSock)
{
	DWORD dwRet = E_RET_SUCCESS;
	if (dwAction == E_PROTO_UB_STORE) {
		m_InMemory.StoreMetaData(refstProto.stProtoUB);
	}
	else if (dwAction == E_PROTO_UB_LOAD) {
		m_InMemory.LoadMetaData(refstProto.stProtoUB, refstProto.stProtoBU);

		// load balancing

		std::string strMsg;
		BuildSendMsg(strMsg, refstProto);
		SendDataToOther(ClientSock, strMsg);
	}
	else if (dwAction == E_PROTO_CB_RESOURCE) {
		m_InMemory.StoreCloudResource(refstProto.stProtoCB);
	}
	else {
		ErrorLog("Invalid Action");
		dwRet = E_RET_FAIL;
	}

	return dwRet;
}

DWORD CEntryPoint::BuildSendMsg(std::string &refstrJsonData, ST_PROTO &refstProto)
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

DWORD CEntryPoint::AnalyzeRecvData(std::string &refstrRecv, ST_PROTO refstProto)
{
	Json::Reader jsonReader;
	Json::Value jsonRecvData;

	bool bParseRet;
	bParseRet = jsonReader.parse(refstrRecv, jsonRecvData);
	if (!bParseRet) {
		return E_RET_FAIL;
	}

	DWORD dwAction;
	dwAction = jsonRecvData["action"].asInt();
	if (dwAction == E_PROTO_UB_STORE) {
		//
	}
	else if (dwAction == E_PROTO_UB_LOAD) {
		//
	}
	else if (dwAction == E_PROTO_CB_RESOURCE) {
		//
	}
	else {
		// nothing 
	}

	return dwAction;
}

DWORD CEntryPoint::StartEntryPoint(std::string &refstrAddress, DWORD dwPort)
{
	InitEPNetworking(refstrAddress, dwPort);

	DWORD dwRet;
	dwRet = StartEPNetworking();
	if (dwRet != E_RET_SUCCESS) {
		return dwRet;
	}
	return dwRet;
}
