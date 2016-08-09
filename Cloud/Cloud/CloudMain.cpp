#include "..\Common\Common.h"
#include "EntryPoint.h"


int main(int argc, char **argv)
{
	if (argc < 3) {
		printf("Usage : [IP Address] [Port]\n");
		return 0;
	}

	std::string strIPAddress = argv[1];
	DWORD dwPort = ::atoi(argv[2]);

	CEntryPoint EP;

	DWORD dwRet;
	dwRet = EP.StartEntryPoint(strIPAddress, dwPort);
	if (dwRet != E_RET_SUCCESS) {
		ErrorLog("Fail to start Entry Point");
		return E_RET_FAIL;
	}

	DebugLog("Success to start Entry Point");
	return E_RET_SUCCESS;
}
