#ifndef _IN_MEMORY_
#define _IN_MEMORY_

#include "..\Common\Common.h"
#include "Protocol.h"

struct ST_METADATA_INDEX
{
	std::string strUID;
	std::string strFileName;

	bool operator==(ST_METADATA_INDEX &refstMDIndex) {
		if (!strUID.compare(refstMDIndex.strUID) && !strFileName.compare(refstMDIndex.strFileName)) {
			return true;
		}
		return false;
	}
};

struct ST_METADATA
{
	DWORD dwCloudNumber;
	DWORD dwSize;
	DWORD dwIndex;
};

struct ST_CLOUD_RESOURCE
{
	
};

class CInMemory
{
	typedef std::map<ST_METADATA_INDEX, ST_METADATA> MapMetaData;
	MapMetaData m_mapMDInMemory;

	typedef std::vector<ST_CLOUD_RESOURCE> VecCloudResource;
	VecCloudResource m_vecCloudResource;

public:
	CInMemory();
	~CInMemory();

	DWORD StoreMetaData(ST_PROTO_UB &refstProtoUB);
	DWORD LoadMetaData(ST_PROTO_UB &refstProtoUB, ST_PROTO_BU &refstProtoBU);

	DWORD StoreCloudResource(ST_PROTO_CB &refstProtoCB);
	DWORD LoadAllCloudResource();
};



#endif