#ifndef _NETWORK_API_
#define _NETWORK_API_

#include "..\Common\Common.h"

DWORD RecvDataFromOther(SOCKET ClientSock, std::string &refstrRecvData)
{
	char szBuf[128] = { 0 };

	DWORD dwRecvCount;
	dwRecvCount = ::recv(ClientSock, szBuf, sizeof(szBuf), 0);
	if (dwRecvCount < 1) {
		return E_RET_FAIL;
	}

	refstrRecvData = szBuf;
	return E_RET_SUCCESS;
}

DWORD RecvLargeDataFromOther(SOCKET ClientSock, char **pBuf, DWORD dwSize)
{
	DWORD dwRecvCount;
	dwRecvCount = ::recv(ClientSock, *pBuf, dwSize, 0);
	if (dwRecvCount < 1) {
		return E_RET_FAIL;
	}
	return E_RET_SUCCESS;
}

DWORD SendDataToOther(SOCKET ClientSock, std::string &refstrSendMsg)
{
	int nSizeOfData = refstrSendMsg.size();
	int nSent = 0, nRet;
	BOOL bContinue = TRUE;
	while (bContinue) {
		nRet = send(ClientSock, refstrSendMsg.c_str() + nSent, nSizeOfData - nSent, 0);
		if (nSent == SOCKET_ERROR) {
			bContinue = FALSE;
			continue;
		}
		else if (nRet == nSizeOfData) {
			DebugLog("Success to send data to MM Server");
			bContinue = FALSE;
			continue;
		}

		/*
		if all data is not sent to client yet, continue to send to rest of data
		*/
		nSent = nRet;
	}
	return E_RET_SUCCESS;
}

DWORD SendLargeDataToOther(SOCKET ClientSock, char **ppBuf, DWORD dwSize)
{
	int nSizeOfData = dwSize;
	int nSent = 0, nRet;
	BOOL bContinue = TRUE;
	while (bContinue) {
		nRet = send(ClientSock, *ppBuf + nSent, nSizeOfData - nSent, 0);
		if (nSent == SOCKET_ERROR) {
			bContinue = FALSE;
			continue;
		}
		else if (nRet == nSizeOfData) {
			DebugLog("Success to send data to MM Server");
			bContinue = FALSE;
			continue;
		}

		/*
		if all data is not sent to client yet, continue to send to rest of data
		*/
		nSent = nRet;
	}
	return E_RET_SUCCESS;
}

#endif