#include "ResourceManager.h"
#include <stdio.h>


VOID CResourceManager::GetUsedBandWidth(ST_RM_DATA &refstRMData)
{
	PDH_HQUERY networkQuery;
	PDH_HCOUNTER networkCounter[4];

	::PdhOpenQueryA(NULL, NULL, &networkQuery);
	//::PdhAddCounterA(networkQuery, "\\Network Interface(*)\\Current Bandwidth", NULL, &networkCounter[0]);
	::PdhAddCounterA(networkQuery, "\\Network Interface(*)\\Bytes Received/sec", NULL, &networkCounter[1]);
	//::PdhAddCounterA(networkQuery, "\\Network Interface(*)\\Bytes Sent/sec", NULL, &networkCounter[2]);
	//::PdhAddCounterA(networkQuery, "\\Network Interface(*)\\Bytes Total/sec", NULL, &networkCounter[3]);
	::PdhCollectQueryData(networkQuery);

	PDH_FMT_COUNTERVALUE stCounterValue;
	PDH_STATUS status;
	/*status = ::PdhGetFormattedCounterValue(networkCounter[0], PDH_FMT_LONG, NULL, &stCounterValue);
	printf("%ld\n", stCounterValue.longValue);*/
	
	status = ::PdhGetFormattedCounterValue(networkCounter[1], PDH_FMT_LONG, NULL, &stCounterValue);
	printf("%ld\n", stCounterValue.longValue);
	
	/*status = ::PdhGetFormattedCounterValue(networkCounter[2], PDH_FMT_LONG, NULL, &stCounterValue);
	printf("%ld\n", stCounterValue.longValue);

	status = ::PdhGetFormattedCounterValue(networkCounter[3], PDH_FMT_LONG, NULL, &stCounterValue);
	printf("%ld\n", stCounterValue.longValue);*/
	::PdhCloseQuery(networkQuery);
}

VOID CResourceManager::GetUsedCPU(ST_RM_DATA &refstRMData)
{
	PDH_HQUERY cpuQuery;
	PDH_HCOUNTER cpuTotal;

	::PdhOpenQueryA(NULL, NULL, &cpuQuery);
	::PdhAddCounterA(cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	::PdhCollectQueryData(cpuQuery);

	PDH_FMT_COUNTERVALUE stCounterValue;

	::PdhCollectQueryData(cpuQuery);
	::PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &stCounterValue);

	stCounterValue.doubleValue;
}

VOID CResourceManager::GetUsedMemory(ST_RM_DATA &refstRMData)
{
	MEMORYSTATUSEX stMEMInfo;
	stMEMInfo.dwLength = sizeof(MEMORYSTATUSEX);
	::GlobalMemoryStatusEx(&stMEMInfo);

	DWORDLONG dwlTotalPhysMem = stMEMInfo.ullTotalPhys;
	DWORDLONG dwlPhysMemUsed = stMEMInfo.ullAvailPhys;
}

VOID CResourceManager::GetUsedDiskIO(ST_RM_DATA &refstRMData)
{
	PDH_HQUERY cpuQuery;
	PDH_HCOUNTER cpuTotal;

	::PdhOpenQueryA(NULL, NULL, &cpuQuery);
	//::PdhAddCounterA(cpuQuery, "\\LogicalDisk(*)\\% Free Space", NULL, &cpuTotal);
	::PdhAddCounterA(cpuQuery, "\\LogicalDisk(_Total)\\Disk Bytes/sec", NULL, &cpuTotal);
	::PdhCollectQueryData(cpuQuery);

	PDH_FMT_COUNTERVALUE stCounterValue;

	::PdhCollectQueryData(cpuQuery);
	::PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_LONG, NULL, &stCounterValue);

	printf("%ld\n", stCounterValue.longValue);
}

VOID CResourceManager::GetResourceData(ST_RM_DATA &refstRMData)
{
	ST_RM_DATA stRMData;

	GetUsedBandWidth(stRMData);
	GetUsedCPU(stRMData);
	GetUsedMemory(stRMData);
	GetUsedDiskIO(stRMData);

	refstRMData = stRMData;
}