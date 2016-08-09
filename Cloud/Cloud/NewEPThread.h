#ifndef _NEW_EP_THEAD_
#define _NEW_EP_THEAD_

#include "..\Common\Common.h"
#include "EntryPoint.h"
#include "Protocol.h"
#include "StorageManager.h"

enum E_RET_COMM
{
	E_RET_COMM_RECV = 1,
	E_RET_COMM_ANAL,
	E_RET_COMM_BRANCH,

	E_RET_COMM_UNKNOWN
};

class CEPThread
{
	CStorageManager	m_SM;
	ST_THREAD_PARAM m_stThreadParam;

	DWORD AnalyzeRecvData(std::string &refstrRecv, ST_PROTO_UC &refstProtoUC);
	DWORD BranchByAction(ST_PROTO_UC &refstProtoUC);

	DWORD CommunicateWithUser();
public:
	CEPThread();
	~CEPThread();

	DWORD StartEPThread(ST_THREAD_PARAM &refstThreadParam);
};

#endif