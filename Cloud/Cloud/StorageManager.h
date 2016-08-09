#ifndef _STORAGE_MANAGER_
#define _STORAGE_MANAGER_

#include "..\Common\Common.h"
#include "Protocol.h"

class CStorageManager
{
	bool IsFile(std::string &refstrFilePath);
	bool IsDirectory(std::string &refstrLogPath);
public:
	CStorageManager();
	~CStorageManager();

	DWORD ExecuteOpStore(ST_PROTO_UC &refProtoUC, char *pBuf);
	DWORD ExecuteOpLoad(ST_PROTO_UC &refProtoUC, char **ppBuf);
};


#endif
