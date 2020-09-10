/*

Minimal client fot the test filter driver.

Controls the devices through their I/O control codes and extracts
the intercepted IRPs and data.

Command line: execute TestClt /h for help


*/
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <CodeAnalysis/warnings.h>

#pragma warning(push)

#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)

#include <strsafe.h>

#pragma warning(pop)

#include <KrnlUtils.h>


#include "DrvR3.h"

// Command line defines
// --------------------

// Function codes
//
#define CLTFUN_CONTROL		L'c'
#define CLTFUN_GETDATA		L'd'
#define CLTFUN_HELP			L'h'
#define CLTFUN_LIST			L'l'

// CLTFUN_CONTROL subfunction codes
//
#define CONTROL_SUBFUNC_GET	L'g'
#define CONTROL_SUBFUNC_SET	L's'

// Processor architectures ids
//
#define	ARCH_STR_X86		L"x86"
#define	ARCH_STR_X64		L"x64"

// Windows versions ids
//
#define OSVER_STR_WXP		L"wxp"
#define OSVER_STR_WLH		L"wlh"
#define OSVER_STR_WIN7		L"win7"


// Other defines
// -------------

#define CLT_ERROR	100
#define DATA_BUFFER_SIZE	0x10000
#define BYTES_PER_LINE	16

BOOLEAN CheckParams(
	int		argC, 
	wchar_t	*lpwszArgV[]);

int DCGetMJFFlags(
	int			argC, 
	wchar_t		*lpwszArgV[]);

int DCSetMJFFlags(
	int			argC, 
	wchar_t		*lpwszArgV[]);

int DeviceControl(
	int		argC, 
	wchar_t	*lpwszArgV[]);

VOID DumpIoStackLocData(
	PCHAR				pStackData,
	ARCHIT				Arch,
	OSVER				OsVer);

VOID DumpIrp(
	PCHAR		pIrpData,
	SIZE_T		len,
	ARCHIT		Arch,
	OSVER		OsVer);

VOID DumpIrpData(
	PCHAR	pIrpData,
	ARCHIT	Arch,
	OSVER	OsVer);

VOID DumpMemory(
	PCHAR		pMem,
	SIZE_T		cbLen);

int GetData(
	int		argC,
	wchar_t	*lpwszArgV[]);

BOOL GetProcessHeapWrap(
	PHANDLE		phHeap);

BOOL HeapAllocWrap(
	HANDLE		hHeap, 
	SIZE_T		cbSize, 
	PVOID		*ppBuf);

BOOL ListDevices();

HANDLE OpenDevice(
  LPWSTR		lpwszDevName);

BOOL PrintAccDevList(
	PACCESS_DEVICES_LIST	pAdl);

VOID PrintHelp();

VOID PrintMultiSz(
  LPWSTR		pwszMulti );

BOOL SendIoCtl(
	HANDLE		hDevice, 
	INT			code,
	LPVOID		lpInBuffer,
	INT			inBufSize,
	LPVOID		lpOutBuffer,
	INT			outBufSize );

int wmain( int argC, wchar_t *lpwszArgV[], wchar_t *lpwszEnvV[]) {


	wchar_t	function;
	int		RetCode;

	wprintf(L"\nTest client, compiled %S %S\n\n",__DATE__, __TIME__);

	if (!CheckParams(argC, lpwszArgV)) {
		PrintHelp();
		return CLT_ERROR;
	}
	function = *(lpwszArgV[1] + 1);
	switch (function) {
		case CLTFUN_LIST:
			RetCode = ListDevices();
			break;
		case CLTFUN_CONTROL:
			RetCode = DeviceControl(argC, lpwszArgV);
			break;
		case CLTFUN_GETDATA:
			RetCode = GetData(argC, lpwszArgV);
			break;
		case CLTFUN_HELP:
			PrintHelp();
			RetCode = 0;
			break;
		default:
			wprintf(L"\nInvalid command: %s", lpwszArgV[1]);
			RetCode = CLT_ERROR;
	}
	return RetCode;
}


//++
// Function:	CheckParams
//
// Description:
//
//		Check the command line params.
//
// Arguments:
//
//		argC - from the command line.
//
//		lpwszArgV - from the command line.
//
//
// Return value: TRUE if the params are correct, FALSE otherwise.
//--
BOOLEAN CheckParams(
	int		argC, 
	wchar_t	*lpwszArgV[])
{
	if (argC < 2) {
		wprintf(L"\n\nCommand missing\n");
		return FALSE;
	}
	if (wcslen(lpwszArgV[1]) != 2) {
		wprintf(L"\n\nCommand format not valid: %s\n", lpwszArgV[1]);
		return FALSE;
	}
	if (*lpwszArgV[1] != L'/') {
		wprintf(L"\n\nCommand format not valid: %s\n", lpwszArgV[1]);
		return FALSE;
	}
	switch (*(lpwszArgV[1] + 1)) {
		case CLTFUN_LIST:
			return TRUE;
			break;
		case CLTFUN_CONTROL:
			if (argC < 5) {
				wprintf(L"\n\nMissing parameter(s)\n");
				return FALSE;
			}
			if ((*lpwszArgV[2] != CONTROL_SUBFUNC_GET) &&
				(*lpwszArgV[2] != CONTROL_SUBFUNC_SET)) {
				wprintf(L"\n\nInvalid subfunction code: %s\n",
					lpwszArgV[2]);
				return FALSE;
			}
			return TRUE;
			break;
		case CLTFUN_GETDATA:
			if (argC < 4) {
				wprintf(L"\n\nMissing parameter(s)\n");
				return FALSE;
			}
			return TRUE;
			break;
		case CLTFUN_HELP:
			return TRUE;
			break;
		default:
			wprintf(L"\n\nInvalid command: %s\n", lpwszArgV[1]);
			return FALSE;
	}
}


//++
// Function:	DCGetMJFFlags
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
int DCGetMJFFlags(
	int			argC, 
	wchar_t		*lpwszArgV[])
{
	wprintf(L"\n\nTo be implemented...\n");
	return CLT_ERROR;
}


//++
// Function:	DCSetMJFFlags
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
int DCSetMJFFlags(
	int			argC, 
	wchar_t		*lpwszArgV[])
{
	// lpwszArgV[3] = MajorFunction
	// lpwszArgV[4] = flags
	// lpwszArgV[5] = device name

	HANDLE		hFA;
	int			iMajFun;
	int			iFlags;
	MJF_CONTROL MJFCtl;

	if (swscanf_s(lpwszArgV[3], L"0x%x", &iMajFun) != 1) {
		wprintf(L"\nParameter parsing error: %s", lpwszArgV[3]);
		return CLT_ERROR;
	}
	if (swscanf_s(lpwszArgV[4], L"0x%x", &iFlags) != 1) {
		wprintf(L"\nParameter parsing error: %s", lpwszArgV[4]);
		return CLT_ERROR;
	}
	wprintf(L"\nSetting flags for MajorFunction = %#010x to %#010x",
		iMajFun,
		iFlags);
	MJFCtl.MajorFunction = (UCHAR) iMajFun;
	MJFCtl.dwFlags = (DWORD) iFlags;
	hFA = OpenDevice(lpwszArgV[5]);
	if (hFA == INVALID_HANDLE_VALUE) return CLT_ERROR;
	if (!SendIoCtl(hFA, IOCTL_FILTER_SET_MJF_FLAGS, &MJFCtl, 
		sizeof MJFCtl, NULL, 0)) {
		return CLT_ERROR;
	} else {
		return 0;
	}

}


//++
// Function:	DeviceControl
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
int DeviceControl(
	int		argC, 
	wchar_t	*lpwszArgV[])
{
	int ret;
	WCHAR	IoCtl;
	IoCtl = *lpwszArgV[2];

	switch (IoCtl) {
		case CONTROL_SUBFUNC_GET:
			ret = DCGetMJFFlags(argC, lpwszArgV);
			break;
		case CONTROL_SUBFUNC_SET:
			ret = DCSetMJFFlags(argC, lpwszArgV);
			break;
		default:
			wprintf(L"\nInvalid IOCTL code: %c", IoCtl);
			ret = CLT_ERROR;
	}
	return ret;
}



//++
// Function:	DumpIoStackLocData
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
VOID DumpIoStackLocData(
	PCHAR				pStackData,
	ARCHIT				Arch,
	OSVER				OsVer)
{
	PWCHAR		pFStack, pScan;
	if (FormatIoStack(pStackData, &pFStack, Arch, OsVer)) {
		for (pScan = pFStack;*pScan != L'\0';pScan += wcslen(pScan) + 1) {
			wprintf(L"%s\n", pScan);
		}
		FreeFmtStack(pFStack);
	}
	return;
}


//++
//--
VOID DumpIrp(
	PCHAR		pIrpData,
	SIZE_T		len,
	ARCHIT		Arch,
	OSVER		OsVer)
{
	IRP_HEADER	IrpHea;
	SIZE_T		offset;

	memcpy(&IrpHea, pIrpData, sizeof IrpHea);
	wprintf(L"\n\n---------------------------------------");
	wprintf(L"\n\nTraced IRP");
	wprintf(L"\n\nData Length: %#x", IrpHea.cbDataLen);
	wprintf(L"\n\nIRP:\n\n");
	offset = sizeof(IRP_HEADER);
	DumpIrpData(pIrpData + offset, Arch, OsVer);
	offset += IrpSize(Arch, OsVer);
	wprintf(L"\n\nI/O Stack location:\n\n");
	DumpIoStackLocData(pIrpData + offset, Arch, OsVer);
	offset += IoStackSize(Arch, OsVer);
	DumpMemory(pIrpData + offset, len - offset);
}


//++
// Function:	DumpIrpData
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
VOID DumpIrpData(
	PCHAR	pIrpData,
	ARCHIT	Arch,
	OSVER	OsVer)
{
	PWCHAR		pFIrp, pScan;
	if (FormatIrp(pIrpData, &pFIrp, Arch, OsVer)) {
		for (pScan = pFIrp;*pScan != L'\0';pScan += wcslen(pScan) + 1) {
			wprintf(L"%s\n", pScan);
		}
		FreeFmtIrp(pFIrp);
	}
	return;
}

//++
//--
VOID DumpMemory(
	PCHAR		pMem,
	SIZE_T		cbLen)
{
	UINT i, j, value;
	for (i = 0; i < cbLen; i++) {
		if (i % BYTES_PER_LINE == 0) printf ("\n0x%08x -", i);
		value = (UCHAR) *(pMem + i);
		printf(" %02x", value);
		if (((i + 1) % BYTES_PER_LINE == 0) && (i > 0)) {
			printf(" -- ");
			for (j = 0; j < BYTES_PER_LINE; j++) {
				CHAR ch = *(pMem + i - BYTES_PER_LINE + 1 + j);
				if (ch < 0x20) {
					printf(".");
				} else {
					printf("%c", ch);
				}
			}
		}
	}
}


//++
// Function:	GetData
//
// Description:
//
//		Extracts the data stored into the filter buffer and prints it
//		to the std out.
//
// Arguments:
//
//
// Return value: 
//--
int GetData(
	int		argC,
	wchar_t	*lpwszArgV[])
{
	// lpwszArgV[2] = archit
	// lpwszArgV[3] = OS ver
	// lpwszArgV[4] = dev name

	ARCHIT	Arch;
    HANDLE  hDevice = INVALID_HANDLE_VALUE;
	HANDLE	hHeap;
	DWORD	dwBytesRead;
	OSVER	OsVer;
	PCHAR	pData = NULL;
	int		Ret = CLT_ERROR;

	if (!wcscmp(lpwszArgV[2], ARCH_STR_X86)) {
		Arch = ARCH_X86;
	} else if (!wcscmp(lpwszArgV[2], ARCH_STR_X64)) {
		Arch = ARCH_X64;
	} else {
		wprintf(L"\nInvalid processor architecture: \"%s\"",
			lpwszArgV[2]);
		return CLT_ERROR;
	}

	if (!wcscmp(lpwszArgV[3], OSVER_STR_WXP)) {
		OsVer = OSVER_XP;
	} else if (!wcscmp(lpwszArgV[3], OSVER_STR_WLH)) {
		OsVer = OSVER_VISTA;
	} else if (!wcscmp(lpwszArgV[3], OSVER_STR_WIN7)) {
		OsVer = OSVER_WIN7;
	} else {
		wprintf(L"\nInvalid Windows Version: \"%s\"",
			lpwszArgV[3]);
		return CLT_ERROR;
	}


	if (!GetProcessHeapWrap(&hHeap)) {
		return CLT_ERROR;
	}
	if (!HeapAllocWrap(hHeap, DATA_BUFFER_SIZE, &pData)) {
		return CLT_ERROR;
	}
	hDevice = OpenDevice(lpwszArgV[4]);
	if (hDevice == INVALID_HANDLE_VALUE) goto CLEANUP;
	_tprintf(TEXT("Device succesfully opened\n"));
	for (;;) {
		if (ReadFile(hDevice, pData, DATA_BUFFER_SIZE, &dwBytesRead, 
			NULL)) {
			DumpIrp(pData, dwBytesRead, Arch, OsVer);
		}
	}
	Ret = 0;
CLEANUP:
	
	if (hDevice != INVALID_HANDLE_VALUE) CloseHandle(hDevice);
	if (pData != NULL) HeapFree(hHeap, 0, pData);
	return Ret;
}


//++
// Function:	
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
BOOL GetProcessHeapWrap(
	PHANDLE		phHeap)
{
	DWORD dwLastErr;
	*phHeap = GetProcessHeap();
	if (*phHeap == NULL) {
		dwLastErr = GetLastError();
		wprintf(
			L"\nGetProcessHeap failed; GetLastError() = %#010x (%d)",
			dwLastErr,
			dwLastErr);
		return FALSE;
	} else {
		return TRUE;
	}
}


//++
// Function:	
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
BOOL HeapAllocWrap(
	HANDLE		hHeap, 
	SIZE_T		cbSize, 
	PVOID		*ppBuf)
{
	*ppBuf = HeapAlloc(hHeap, 0, cbSize);
	if (*ppBuf == NULL) {
		wprintf(L"\nHeapAlloc failed");
		return FALSE;
	} else {
		return TRUE;
	}
}


//++
//--
BOOL ListDevices()
{
	DWORD					dwBytesRead;
	HANDLE					hHeap;
	HANDLE					hMasterDev;
	PACCESS_DEVICES_LIST	pAdl;

	hMasterDev = OpenDevice(MASTER_DEVICE_NAME);
	if (hMasterDev == INVALID_HANDLE_VALUE) return FALSE;
	if (!GetProcessHeapWrap(&hHeap)) return FALSE;
	if (!HeapAllocWrap(
		hHeap,
		4096, 
		&pAdl)) return FALSE;
	if (ReadFile(hMasterDev, pAdl, 4096, &dwBytesRead, NULL)) {
		PrintAccDevList((PACCESS_DEVICES_LIST) pAdl);
	}
	return TRUE;
}

//++
//--
HANDLE OpenDevice(
  LPWSTR		lpwszDevName)
{
	HANDLE		hDevice;
	WCHAR		wszDevPath[501];

	HRESULT res = StringCbPrintfW(
		wszDevPath,
		sizeof wszDevPath,
		L"\\\\.\\%s",
		lpwszDevName);
	if (!SUCCEEDED(res)) {
		wprintf(L"\nOpenDevice - StringCbPrintfW() returned %#x", res);
		return INVALID_HANDLE_VALUE;
	}

    hDevice =
        CreateFile( wszDevPath,
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,	// no security
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					NULL );		// no template

	if ( hDevice == INVALID_HANDLE_VALUE ) {
		wprintf( L"Failed to obtain file handle to %ls. "
			L"GetLastError() = %d (0x%x)\n",
			lpwszDevName,
			GetLastError(),
			GetLastError());
        return hDevice;
	}
    return hDevice;

}


//++
//--
BOOL PrintAccDevList(
	PACCESS_DEVICES_LIST	pAdl)
{
	UINT	i;
	wprintf(
		L"\nComplete data:       %s\n",
		pAdl->bIncomplete ? L"no": L"yes" );
	wprintf(
		L"\nBytes needed:        %d\n",
		pAdl->ReqBufSize);

	for (i = 0; i < pAdl->DevInfoCount; i++) {
		wprintf(
			L"\n"
			L"Device:         %s\n",
			pAdl->TargDevVect[i].SymLink );
		wprintf(
			L"Text:              %s\n",
			pAdl->TargDevVect[i].DeviceText );
		wprintf(
			L"Device ID:         %s\n",
			pAdl->TargDevVect[i].DeviceId );
		wprintf(
			L"Instance ID:       %s\n",
			pAdl->TargDevVect[i].InstanceId );
		wprintf(
			L"Hardware IDs:      ");
		PrintMultiSz(pAdl->TargDevVect[i].HardwareIDs);
		wprintf(L"\n");
		wprintf(
			L"Compatible IDs:    ");
		PrintMultiSz(pAdl->TargDevVect[i].CompatibleIDs);
		wprintf(L"\n");
	}

	return FALSE;
}


//++
//--
VOID PrintHelp()
{
	wprintf(L"\n\nUsage:");
	wprintf(L"\n\n    List devices:");
	wprintf(L"\n\n        TestClt /l");
	wprintf(L"\n\n    Set flags for IRP major function code:");
	wprintf(L"\n\n        TestClt /c s major_function flags symbolic_link");
	wprintf(L"\n\n            major_function: IRP major function code in hex");
	wprintf(L"\n\n            flags         : flags value in hex");
	wprintf(L"\n\n            symbolic_link : sym link for the FA device");
	wprintf(L"\n\n    Get intercepted data:");
	wprintf(L"\n\n        TestClt /d archit osver symbolic_link");
	wprintf(L"\n\n            archit        : processor architecture (x86, x64)");
	wprintf(L"\n\n            osver         : windows version (wxp, vista, win7)");
	wprintf(L"\n\n            symbolic_link : sym link for the FA device");
	wprintf(L"\n");
}


//++
//--
VOID PrintMultiSz(
  LPWSTR		pwszMulti )
{
	SIZE_T StrLen;

	for (;;) {
		wprintf(pwszMulti);
		StrLen = wcslen(pwszMulti);
		pwszMulti += StrLen + 1;
		if (*pwszMulti == L'\0') break;
		wprintf(L", ");
	} 

}


//++
//--
BOOL SendIoCtl(
	HANDLE		hDevice, 
	INT			code,
	LPVOID		lpInBuffer,
	INT			inBufSize,
	LPVOID		lpOutBuffer,
	INT			outBufSize )
{
	BOOL	bResult;
	DWORD	dwBytesRet;

	bResult = DeviceIoControl(
		hDevice,
		code,
		lpInBuffer,
		inBufSize,
		lpOutBuffer,
		outBufSize,
		&dwBytesRet,
		NULL);

	if (!bResult) {
		_tprintf( TEXT("Failed to send IOCTL. Code: 0x%8x, GetLastError() = 0x%8x, (%i)"),
			code,
			GetLastError(),
			GetLastError() );
		return bResult;
	}

	return bResult;
}

