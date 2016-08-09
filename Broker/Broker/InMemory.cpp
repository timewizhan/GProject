#include "InMemory.h"

CInMemory::CInMemory()
{
}

CInMemory::~CInMemory()
{
}

DWORD CInMemory::StoreMetaData(ST_PROTO_UB &refstProtoUB)
{
	ST_METADATA_INDEX stMetaDataIndex;
	stMetaDataIndex.strUID = "";
	stMetaDataIndex.strFileName = "";

	MapMetaData::iterator iter;
	iter = m_mapMDInMemory.find(stMetaDataIndex);
	if (iter == m_mapMDInMemory.end()) {
		ST_METADATA stMetaData;

		std::pair<ST_METADATA_INDEX, ST_METADATA> InsertPair(stMetaDataIndex, stMetaData);
		m_mapMDInMemory.insert(InsertPair);
	}
	else {
		iter->second.dwCloudNumber = 0;
		iter->second.dwIndex = 0;
		iter->second.dwSize = 0;
	}

	return E_RET_SUCCESS;
}

DWORD CInMemory::LoadMetaData(ST_PROTO_UB &refstProtoUB, ST_PROTO_BU &refstProtoBU)
{
	ST_METADATA_INDEX stMetaDataIndex;
	stMetaDataIndex.strUID = "";
	stMetaDataIndex.strFileName = "";

	MapMetaData::iterator iter;
	iter = m_mapMDInMemory.find(stMetaDataIndex);
	if (iter != m_mapMDInMemory.end()) {
		iter->second.dwCloudNumber = 0;
		iter->second.dwIndex = 0;
		iter->second.dwSize = 0;
	}
	
	return E_RET_SUCCESS;
}

DWORD CInMemory::StoreCloudResource(ST_PROTO_CB &refstProtoCB)
{
}

DWORD CInMemory::LoadAllCloudResource()
{
}

