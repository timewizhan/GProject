#ifndef _RESOURCE_MANAGER_
#define _RESOURCE_MANAGER_

#include "..\Common\Common.h"
#include <pdh.h>
#include <PdhMsg.h>

#pragma comment(lib, "Pdh")

struct ST_RM_DATA
{
	//BandWidth
	long lCurrentBandwidth;

	//CPU
	long lCurrentAvailCPU;

	//Memory
	long lCurrentAvailMem;

	//DiskIO
	long lCurrentAvailDiskID;
};

class CResourceManager
{

	VOID GetUsedBandWidth(ST_RM_DATA &refstRMData);
	VOID GetUsedCPU(ST_RM_DATA &refstRMData);
	VOID GetUsedMemory(ST_RM_DATA &refstRMData);
	VOID GetUsedDiskIO(ST_RM_DATA &refstRMData);
public:
	CResourceManager() {}
	~CResourceManager() {}

	VOID GetResourceData(ST_RM_DATA &refstRMData);

};


#endif