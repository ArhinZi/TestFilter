#include <windows.h>

#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#include <strsafe.h>
#pragma warning(pop)


#include "KrnlUtils.h"
#include "KUPrivate.h"
#include "WxpX86.h"
#include "WlhX86.h"
#include "WlhX64.h"
#include "Win7X64.h"
#include "Win7X86.h"
#include "krnldefs.h"



//----------------------------------------------------------------------

// Static variables
// ----------------

PWSTR pwszMajorFunctionV[] = {
	L"IRP_MJ_CREATE",
	L"IRP_MJ_CREATE_NAMED_PIPE",
	L"IRP_MJ_CLOSE",
	L"IRP_MJ_READ",
	L"IRP_MJ_WRITE",
	L"IRP_MJ_QUERY_INFORMATION",
	L"IRP_MJ_SET_INFORMATION",
	L"IRP_MJ_QUERY_EA",
	L"IRP_MJ_SET_EA",
	L"IRP_MJ_FLUSH_BUFFERS",
	L"IRP_MJ_QUERY_VOLUME_INFORMATION",
	L"IRP_MJ_SET_VOLUME_INFORMATION",
	L"IRP_MJ_DIRECTORY_CONTROL",
	L"IRP_MJ_FILE_SYSTEM_CONTROL",
	L"IRP_MJ_DEVICE_CONTROL",
	L"IRP_MJ_INTERNAL_DEVICE_CONTROL",
	L"IRP_MJ_SHUTDOWN",
	L"IRP_MJ_LOCK_CONTROL",
	L"IRP_MJ_CLEANUP",
	L"IRP_MJ_CREATE_MAILSLOT",
	L"IRP_MJ_QUERY_SECURITY",
	L"IRP_MJ_SET_SECURITY",
	L"IRP_MJ_POWER",
	L"IRP_MJ_SYSTEM_CONTROL",
	L"IRP_MJ_DEVICE_CHANGE",
	L"IRP_MJ_QUERY_QUOTA",
	L"IRP_MJ_SET_QUOTA",
	L"IRP_MJ_PNP",
};

PWSTR pwszPnpMinorFunctionV[] = {
	L"IRP_MN_START_DEVICE",
	L"IRP_MN_QUERY_REMOVE_DEVICE",
	L"IRP_MN_REMOVE_DEVICE",
	L"IRP_MN_CANCEL_REMOVE_DEVICE",
	L"IRP_MN_STOP_DEVICE",
	L"IRP_MN_QUERY_STOP_DEVICE",
	L"IRP_MN_CANCEL_STOP_DEVICE",
	L"IRP_MN_QUERY_DEVICE_RELATIONS",
	L"IRP_MN_QUERY_INTERFACE",
	L"IRP_MN_QUERY_CAPABILITIES",
	L"IRP_MN_QUERY_RESOURCES",
	L"IRP_MN_QUERY_RESOURCE_REQUIREMENTS",
	L"IRP_MN_QUERY_DEVICE_TEXT",
	L"IRP_MN_FILTER_RESOURCE_REQUIREMENTS",
	L"unknown",
	L"IRP_MN_READ_CONFIG",
	L"IRP_MN_WRITE_CONFIG",
	L"IRP_MN_EJECT",
	L"IRP_MN_SET_LOCK",
	L"IRP_MN_QUERY_ID",
	L"IRP_MN_QUERY_PNP_DEVICE_STATE",
	L"IRP_MN_QUERY_BUS_INFORMATION",
	L"IRP_MN_DEVICE_USAGE_NOTIFICATION",
	L"IRP_MN_SURPRISE_REMOVAL",
};

PWSTR pwszPowerMinorFunctionV[] = {
	L"IRP_MN_WAIT_WAKE",
	L"IRP_MN_POWER_SEQUENCE",
	L"IRP_MN_SET_POWER",
	L"IRP_MN_QUERY_POWER",
};

//----------------------------------------------------------------------
PVOID AllocFromHeap(
	HANDLE		hHeap, 
	SIZE_T		cbBufSize)
{
	PVOID pMem = HeapAlloc(hHeap, 0, cbBufSize);
	if (pMem == NULL) {

		// HeapAlloc does not call SetLastError() on its own.
		//
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
	}
	return pMem;
}


//----------------------------------------------------------------------
BOOL AppendString(
	PWSTR		wszItem, 
	PCHAR*		ppFmtBuf, 
	SIZE_T*		pcbOffset, 
	SIZE_T*		pcbBufSize, 
	HANDLE		hHeap)
{
	SIZE_T	cbItemLen;

	StringCbLength(wszItem, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	return CopyToBuffer(ppFmtBuf, pcbOffset, (PCHAR) wszItem, 
		cbItemLen, pcbBufSize, hHeap);
}


//----------------------------------------------------------------------
BOOL CopyToBuffer(
	PCHAR*		ppBuffer, 
	SIZE_T*		pcbOffset,
	PCHAR		pSource, 
	SIZE_T		cbLen, 
	SIZE_T*		pcbBufSize, 
	HANDLE		hHeap)
{
	SIZE_T cbNeeded = *pcbOffset + cbLen;
	if (cbNeeded > *pcbBufSize) {
		if (!ExtendBuffer(ppBuffer, pcbBufSize, cbNeeded, hHeap)) {
			return FALSE;
		}
	}
	memcpy(*ppBuffer + *pcbOffset, pSource, cbLen);
	*pcbOffset += cbLen;
	return TRUE;
}


//----------------------------------------------------------------------
PWSTR DecodeEnum(
	INT			Value, 
	INT			Base,
	PWSTR		pwszDecodeV[],
	UINT		VectLen,
	PWSTR		pwszOutBuf, 
	SIZE_T		cbBufSize)
{
	INT Index = Value - Base;
	if ((Index >= 0) && ((UINT) Index < VectLen)) {
		StringCbCopyW(pwszOutBuf, cbBufSize, pwszDecodeV[Index]);
	} else {
		StringCbPrintfW(pwszOutBuf, cbBufSize, L"%#x", Value);
	}

	return pwszOutBuf;
}


//----------------------------------------------------------------------
BOOL ExtendBuffer(
	PCHAR*		ppBuffer, 
	SIZE_T*		pcbBufSize, 
	SIZE_T		cbNeeded, 
	HANDLE		hHeap)
{
	PCHAR pNew = (PCHAR) AllocFromHeap(hHeap, cbNeeded);
	if (pNew == NULL) return FALSE;
	memcpy(pNew, *ppBuffer, *pcbBufSize);
	HeapFree(hHeap, 0, *ppBuffer);
	*ppBuffer = pNew;
	*pcbBufSize = cbNeeded;

	return TRUE;
}


//----------------------------------------------------------------------
BOOL FormatIoStack(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack, 
	ARCHIT		Arch, 
	OSVER		OsVer)
{
	if ((Arch == ARCH_X86) && (OsVer == OSVER_XP)) {
		return FormatIoStackWxpX86(pStackData, ppFStack);
	} else if ((Arch == ARCH_X86) && (OsVer == OSVER_VISTA)) {
		return FormatIoStackWlhX86(pStackData, ppFStack);
	} else if ((Arch == ARCH_X86) && (OsVer == OSVER_WIN7)) {
		return FormatIoStackWin7X86(pStackData, ppFStack);
	} else if ((Arch == ARCH_X64) && (OsVer == OSVER_VISTA)) {
		return FormatIoStackWlhX64(pStackData, ppFStack);
	} else if ((Arch == ARCH_X64) && (OsVer == OSVER_WIN7)) {
		return FormatIoStackWin7X64(pStackData, ppFStack);
	} else {
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}
}



//----------------------------------------------------------------------
BOOL FormatIrp(
	PCHAR		pIrpData,
	PWCHAR		*ppFIrp,
	ARCHIT		Arch,
	OSVER		OsVer)
{
	if ((Arch == ARCH_X86) && (OsVer == OSVER_XP)) {
		return FormatIrpWxpX86(pIrpData, ppFIrp);
	} else if ((Arch == ARCH_X86) && (OsVer == OSVER_VISTA)) {
		return FormatIrpWlhX86(pIrpData, ppFIrp);
	} else if ((Arch == ARCH_X86) && (OsVer == OSVER_WIN7)) {
		return FormatIrpWin7X86(pIrpData, ppFIrp);
	} else if ((Arch == ARCH_X64) && (OsVer == OSVER_VISTA)) {
		return FormatIrpWlhX64(pIrpData, ppFIrp);
	} else if ((Arch == ARCH_X64) && (OsVer == OSVER_WIN7)) {
		return FormatIrpWin7X64(pIrpData, ppFIrp);
	} else {
		SetLastError(ERROR_INVALID_PARAMETER);
		return FALSE;
	}

}


//----------------------------------------------------------------------
VOID FreeFmtIrp(
	PWCHAR		pFIrp)
{
	HANDLE hHeap = GetProcessHeap();
	if (hHeap != NULL) {
		HeapFree(hHeap, 0, pFIrp);
	}
}


//----------------------------------------------------------------------
VOID FreeFmtStack(
	PWCHAR		pFStack)
{
	HANDLE hHeap = GetProcessHeap();
	if (hHeap != NULL) {
		HeapFree(hHeap, 0, pFStack);
	}
}


//----------------------------------------------------------------------
SIZE_T IoStackSize(
	ARCHIT		Arch, 
	OSVER		OsVer)
{
	if ((Arch == ARCH_X86) && (OsVer == OSVER_XP)) {
		return IoStackSizeWxpX86();
	} else if ((Arch == ARCH_X86) && (OsVer == OSVER_VISTA)) {
		return IoStackSizeWlhX86();
	} else if ((Arch == ARCH_X86) && (OsVer == OSVER_WIN7)) {
		return IoStackSizeWin7X86();
	} else if ((Arch == ARCH_X64) && (OsVer == OSVER_VISTA)) {
		return IoStackSizeWlhX64();
	} else if ((Arch == ARCH_X64) && (OsVer == OSVER_WIN7)) {
		return IoStackSizeWin7X64();
	} else {
		return 0;
	}
}


//----------------------------------------------------------------------
SIZE_T IrpSize(
	ARCHIT		Arch, 
	OSVER		OsVer)
{
	if ((Arch == ARCH_X86) && (OsVer == OSVER_XP)) {
		return IrpSizeWxpX86();
	} else if ((Arch == ARCH_X86) && (OsVer == OSVER_VISTA)) {
		return IrpSizeWlhX86();
	} else if ((Arch == ARCH_X86) && (OsVer == OSVER_WIN7)) {
		return IrpSizeWin7X86();
	} else if ((Arch == ARCH_X64) && (OsVer == OSVER_VISTA)) {
		return IrpSizeWlhX64();
	} else if ((Arch == ARCH_X64) && (OsVer == OSVER_WIN7)) {
		return IrpSizeWin7X64();
	} else {
		return 0;
	}
}


//----------------------------------------------------------------------
LPWSTR	MajorFunctionDes(
	 UCHAR		MajorFunction,
	 PWSTR		pwszDecodeBuf,
	 SIZE_T		cbBufSize)
{
	return DecodeEnum(
		MajorFunction,
		0,
		pwszMajorFunctionV,
		sizeof pwszMajorFunctionV / sizeof(PWSTR),
		pwszDecodeBuf,
		cbBufSize);
}


//----------------------------------------------------------------------
LPWSTR MinorFunctionDes(
	UCHAR		MajorFunction,
	UCHAR		MinorFunction,
	PWSTR		pwszDecodeBuf,
	SIZE_T		cbBufSize)
{
	switch (MajorFunction) {
		case IRP_MJ_PNP:
			return DecodeEnum(
				MinorFunction,
				0,
				pwszPnpMinorFunctionV,
				sizeof pwszPnpMinorFunctionV / sizeof (PWSTR),
				pwszDecodeBuf,
				cbBufSize);
			break;
		case IRP_MJ_POWER:
			return DecodeEnum(
				MinorFunction,
				0,
				pwszPowerMinorFunctionV,
				sizeof pwszPowerMinorFunctionV / sizeof (PWSTR),
				pwszDecodeBuf,
				cbBufSize);
			break;
		default:
			StringCbPrintfW(
				pwszDecodeBuf,
				cbBufSize,
				L"%#x",
				MinorFunction);
			return pwszDecodeBuf;
	}
}
