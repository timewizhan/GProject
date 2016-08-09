#ifndef _PROTOCOL_
#define _PROTOCOL_

#include "..\Common\Common.h"

// User -> Cloud
enum E_PROTO_UC
{
	E_PROTO_UC_STORE = 1,
	E_PROTO_UC_LOAD,


	E_PROTO_UNKNOWN = 999
};

// Cloud -> User
enum E_PROTO_CU
{
	E_PROTO_CU_RES = 1,
};

// User -> Broker
enum E_PROTO_UB
{
	E_PROTO_UB_STORE = 1,
	E_PROTO_UB_LOAD,

	E_PROTO_UB_UNKNOWN = 999
};

// Broker -> User
enum E_PROTO_BU
{
	E_PROTO_BU_LOAD = 1,

	E_PROTO_BU_UNKNOWN = 999
};

// Cloud -> Broker
enum E_PROTO_CB
{
	E_PROTO_CB_RESOURCE = 1,

	E_PROTO_CB_UNKNOWN = 999
};

// Broker -> Cloud
enum E_PROTO_BC
{
	E_PROTO_BC_UNKNOWN = 999
};

struct ST_PROTO_UC
{
	DWORD dwAction;
	std::string strUID;
	std::string strFileName;
	DWORD		dwIndex;
	DWORD		dwSize;
};

struct ST_PROTO_UB
{
	DWORD dwAction;

};

struct ST_PROTO_BU
{
	DWORD dwAction;
};

struct ST_PROTO_CB
{
	DWORD dwAction;
};

#endif