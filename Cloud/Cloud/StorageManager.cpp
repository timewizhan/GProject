#include "StorageManager.h"

CStorageManager::CStorageManager()
{
}

CStorageManager::~CStorageManager()
{
}

bool CStorageManager::IsFile(std::string &refstrFilePath)
{
	DWORD dwRet = GetFileAttributesA(refstrFilePath.c_str());
	if (dwRet == INVALID_FILE_ATTRIBUTES)
		return false;

	if (dwRet & FILE_ATTRIBUTE_ARCHIVE)
		return true;

	return false;
}

bool CStorageManager::IsDirectory(std::string &refstrLogPath)
{
	DWORD dwRet = GetFileAttributesA(refstrLogPath.c_str());
	if (dwRet == INVALID_FILE_ATTRIBUTES)
		return false;

	if (dwRet & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

DWORD CStorageManager::ExecuteOpStore(ST_PROTO_UC &refProtoUC, char *pBuf)
{
	// Only C Drive
	const std::string cStaticPath = "C:";

	std::string strBasicPath = cStaticPath + "\\" + refProtoUC.strUID + "\\" + refProtoUC.strFileName + "\\" + std::to_string(refProtoUC.dwIndex);
	std::string strFileName = refProtoUC.strUID + "_" + refProtoUC.strFileName + "_" + std::to_string(refProtoUC.dwIndex);
	std::string strFullPath = strBasicPath + "\\" + strFileName;

	FILE *pFile = NULL;
	errno_t err;
	err = fopen_s(&pFile, strFullPath.c_str(), "w");
	if (err != 0) {
		ErrorLog("Fail to create file point");
		return E_RET_FAIL;
	}

	DWORD dwWritten;
	dwWritten = ::fwrite(pBuf, 1, refProtoUC.dwSize, pFile);
	if (dwWritten != refProtoUC.dwSize) {
		ErrorLog("Fail to make file");
	}
	else {
		DebugLog("Success to make file");
	}

	::fclose(pFile);
	return E_RET_SUCCESS;
}

DWORD CStorageManager::ExecuteOpLoad(ST_PROTO_UC &refProtoUC, char **ppBuf)
{
	// Only C Drive
	const std::string cStaticPath = "C:";

	std::string strBasicPath = cStaticPath + "\\" + refProtoUC.strUID + "\\" + refProtoUC.strFileName + "\\" + std::to_string(refProtoUC.dwIndex);
	std::string strFileName = refProtoUC.strUID + "_" + refProtoUC.strFileName + "_" + std::to_string(refProtoUC.dwIndex);

	if (!IsDirectory(strBasicPath)) {
		ErrorLog("Invalid Dir Path : %s", strBasicPath.c_str());
		return E_RET_FAIL;
	}

	std::string strFullPath = strBasicPath + "\\" + strFileName;
	if (!IsFile(strFullPath)) {
		ErrorLog("Invalid File Path : %s", strFullPath.c_str());
		return E_RET_FAIL;
	}

	FILE *pFile = NULL;
	errno_t err;
	err = fopen_s(&pFile, strFullPath.c_str(), "r");
	if (err != 0) {
		ErrorLog("Fail to create file point");
		return E_RET_FAIL;
	}

	DWORD dwWritten;
	dwWritten = ::fread(*ppBuf, refProtoUC.dwSize, 1, pFile);
	if (dwWritten != refProtoUC.dwSize) {
		ErrorLog("Fail to make file");
	}
	else {
		DebugLog("Success to make file");
	}

	::fclose(pFile);
	return E_RET_SUCCESS;
}
