#include "NewEPThread.h"
#include "NetworkAPI.h"

#include "json\json.h"

CEPThread::CEPThread()
{
}

CEPThread::~CEPThread()
{
}

DWORD CEPThread::AnalyzeRecvData(std::string &refstrRecv, ST_PROTO_UC &refstProtoUC)
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
	if (dwAction == E_PROTO_UC_STORE) {
		refstProtoUC.strUID = jsonRecvData["uid"].asString();
		refstProtoUC.strFileName = jsonRecvData["filename"].asString();
		refstProtoUC.dwIndex = static_cast<DWORD>(jsonRecvData["index"].asInt());
		refstProtoUC.dwSize = static_cast<DWORD>(jsonRecvData["size"].asInt());
	}
	else if (dwAction == E_PROTO_UC_LOAD) {
		refstProtoUC.strUID = jsonRecvData["uid"].asString();
		refstProtoUC.strFileName = jsonRecvData["filename"].asString();
		refstProtoUC.dwIndex = static_cast<DWORD>(jsonRecvData["index"].asInt());
		refstProtoUC.dwSize = static_cast<DWORD>(jsonRecvData["size"].asInt());
	}
	else {
		// nothing 
	}

	return E_RET_SUCCESS;
}

DWORD CEPThread::BranchByAction(ST_PROTO_UC &refstProtoUC)
{
	DWORD dwRet;
	if (refstProtoUC.dwAction == E_PROTO_UC_STORE) {
		char *pRecvBuf = new char[refstProtoUC.dwSize];
		dwRet = RecvLargeDataFromOther(m_stThreadParam.hServerSock, &pRecvBuf, refstProtoUC.dwSize);
		if (dwRet != E_RET_SUCCESS) {
			delete pRecvBuf;
			return dwRet;
		}

		m_SM.ExecuteOpStore(refstProtoUC, pRecvBuf);
		delete pRecvBuf;
	}
	else if (refstProtoUC.dwAction == E_PROTO_UC_LOAD) {
		char *pRecvBuf = new char[refstProtoUC.dwSize];
		dwRet = m_SM.ExecuteOpLoad(refstProtoUC, &pRecvBuf);
		if (dwRet != E_RET_SUCCESS) {
			delete pRecvBuf;
			return dwRet;
		}

		SendLargeDataToOther(m_stThreadParam.hServerSock, &pRecvBuf, refstProtoUC.dwSize);
		delete pRecvBuf;
	}
	else {
		return E_RET_FAIL;
	}

	return dwRet;
}

DWORD CEPThread::CommunicateWithUser()
{
	std::string strRecv;

	DWORD dwRet;
	dwRet = RecvDataFromOther(m_stThreadParam.hServerSock, strRecv);
	if (dwRet != E_RET_SUCCESS) {
		return E_RET_COMM_RECV;
	}

	ST_PROTO_UC stProtoUC;
	dwRet = AnalyzeRecvData(strRecv, stProtoUC);
	if (dwRet != E_RET_SUCCESS) {
		return E_RET_COMM_ANAL;
	}

	dwRet = BranchByAction(stProtoUC);
	if (dwRet != E_RET_SUCCESS) {
		return E_RET_COMM_BRANCH;
	}
}

DWORD CEPThread::StartEPThread(ST_THREAD_PARAM &refstThreadParam)
{
	DWORD dwRet = E_RET_SUCCESS;
	m_stThreadParam = refstThreadParam;
	try
	{
		dwRet = CommunicateWithUser();
		if (dwRet != E_RET_SUCCESS) {
			throw std::exception("");
		}
	}
	catch (std::exception &e)
	{
		ErrorLog("EP Thread Error : %s", e.what());
		dwRet = E_RET_FAIL;
	}

	return dwRet;
}
