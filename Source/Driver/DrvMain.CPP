/*
=======================================================================

Test Filter Driver



=======================================================================
*/



//----------------------------------------------------------------------
//
// Includes
// --------


extern "C" {
#include <NTDDK.h>
#include <ntstrsafe.h>
}

// Defs shared with ring 3 components.
//
#include "DrvR3.h"

// Circular buffer functions
//
#include "CircBuffer.h"

//----------------------------------------------------------------------
//
// Defines
// -------

#define MJ_FLAGS_DEFAULT	0x00

//----------------------------------------------------------------------
//
// Data Types
// ----------

// This enumeration defines the device types
//
typedef enum {

	// The single master device.
	//
	DT_MASTER,

	// A filter device attached to a device being filtered
	//
	DT_FILTER,

	// A filter access device used to interact with a filter device
	// from a client
	//
	DT_FILTER_ACCESS
} MY_DEVICE_TYPE;

// Data members common to the filter device and the filter access 
// device.
//
typedef struct _COMMON_DEVICE_DATA {

	// Possible types: filter device, filter access device, master 
	//	device, etc.
	//
	MY_DEVICE_TYPE		DevType;

	// Back pointer to device object
	//
	PDEVICE_OBJECT	pDevice;

} COMMON_DEVICE_DATA, *PCOMMON_DEVICE_DATA;


// Device ext for the filter device
//
typedef struct _DEVICE_EXTENSION {

	COMMON_DEVICE_DATA hdr;

	// DPC used to schedule completion of client IRPs
	//
	KDPC	ComplDpc;


	// Data buffer control variables.
	//
	CIRCULAR_BUFFER	DataBuffer;

	// Spinlock which protects the data buffer and the
	// IRP queues.
	//
	KSPIN_LOCK	DataLock;

	// Flags which control the filter behavour
	DWORD		dwFlags;

	// Array of flags one for each IRP_MJ_... value.
	//
	DWORD dwMJFlags[IRP_MJ_MAXIMUM_FUNCTION + 1];

	// Flags which store information on the filter status.
	//
	DWORD		dwStatus;

	// Address of the extension for the filter access device
	//
	struct _FADEVICE_EXTENSION*	pFADevExt;

	// List of the client IRPs waiting for intercepted data.
	//
	LIST_ENTRY	pIrpList;

	PDEVICE_OBJECT pTargStack;

	// List of the client IRPs waiting to be completed
	//
	LIST_ENTRY	pWaitingComplIrpList;

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;



// Device ext for the filter access device
//
typedef struct _FADEVICE_EXTENSION {

	COMMON_DEVICE_DATA hdr;

	// Ptr to dev ext of associated filter device
	//
	PDEVICE_EXTENSION	pFilterDevExt;

	// Filtered device informational data
	//
	TARGET_DEVICE_INFO	TargDevInfo;

	// List entry to chain the FA extensions in a list
	LIST_ENTRY			ListEntry;

} FADEVICE_EXTENSION, *PFADEVICE_EXTENSION;

// Device extension fot the master device
//
typedef struct _MADEVICE_EXTENSION {
	COMMON_DEVICE_DATA hdr;

	KSPIN_LOCK		SyncLock;

	// Linked list of FA devices.
	//
	LIST_ENTRY		FADevsList;

	// Number of FA extensions in the list.
	//
	ULONG				FADevsCount;
} MADEVICE_EXTENSION, *PMADEVICE_EXTENSION;


// Str. of an item queued in the client IRP queue
//
typedef struct _QUEUED_IRP{
	LIST_ENTRY		listEntry;
	PIRP			pIrp;
} QUEUED_IRP, *PQUEUED_IRP;


//----------------------------------------------------------------------
//
// Static variables
// ----------------

static LONG	DevNumber = 0;
static PMADEVICE_EXTENSION pMastDevExt = NULL;



// Table for MajorFunction decoding
//
PSTR pszMajorFunctionV[] = {
	"IRP_MJ_CREATE",
	"IRP_MJ_CREATE_NAMED_PIPE",
	"IRP_MJ_CLOSE",
	"IRP_MJ_READ",
	"IRP_MJ_WRITE",
	"IRP_MJ_QUERY_INFORMATION",
	"IRP_MJ_SET_INFORMATION",
	"IRP_MJ_QUERY_EA",
	"IRP_MJ_SET_EA",
	"IRP_MJ_FLUSH_BUFFERS",
	"IRP_MJ_QUERY_VOLUME_INFORMATION",
	"IRP_MJ_SET_VOLUME_INFORMATION",
	"IRP_MJ_DIRECTORY_CONTROL",
	"IRP_MJ_FILE_SYSTEM_CONTROL",
	"IRP_MJ_DEVICE_CONTROL",
	"IRP_MJ_INTERNAL_DEVICE_CONTROL",
	"IRP_MJ_SHUTDOWN",
	"IRP_MJ_LOCK_CONTROL",
	"IRP_MJ_CLEANUP",
	"IRP_MJ_CREATE_MAILSLOT",
	"IRP_MJ_QUERY_SECURITY",
	"IRP_MJ_SET_SECURITY",
	"IRP_MJ_POWER",
	"IRP_MJ_SYSTEM_CONTROL",
	"IRP_MJ_DEVICE_CHANGE",
	"IRP_MJ_QUERY_QUOTA",
	"IRP_MJ_SET_QUOTA",
	"IRP_MJ_PNP",
};

// Table for PNP MinorFunction decoding
//
PSTR pszPnpMinorFunctionV[] = {
	"IRP_MN_START_DEVICE",
	"IRP_MN_QUERY_REMOVE_DEVICE",
	"IRP_MN_REMOVE_DEVICE",
	"IRP_MN_CANCEL_REMOVE_DEVICE",
	"IRP_MN_STOP_DEVICE",
	"IRP_MN_QUERY_STOP_DEVICE",
	"IRP_MN_CANCEL_STOP_DEVICE",
	"IRP_MN_QUERY_DEVICE_RELATIONS",
	"IRP_MN_QUERY_INTERFACE",
	"IRP_MN_QUERY_CAPABILITIES",
	"IRP_MN_QUERY_RESOURCES",
	"IRP_MN_QUERY_RESOURCE_REQUIREMENTS",
	"IRP_MN_QUERY_DEVICE_TEXT",
	"IRP_MN_FILTER_RESOURCE_REQUIREMENTS",
	"unknown",
	"IRP_MN_READ_CONFIG",
	"IRP_MN_WRITE_CONFIG",
	"IRP_MN_EJECT",
	"IRP_MN_SET_LOCK",
	"IRP_MN_QUERY_ID",
	"IRP_MN_QUERY_PNP_DEVICE_STATE",
	"IRP_MN_QUERY_BUS_INFORMATION",
	"IRP_MN_DEVICE_USAGE_NOTIFICATION",
	"IRP_MN_SURPRISE_REMOVAL",
};

// Table for power MinorFunction decoding
//
PSTR pszPowerMinorFunctionV[] = {
	"IRP_MN_WAIT_WAKE",
	"IRP_MN_POWER_SEQUENCE",
	"IRP_MN_SET_POWER",
	"IRP_MN_QUERY_POWER",
};



//----------------------------------------------------------------------
//
// Forward declarations
// --------------------

static NTSTATUS AddDevice(
		IN PDRIVER_OBJECT	pDriverObject,
		IN PDEVICE_OBJECT	pTargDev );


static NTSTATUS CheckFAReadParams(
	PIRP	pIrp);

static NTSTATUS CheckMAReadParams(
	PIRP				pIrp,
	PIO_STACK_LOCATION	*ppStack);

static VOID CopyMultiWString(
	PWCHAR		pDest, 
	PWCHAR		pSrc, 
	SIZE_T		DestSize);

static VOID CopyWString(
	PWCHAR		pDest, 
	PWCHAR		pSrc, 
	SIZE_T		DestSize);

static NTSTATUS CreateDevice (
		IN PDRIVER_OBJECT	pDriverObject,
		PWCHAR				pBaseName,
		ULONG				DevExtSize,
		OUT PDEVICE_OBJECT*	ppDev,
		LONG				SeqNum,
		PWCHAR				pwszSymLink);

static NTSTATUS CreateMainDevice(
	PDRIVER_OBJECT		pDriverObject );

static PSTR DecodeEnum(
	INT			Value, 
	INT			Base,
	PSTR		pszDecodeV[],
	INT		VectLen,
	PSTR		pszOutBuf, 
	SIZE_T		cbBufSize);

static NTSTATUS DispatchFilteredRead(
	  IN PDEVICE_OBJECT	pDevObj,
	  IN PIRP			pIrp );

static NTSTATUS DispatchFilteredWrite(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp);

static VOID DpcCltIrpCompletion(
	 IN struct _KDPC		*Dpc,
	 IN PVOID				DeferredContext,
	 IN PVOID				SystemArgument1,
	 IN PVOID				SystemArgument2);

static NTSTATUS FADispatchIoctl(
	PDEVICE_OBJECT	pDevObj, 
	PIRP			pIrp);

static NTSTATUS FADispatchRead(
	PDEVICE_OBJECT	pDevObj, 
	PIRP			pIrp);

static NTSTATUS FAMainDispatch(
	PDEVICE_OBJECT		pDevObj,
	PIRP				pIrp);

static NTSTATUS FltMainDispatch(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp);

static NTSTATUS ForwardIrpOrPIrp(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp);

static NTSTATUS ForwardPIrpToNext(
	  IN PDEVICE_OBJECT	pDevObj,
	  IN PIRP			pIrp );

static NTSTATUS ForwardToNext(
	  IN PDEVICE_OBJECT	pDevObj,
	  IN PIRP			pIrp );

static NTSTATUS GenericTrace(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp);

static VOID GetReadData(
	PIRP				pReadIrp, 
	PDEVICE_EXTENSION	pDevExt, 
	PVOID				*ppReadData, 
	SIZE_T				*pcbDataLen);

static NTSTATUS GetRWIrpBuffer(
	PIRP			pIrp, 
	PDEVICE_OBJECT	pDevObj, 
	PVOID			*ppBuffer);

static NTSTATUS GetTargDevInfo(
	PFADEVICE_EXTENSION	pFaDevExt);

static VOID GetWriteData(
	PIRP				pIrp, 
	PDEVICE_EXTENSION	pDevExt, 
	PVOID*				ppWriteData, 
	SIZE_T*				pcbDataLen);

static NTSTATUS GTCompletion(
	PDEVICE_OBJECT		pDevObj,
	PIRP				pIrp,
	PVOID				Context);

static VOID InterceptIrpAndData(
	PIRP				pIrp, 
	PVOID				pData, 
	SIZE_T				cbDataLen, 
	PDEVICE_EXTENSION	pDevExt);

static NTSTATUS IoctlGetMJFFlags(
	PIRP				pIrp, 
	PDEVICE_EXTENSION	pFltDevExt);

static NTSTATUS IoctlSetMJFFlags(
	PIRP				pIrp, 
	PDEVICE_EXTENSION	pFltDevExt);

static NTSTATUS MainDispatch(
	PDEVICE_OBJECT		pDevObj,
	PIRP				pIrp);

static PSTR	MajorFunctionDes(
	 UCHAR		MajorFunction,
	 PSTR		pszDecodeBuf,
	 SIZE_T		cbBufSize);

static NTSTATUS MAMainDispatch(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp);

static PSTR MinorFunctionDes(
	UCHAR		MajorFunction,
	UCHAR		MinorFunction,
	PSTR		pszDecodeBuf,
	SIZE_T		cbBufSize);

static VOID MoveIrpDataToCltIrp(
	PDEVICE_EXTENSION	pDevExt, 
	PIRP				pCltIrp);

static NTSTATUS QueueCltIrp(
	PDEVICE_EXTENSION	pDevExt,
	PIRP	pCltIrp);

static VOID QueueCltIrpForCompletion(
	PDEVICE_EXTENSION	pDevExt,
	PIRP				pCltIrp);

static NTSTATUS ReadCompletion(
	PDEVICE_OBJECT		pDevObj,
	PIRP				pIrp,
	PVOID				pContext);


static VOID SendIrpDataToClt(
	PDEVICE_EXTENSION	pDevExt);

static NTSTATUS SendIrpSync(
	PIRP		pIrp,
	PKEVENT		PEvent);

static NTSTATUS SendIrpSyncCompletion(
	PDEVICE_OBJECT	pDevObj,
	PIRP			pIrp,
	PVOID			Context);

static VOID TraceIrpBasicInfo(
	PDEVICE_EXTENSION		pFltDevExt, 
	PSTR					pszCallingFunction,
	PIRP					pIrp);

//----------------------------------------------------------------------
//
// Functions
// ---------


//++
// Function:	DriverEntry
//
// Description:
//
//      Announces entry points.
//
//		Creates the master device, which is used to get to the 
//		access devices for the filters
//
//
// Arguments:
//		pDriverObject - Passed from I/O Manager
//		pRegistryPath - UNICODE_STRING pointer to
//						registry info (service key)
//						for this driver
//
// Return value:
//		NTSTATUS signaling success or failure
//--
extern "C" NTSTATUS DriverEntry (
			IN PDRIVER_OBJECT pDriverObject,
			IN PUNICODE_STRING pRegistryPath	) {
	INT					i;
	PDEVICE_EXTENSION	pDevExt;

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"\n\nFLT - DriverEntry - Test filter driver, compiled "
		"%s %s\n\n",__DATE__, __TIME__);

	// The filter will be loaded by the system and stay loaded,
	// so we don't have an unload routine.
	//
	pDriverObject->DriverUnload = NULL;
	pDriverObject->DriverExtension->AddDevice = AddDevice;

	// Announce entry points for all the function
	// codes, so that the filter can be transparent.
	//
	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION + 1; i++) {
		pDriverObject->MajorFunction[i] =
				MainDispatch;
	}

	// Create and initialize the main device.
	//
	NTSTATUS status = CreateMainDevice(pDriverObject);
	if (!NT_SUCCESS(status)) return status;

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - DriverEntry - Test filter driver succesfully "
		"loaded.\n");

	return STATUS_SUCCESS;
}


//++
// Function:	AddDevice
//
// Description:
//		Creates a filter device object and attaches it to the device
//		stack.
//
//		Creates filter access device, which is called by clients to
//		interact with the filter.
//
// Arguments:
//
//		pDriverObject - ptr to driver obj.
//
//		pTargDev - ptr to device being filtered.
//
// Return value:
//		None
//--
static NTSTATUS AddDevice(
		IN PDRIVER_OBJECT	pDriverObject,
		IN PDEVICE_OBJECT	pTargDev )
{
	PDEVICE_OBJECT		pFaDev = NULL;
	PVOID				pDataBuffer = NULL;
	PDEVICE_EXTENSION	pDevExt;
	PDEVICE_OBJECT	pFilterDev = NULL;
	PDEVICE_OBJECT	pTargStack = NULL;
	NTSTATUS		status;

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - AddDevice - Entering, pTargDev: %#p\n", pTargDev);

	// Acquire an unique device number to form the device names and
	// symbolic links
	//
	LONG NewDevNumber = InterlockedIncrement(&DevNumber);
	status = CreateDevice(
		pDriverObject,
		DRV_DEVICE_NAME,
		sizeof (DEVICE_EXTENSION),
		&pFilterDev,
		NewDevNumber,
		NULL);
	if (!NT_SUCCESS(status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - AddDevice - Failed to create filter dev with "
			"status: %#010x\n",
			status);
		return status;
	}

	// Initialize the Device Extension
	//
	pDevExt = (PDEVICE_EXTENSION) pFilterDev->DeviceExtension;
	pDevExt->hdr.DevType = DT_FILTER;
	pDevExt->hdr.pDevice = pFilterDev;	// back pointer
	pDevExt->dwFlags = 0;
	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION + 1; i++) 
		pDevExt->dwMJFlags[i] = MJ_FLAGS_DEFAULT;
	KeInitializeSpinLock(&pDevExt->DataLock);
	KeInitializeDpc(&pDevExt->ComplDpc, DpcCltIrpCompletion, pDevExt);
	InitializeListHead(&pDevExt->pIrpList);
	InitializeListHead(&pDevExt->pWaitingComplIrpList);
	ULONG Tag;
	PCHAR pTagChar = (PCHAR) &Tag;
	pTagChar[3] = 'F';
	pTagChar[2] = 'i';
	pTagChar[1] = 'l';
	pTagChar[0] = 't';
	pTargStack = IoAttachDeviceToDeviceStack(
		pFilterDev,
		pTargDev);
	if (pTargStack == NULL) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - AddDevice - Failed to attach filter to device %#p\n",
			pTargDev);
		status = STATUS_UNSUCCESSFUL;
		goto Error_Cleanup;
	}

	// VERY IMPORTANT!
	//
	// Propagate the flags from the top of the filtered stack, so that the 
	// I/O manager will treat our filter as if it were the previous top 
	// device.
	//
	pFilterDev->Flags = pTargStack->Flags;
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - AddDevice - Attached filter\n"
			 "    filter:        %#p\n"
	         "    target device: %#p\n"
			 "    target stack:  %#p\n",
		pFilterDev,
		pTargDev,
		pTargStack);
	pDevExt->pTargStack = pTargStack;

	// Allocate the filter buffer
	//
	pDataBuffer = ExAllocatePoolWithTag(NonPagedPool, FILT_BUF_SIZE, 
		Tag);
	if (pDataBuffer == NULL) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - AddDevice - Failed to allocate the data "
			"buffer\n");
		status = STATUS_NO_MEMORY;
		goto Error_Cleanup;
	}
	CBInitBufferCtlVar(&pDevExt->DataBuffer, pDataBuffer, FILT_BUF_SIZE);

	// Create the filter access device
	//
	PFADEVICE_EXTENSION pFaDevExt;

	// wszSymLink receives the sym link name. We need it to copy it
	// into the device extension.
	//
	WCHAR wszSymLink[NAME_BUF_SIZE - 3];
	status = CreateDevice(
		pDriverObject,
		FADEVICE_NAME,
		sizeof (FADEVICE_EXTENSION),
		&pFaDev,
		NewDevNumber,
		wszSymLink);
	if (!NT_SUCCESS(status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - AddDevice - Failed to create filter access dev with "
			"status: %#010x\n",
			status);
		goto Error_Cleanup;
	}
	pFaDevExt = (PFADEVICE_EXTENSION) pFaDev->DeviceExtension;

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - AddDevice - Filter access device created: %#p; dev "
		"ext: %#p\n",
		pFaDev, pFaDevExt);

	// Initialize the device extension.
	//
	RtlZeroMemory(pFaDevExt, sizeof (PFADEVICE_EXTENSION));
	pFaDevExt->hdr.DevType = DT_FILTER_ACCESS;
	pFaDevExt->hdr.pDevice = pFaDev;
	pFaDevExt->pFilterDevExt = pDevExt;
	CopyWString(
		(PWCHAR) &pFaDevExt->TargDevInfo.SymLink,
		(PWCHAR) wszSymLink,
		sizeof pFaDevExt->TargDevInfo.SymLink);

	// The filter access device uses direct I/O
	//
	pFaDev->Flags |= DO_DIRECT_IO;
	pFaDev->Flags &= ~DO_DEVICE_INITIALIZING;


	// Set the address of the extension for the filter access dev into
	// the extension for the filter dev.
	//
	pDevExt->pFADevExt = pFaDevExt;

	// Get the target device description, etc.
	//
	GetTargDevInfo(pFaDevExt);

	// Add the FA device to the FA device list. The list head is stored
	// into the device extension of the master device 
	//
	KLOCK_QUEUE_HANDLE hLock;
	KeAcquireInStackQueuedSpinLock(&pMastDevExt->SyncLock,
		&hLock);
	InsertTailList(
		&pMastDevExt->FADevsList, 
		&pFaDevExt->ListEntry);
	pMastDevExt->FADevsCount++;
	KeReleaseInStackQueuedSpinLock(&hLock);
	return STATUS_SUCCESS;

Error_Cleanup:
	if (pFaDev != NULL) IoDeleteDevice (pFaDev);
	if (pDataBuffer != NULL) ExFreePool(pDataBuffer);
	if (pTargStack != NULL) IoDetachDevice(pTargStack);
	if (pFilterDev != NULL) IoDeleteDevice (pFilterDev);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - AddDevice - failed; returning status = %#010x\n", 
		status);
	return status;
}


//++
// Function:	CheckFAReadParams
//
// Description:
//
//		Checks the parameters of a read IRP for the FA device.
//
// Arguments:
//
//		pIrp - Address of the IRP.
//
// Return value: success or failure code.
//--
static NTSTATUS CheckFAReadParams(
	PIRP	pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIoStack = IoGetCurrentIrpStackLocation(pIrp);
	SIZE_T bufSize = pIoStack->Parameters.Read.Length;

	// The buffer for a client read IRP must at least have room for
	// an intercepted IRP + I/O stack with their header.
	// It may also have room for any eventual IRP data, but this is
	// not manadatory.
	//
	if (bufSize < sizeof (IRP_HEADER) + sizeof(IRP) +
		sizeof(IO_STACK_LOCATION)) {
		status = STATUS_INVALID_PARAMETER;
		pIrp->IoStatus.Status = status;
		pIrp->IoStatus.Information = 0;
	}
	return status;
}


//++
// Function:	CheckMAReadParams
//
// Description:
//
//	Checks that the buffer for the read operation is long enough and
//	proterly aligned.
//
// Arguments:
//
//	pIrp - IRP for the read operation.
//
//	pDevObj - device object.
//
//	ppStack - will receive the current stack loc address
//
//	ppAdl - address of a ptr which will receive the address of the user
//		buffer.
//
// Return value: success or failure.
//--
static NTSTATUS CheckMAReadParams(
	PIRP					pIrp,
	PDEVICE_OBJECT			pDevObj,
	PIO_STACK_LOCATION		*ppStack,
	PACCESS_DEVICES_LIST	*ppAdl)
{
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	*ppStack = pStack;
	if (pStack->Parameters.Read.Length < 
		FIELD_OFFSET(ACCESS_DEVICES_LIST, TargDevVect)) {
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
				"FLT - CheckMAReadPArams - Client buffer too small; "
				"minimum required: %d, current: %d\n",
				FIELD_OFFSET(ACCESS_DEVICES_LIST, TargDevVect),
				pStack->Parameters.Read.Length);
		return STATUS_INVALID_PARAMETER;
	}

	PACCESS_DEVICES_LIST pAdl;
	NTSTATUS status = GetRWIrpBuffer(pIrp, pDevObj, (PVOID *) &pAdl);
	if (!NT_SUCCESS(status)) return status;
	*ppAdl = pAdl;

	// Check that the buffer is properly aligned for the structure we
	// will return
	//
	if ((ULONG_PTR) pAdl &
		(TYPE_ALIGNMENT(ACCESS_DEVICES_LIST) - 1)) {
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
				"FLT - CheckMAReadPArams - Client buffer not aligned; "
				"required alignment: %#x, buffer address: %#p\n",
				TYPE_ALIGNMENT(ACCESS_DEVICES_LIST),
				pAdl);
		return STATUS_INVALID_PARAMETER;
	}
	return STATUS_SUCCESS;
}


//++
// Function:	CopyMultiWString
//
// Description:
//
//	Copies a REG_MULTI_SZ string eventually truncating it so that it 
//	fits into the destination buffer.
//
//	The destination data always ends with the double NULL terminator,
//	even if the data is truncated.
//
// Arguments:
//
//	pDest - address of the destination buffer
//
//	pSrc - address of the source buffer
//
//	DestSize - size in byte of the destination buffer
//
//
// Return value: 
//--
static VOID CopyMultiWString(
	PWCHAR		pDest, 
	PWCHAR		pSrc, 
	SIZE_T		DestSize)
{
	// pLast points to the next to last WCHAR in the dest buffer.
	//
	PWCHAR pLast = pDest + DestSize / sizeof (WCHAR) -2;
	do {
		for (; *pSrc != L'\0'; pSrc++, pDest++) {
			if (pDest >= pLast) break;
			*pDest = *pSrc;
		}
		*pDest = L'\0';
		pDest++;
		if (pDest >= pLast) break;
		pSrc++;
	} while (*pSrc != L'\0');
	*(pDest) = L'\0';
	return;
}


//++
// Function:	CopyWString
//
// Description:
//
//	Copies a NULL terminated string of WCHAR, eventually truncating it 
//	so that it fits into the destination buffer.
//
//	It always terminates the destination string.
//
// Arguments:
//
//	pDest - address of the destination buffer
//
//	pSrc - address of the source buffer
//
//	DestSize - size in byte of the destination buffer
//
// Return value: 
//--
static VOID CopyWString(
	PWCHAR		pDest, 
	PWCHAR		pSrc, 
	SIZE_T		DestSize)
{
	// pLast points to the last WCHAR in the dest buffer.
	//
	PWCHAR pLast = pDest + DestSize / sizeof (WCHAR) -1;
	for (; *pSrc != L'\0'; pSrc++, pDest++) {
		if (pDest >= pLast) break;
		*pDest = *pSrc;
	}
	*pDest = L'\0';
	return;
}



//++
// Function:	CreateDevice
//
// Description:
//		Creates a new device along with its symbolic link.
//
//		Forms the device and link name and optionally suffixes them
//		with the DevNumber counter.
//
// Arguments:
//		pDriverObject - Passed from I/O Manager
//
//		pBaseName - Base name for the device, used to form the
//			internal name and the symbolic link name. 
//
//		DevExtSize - size of the device extension for the device.
//
//		ppDev - pointer to a pointer which will receive the
//			address of the device object returned by the
//			I/O manager.
//
//		SeqNum - the internal and symbolic link
//			names will be suffixed with the DevNumber counter.
//
//			If SeqNum = -1, the names will not be suffixed.
//
//		pwszSymLink - address of an array of WCHAR which receives
//			the symbolic link. It can be null, but if an address
//			is specified, the buffer MUST bet at 
//			least NAME_BUF_SIZE - 3 WCHAR wide.
//
// Return value:
//
//		Success or failure status. If an error status is returned,
//		*ppDev will be set to NULL on exit.
//
//--
static NTSTATUS CreateDevice (
		IN PDRIVER_OBJECT	pDriverObject,
		PWCHAR				pBaseName,
		ULONG				DevExtSize,
		OUT PDEVICE_OBJECT*	ppDev,
		LONG				SeqNum,
		PWCHAR				pwszSymLink)
{

	PDEVICE_OBJECT	pDevObj = NULL;
	NTSTATUS		status;

	*ppDev = NULL;

	// Form the device name
	//
	WCHAR			wszIntNameBuf[NAME_BUF_SIZE];
	UNICODE_STRING	uIntName;
	wszIntNameBuf[0] = L'\0';
	uIntName.Length = 0;
	uIntName.MaximumLength = sizeof wszIntNameBuf;
	uIntName.Buffer = wszIntNameBuf;
	if (SeqNum != -1) {
		status = RtlUnicodeStringPrintf(
			&uIntName,
			L"\\Device\\%s%d",
			pBaseName,
			SeqNum);
	} else {
		status = RtlUnicodeStringPrintf(
			&uIntName,
			L"\\Device\\%s",
			pBaseName);
	}
	if (!NT_SUCCESS(status)) return status;

	// Form the symbolic link name
	//
	WCHAR			wszSymNameBuf[NAME_BUF_SIZE];
	UNICODE_STRING	uSymName;
	wszSymNameBuf[0] = L'\0';
	uSymName.Length = 0;
	uSymName.MaximumLength = sizeof wszSymNameBuf;
	uSymName.Buffer = wszSymNameBuf;
	if (SeqNum != -1) {
		status = RtlUnicodeStringPrintf(
			&uSymName,
			L"\\??\\%s%d",
			pBaseName,
			DevNumber);
	} else {
		status = RtlUnicodeStringPrintf(
			&uSymName,
			L"\\??\\%s",
			pBaseName);
	}
	if (!NT_SUCCESS(status)) return status;

	// Copy the sym link name to the output buffer, if a buffer
	// has been specified
	//
	// pwszSymLink receives the sym link name. The UNICODE_STRING has a 
	// buffer of NAME_BUF_SIZE chars, however we are going to skip the
	// first 4 chars (\??\), so wszSymLink must hold NAME_BUF_SIZE - 4
	// chars. Furthermore, unicode strings do not have null 
	// terminators, but wszSymLink is a plain string, hence 
	// null-terminated. So wszSymLink must be NAME_BUF_SIZE - 3 chars.
	//
	if (pwszSymLink != NULL) {
		ULONG StrCharLen = uSymName.Length / sizeof (WCHAR) - 4;
		ULONG i = 0;
		for (; i < StrCharLen; i++) {
			pwszSymLink[i] = uSymName.Buffer[i + 4];
		}
		pwszSymLink[i] = L'\0';
	}
	// Create the device
	//
	status =
		IoCreateDevice( pDriverObject,
						DevExtSize,
						&uIntName,
						FILE_DEVICE_UNKNOWN,
						0, FALSE,
						&pDevObj );
	if (!NT_SUCCESS(status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - CreateDevice -  IoCreateDevice() failed with "
			"status %#010x\n",
			status);
		return status;
	}

	// Form the symbolic link name
	status = 
		IoCreateSymbolicLink( &uSymName,
							  &uIntName );
	if (!NT_SUCCESS(status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - CreateDevice - IoCreateSymbolicLink() failed "
			"with status %#010x\n",
			status);
		goto Error_Cleanup;
	}

	// Pass the device object ptr to the caller.
	//
	*ppDev = pDevObj;
	return STATUS_SUCCESS;

Error_Cleanup:
	if (pDevObj != NULL) IoDeleteDevice(pDevObj);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - CreateDevice - Failed. Returning status %#010x\n", 
		status);
	return status;
}


//++
// Function:	CreateMainDevice
//
// Description:
//
//	Creates the (MDO) master device object, which is used to give to 
//	clients access to the filter devices.
//
//	The MDO extension stores the head of a linked list of all the
//	FADs (filter access devices).
//
//	The MDO's dispatch routine allow a client to extract data from the
//	FADs.
//
//	There is a single MDO for the whole driver and its dev ext address
//	is stored into the static variable pMastDevExt.
//
//	This function creates the MDO, initializes its dev ext and stores
//	the dev ext address into the static variable.
//
// Arguments:
//
//		pDriverObject - Address of the driver object for this driver.
//
// Return value: 
//--
static NTSTATUS CreateMainDevice(
	PDRIVER_OBJECT		pDriverObject )
{
	NTSTATUS status;

	PDEVICE_OBJECT pMastDevObj;
	status = CreateDevice(
		pDriverObject,
		MASTER_DEVICE_NAME,
		sizeof (MADEVICE_EXTENSION),
		&pMastDevObj,
		-1,
		NULL);
	if (!NT_SUCCESS(status)) {
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - Failed to create the master device.\n"); 
		return status;
	}
	pMastDevExt = (PMADEVICE_EXTENSION) pMastDevObj->DeviceExtension;
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - Master device created. Device object: %#p, device "
		"extension: %#x\n",
		pMastDevObj,
		pMastDevExt);
	pMastDevObj->Flags |= DO_DIRECT_IO;
	pMastDevExt->hdr.pDevice = pMastDevObj;
	pMastDevExt->hdr.DevType = DT_MASTER;
	KeInitializeSpinLock(
		&pMastDevExt->SyncLock);
	InitializeListHead(&pMastDevExt->FADevsList);
	pMastDevExt->FADevsCount = 0;

	return STATUS_SUCCESS;
		
}


//++
// Function:	DecodeEnum
//
// Description:
//
//	Decodes a numerical value for which a table of descriptive strings 
//	exists. Returns the string corresponding to the value.
//
//	If the value is outside the table boundaries, returns a string
//	representation of the value in hexadecimal form.
//
// Arguments:
//
//	Value - value to decode.
//
//	Base - base of Value (0, 1, ...)
//
//	pszDecodeV - address of the string table.
//
//	VectLen - # of elements inside the string table
//
//	pszOutBuf - address of the output buffer.
//
//	cbBufSize - output buffer length in bytes.
//
// Return value: address of pszOutBuf.
//--
static PSTR DecodeEnum(
	INT			Value, 
	INT			Base,
	PSTR		pszDecodeV[],
	INT		VectLen,
	PSTR		pszOutBuf, 
	SIZE_T		cbBufSize)
{
	INT Index = Value - Base;
	if ((Index >= 0) && ((INT) Index < VectLen)) {
		RtlStringCbCopyA(pszOutBuf, cbBufSize, pszDecodeV[Index]);
	} else {
		RtlStringCbPrintfA(pszOutBuf, cbBufSize, "%#x", Value);
	}

	return pszOutBuf;
}



//++
// Function:	FADispatchRead
//
// Description:
//
//		Handles a read IRP targeted at the filter access device. Returns
//		to the client the data of an intercepted IRP.
//
// Arguments:
//
//		pDevObj - device object.
//
//		pIrp - IRP.
//
// Return value: success or failure code.
//--
static NTSTATUS FADispatchRead(
	PDEVICE_OBJECT	pDevObj, 
	PIRP			pIrp)
{
	NTSTATUS status;
	status = CheckFAReadParams(pIrp);

	if (!NT_SUCCESS(status)) return status;

	PFADEVICE_EXTENSION pFaExt = 
		(PFADEVICE_EXTENSION) pDevObj->DeviceExtension;

	// Get the extension of the filter device
	//
	PDEVICE_EXTENSION pFltExt = pFaExt->pFilterDevExt;
	KLOCK_QUEUE_HANDLE	LockHandle;
	KeAcquireInStackQueuedSpinLock(
		&pFltExt->DataLock,
		&LockHandle);

	// Queue the client IRP.
	//
	status = QueueCltIrp(pFltExt, pIrp);
	if (!NT_SUCCESS(status)) {
		KeReleaseInStackQueuedSpinLock(&LockHandle);
		return status;
	}

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - FADispatchRead - Client IRP %#p queued for "
		"processing\n", pIrp);


	// Schedule the processing of the queued IRP (if there are data)
	//
	SendIrpDataToClt(pFltExt);

	KeReleaseInStackQueuedSpinLock(&LockHandle);

	return status;

}


//++
// Function:	DispatchFilteredRead
//
// Description:
//
//		Dispatches a read IRP targeted at the filtered device.
//
//		Attaches an I/O completion routine to the IRP and forwards it to 
//		the filtered device.
//
// Arguments:
//		pDevObj - Passed from I/O Manager
//		pIrp - Passed from I/O Manager
//
// Return value:
//		NTSTATUS - success or failure code
//--
static NTSTATUS DispatchFilteredRead(
	  IN PDEVICE_OBJECT	pDevObj,
	  IN PIRP			pIrp )
{
	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION) pDevObj->DeviceExtension;
	if (pDevExt->dwMJFlags[IRP_MJ_READ] & FCTL_TRACE_ON) {
		PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
		TraceIrpBasicInfo(pDevExt, "DispatchFilteredRead", pIrp);
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"\toffset = %#I64x\n"
			"\tlength = %#x\n",
			pIrpStack->Parameters.Read.ByteOffset.QuadPart,
			pIrpStack->Parameters.Read.Length
		);
	}

	IoCopyCurrentIrpStackLocationToNext(pIrp);

	// Set the completion routine which will intercept the read data.
	//
	IoSetCompletionRoutine(pIrp, ReadCompletion, NULL, TRUE, TRUE, TRUE);
	return IoCallDriver(
		pDevExt->pTargStack,
		pIrp );
}


//++
// Function:	DispatchFilteredWrite
//
// Description:
//
//		Traces a IRP_MJ_WRITE IRP along with its data (if tracing is
//		enabled) and forwards it to the filtered device.
//
//		When tracing, the IRP, the IO_STACK_LOCATION and the data to be 
//		written are stored into the internal buffer. 
//
//		Since the IRP is stored into the buffer BEFORE being processed
//		by the filtered device, the Status value ie meaningless.
//
// Arguments:
//
//		pDevObj - passed by the I/O manager.
//
//		pIrp - passed by the I/O manager.
//
// Return value: 
//--
static NTSTATUS DispatchFilteredWrite(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp)
{
	PDEVICE_EXTENSION pDevExt = 
		(PDEVICE_EXTENSION) pDevObj->DeviceExtension;
	if (pDevExt->dwMJFlags[IRP_MJ_WRITE] & FCTL_TRACE_ON) {
		PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
		TraceIrpBasicInfo(pDevExt, "DispatchFilteredWrite", pIrp);
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"\tdata length = %#x\n", 
			pStack->Parameters.Write.Length);

		// Get the IRP data, if any.
		//
		PVOID pWriteData;
		SIZE_T cbDataLen;
		GetWriteData(pIrp, pDevExt, &pWriteData, &cbDataLen);
		KLOCK_QUEUE_HANDLE LockHandle;
		KeAcquireInStackQueuedSpinLock(
			&pDevExt->DataLock,
			&LockHandle);
		InterceptIrpAndData(pIrp, pWriteData, cbDataLen, pDevExt);
		SendIrpDataToClt(pDevExt);
		KeReleaseInStackQueuedSpinLock(&LockHandle);

	}
	IoSkipCurrentIrpStackLocation(pIrp);
	return IoCallDriver(pDevExt->pTargStack, pIrp);



}


//++
// Function:	DpcCltIrpCompletion
//
// Description:
//
//		DPC routine called to complete client IRPs.
//
//		Extracts all the client IRPs waiting in the internal queue and
//		completes them.
//
// Arguments:
//
//
// Return value: 
//--
static VOID DpcCltIrpCompletion(
	 IN struct _KDPC		*Dpc,
	 IN PVOID				DeferredContext,
	 IN PVOID				SystemArgument1,
	 IN PVOID				SystemArgument2)
{
	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION) DeferredContext;
	PLIST_ENTRY pWCompl = &pDevExt->pWaitingComplIrpList;
	KLOCK_QUEUE_HANDLE LockHandle;
	for (;;) {
		KeAcquireInStackQueuedSpinLock(&pDevExt->DataLock, &LockHandle);
		if (IsListEmpty(pWCompl)) {
			KeReleaseInStackQueuedSpinLock(&LockHandle);
			break;
		}
		PLIST_ENTRY pItem = RemoveHeadList(pWCompl);
		KeReleaseInStackQueuedSpinLock(&LockHandle);
		PIRP pIrp = 
			CONTAINING_RECORD(pItem, IRP, Tail.Overlay.ListEntry);

		IoSetCancelRoutine(pIrp, NULL);
		IoCompleteRequest(pIrp, IO_NO_INCREMENT);

		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - DpcCltIrpCompletion - Client Irp %#p "
			"completed\n", pIrp);
	}
}


//++
// Function:	FltMainDispatch
//
// Description:
//
//		Main dispatch routine for the filter device.
//
// Arguments:
//
//		Forward IRPs handled specifically to the corresponding routine.
//
//		Forwards all other IRPs to the common tracking routine which
//		does minimal tracking for any IRP for which tracking is
//		enabled, regardless of the major code.
//
//		The minimal tracking consists of attaching a completion routine
//		which will store in the internal buffer the IRP, and the
//		current I/O stack location. Given that the IRP is copied at
//		completion time, it holds the NTSTATUS it has been completed
//		with.
//
//		Tracking can be enabled or disabled for any major code: a table
//		stores the setting for each possible major code.
//
//		For major codes handled in specific ways extra data is stored
//		into the buffer, e. g. output data from a read IRP.
//		
//		Regardless of how an IRP is tracked, it is forwarded to the
//		underlying device.
//
// Return value: 
//--
static NTSTATUS FltMainDispatch(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp)
{
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	UCHAR MajorFunction = pStack->MajorFunction;
	switch (MajorFunction) {
		case IRP_MJ_READ:
			return DispatchFilteredRead(pDevObj, pIrp);
			break;
		case IRP_MJ_WRITE:
			return DispatchFilteredWrite(pDevObj, pIrp);
			break;
		default:

			// Common tracing and forwarding.
			//
			return GenericTrace(pDevObj, pIrp);
	}
}


//++
// Function:	FACancel
//
// Description:
//
//		Handles cancellation of client IRPs.
//
//		The structure of this function is taken from:
//
//		The Truth About Cancel - IRP Cancel Operations (Part I) 
//		The NT Insider, Vol 4, Issue 6, Nov-Dec 1997
//
//		available at www.osronline.com.
//
//		The key point about this function is that it does not complete
//		the IRP received in pIrp, but, rather, the first IRP it
//		founds in the internal queue which is flagged for cancellation.
//
//		This allow other code to complete an IRP 
//		without knowing wether the IRP is in the process of being
//		cancelled and this function will be called for it.
//
//		As long as this other code removes the IRP from the
//		queue (under the protection of the spinlock) before completing
//		it, it can be sure that this function will not complete it
//		again, because it will not find it in the queue anymore.
//
//		Thus, there's no need to know wether this function is going to
//		execute or not.
//
//		On the other hand, if this function finds an IRP to be
//		cancelled, removes it from the queue and then completes it.
//
//		Again, the IRP is dequeued under the spinlock protection, so
//		no other code path will be able to touch it.
//
// Arguments:
//
//
// Return value: 
//--
static VOID FACancel(
	 PDEVICE_OBJECT		pDevObj,
	 PIRP				pIrp)
{
	IoReleaseCancelSpinLock(pIrp->CancelIrql);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - FACancel - Entering, IRP %#p\n", pIrp);
	PFADEVICE_EXTENSION pFaDevExt = 
		(PFADEVICE_EXTENSION) pDevObj->DeviceExtension;

	// Get the filter device extension.
	//
	PDEVICE_EXTENSION pFltDevExt = pFaDevExt->pFilterDevExt;

	// Look for an IRP to cancel into the internal queue.
	//
	KLOCK_QUEUE_HANDLE LockHandle;
	KeAcquireInStackQueuedSpinLock(&pFltDevExt->DataLock, &LockHandle);
	PLIST_ENTRY pCurrItem;
	PIRP pCurIrp = NULL;
	for (
		pCurrItem = pFltDevExt->pIrpList.Flink; 
		pCurrItem != &pFltDevExt->pIrpList;
		pCurrItem = pCurrItem->Flink) {

		pCurIrp = CONTAINING_RECORD(pCurrItem, IRP,
			Tail.Overlay.ListEntry);
		if (pCurIrp->Cancel) {
			RemoveEntryList(pCurrItem);
			break;
		}
		pCurIrp = NULL;
	}
	KeReleaseInStackQueuedSpinLock(&LockHandle);
	if (pCurIrp != NULL) {
		pCurIrp->IoStatus.Status = STATUS_CANCELLED;
		pCurIrp->IoStatus.Information = 0;
		IoCompleteRequest(pCurIrp, IO_NO_INCREMENT);
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - FACancel - Client IRP %#p cancelled\n", 
			pCurIrp);
	}
	return;
}


//++
// Function:	FADispatchIoctl
//
// Description:
//
//		Handles IRP_MJ_DEVICE_CONTROL IRP for the FA device.
//
// Arguments:
//
//		pDevObj - address of the FA device object
//
//		pIrp - address of the IRP
//
// Return value: success or failure code.
//--
static NTSTATUS FADispatchIoctl(
	PDEVICE_OBJECT	pDevObj, 
	PIRP			pIrp)
{
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	ULONG ctlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - FADispatchDioc - IOCTL code = %#010x\n", ctlCode);

	pIrp->IoStatus.Information = 0;	// bytes xfered

	// pFaDevExt points to the dev ext for the FA device. pFltDevExt
	// points to the one for the filter device.
	//
	PFADEVICE_EXTENSION pFaDevExt = 
		(PFADEVICE_EXTENSION) pDevObj->DeviceExtension;
	PDEVICE_EXTENSION pFltDevExt = 
		(PDEVICE_EXTENSION) pFaDevExt->pFilterDevExt;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	//PMJF_CONTROL pOutFlags = NULL;
	//PMJF_CONTROL pNewFlags = NULL;
	switch (ctlCode) {
	    case IOCTL_FILTER_GET_MJF_FLAGS:
			status = IoctlGetMJFFlags(pIrp, pFltDevExt);
			break;
	    case IOCTL_FILTER_SET_MJF_FLAGS:
			status = IoctlSetMJFFlags(pIrp, pFltDevExt);
			break;
		default:
			status = STATUS_INVALID_DEVICE_REQUEST;
			pIrp->IoStatus.Status = status;
	}
	return status;
}


//++
// Function:	FAMainDispatch
//
// Description:
//
//		This is the main entry point for the FA device dispatch 
//		routines.
//
//		It processes IRPs targeted at the FA device.
//
// Arguments:
//
//		pDevObj - Address of the FA device object
//
//		pIrp - Address of the IRP.
//
// Return value: 
//--
static NTSTATUS FAMainDispatch(
	PDEVICE_OBJECT		pDevObj,
	PIRP				pIrp)
{
	NTSTATUS status;
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	switch (pStack->MajorFunction) {
		case IRP_MJ_CLEANUP:
			status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0;
			pIrp->IoStatus.Status = status;
			break;
		case IRP_MJ_CLOSE:
			status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0;
			pIrp->IoStatus.Status = status;
			break;
		case IRP_MJ_CREATE:
			status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0;
			pIrp->IoStatus.Status = status;
			break;
		case IRP_MJ_DEVICE_CONTROL:
			status = FADispatchIoctl(pDevObj, pIrp);
			break;
		case IRP_MJ_READ:
			status = FADispatchRead(pDevObj, pIrp);
			break;
		default:
			status = STATUS_NOT_SUPPORTED;
			pIrp->IoStatus.Information = 0;
			pIrp->IoStatus.Status = status;
	}
	if (status != STATUS_PENDING) {

		// This should not be necessary. Code paths which set a cancel
		// routine either return STATUS_PENDING or remove the cancel 
		// routine by themselves. We do it in case we forgot something.
		//
		IoSetCancelRoutine(pIrp, NULL);
		IoCompleteRequest(
			pIrp, IO_NO_INCREMENT);
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - FAMainDispatch - Client IRP %#p completed with "
			"status %#010x\n", 
			pIrp,
			pIrp->IoStatus.Status);
	}
	return status;
}


//++
// Function:	ForwardIrpOrPIrp
//
// Description:
//
//		Forwards an IRP to the filtered stack, handling the different
//		behaviour required for power IRPs.
//
//		This function can be called only when no completion routine has
//		been set for the IRP (calls IoSkipCurrentIrpStackLocation).
//
// Arguments:
//
//
// Return value: NTSTATUS returned by the filtered stack.
//--
static NTSTATUS ForwardIrpOrPIrp(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp)
{
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	UCHAR MajFun = pStack->MajorFunction;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	if (MajFun == IRP_MJ_POWER) {

		// Power IRP must be forwarded differently.
		//
		status = ForwardPIrpToNext(pDevObj, pIrp);
	} else {
		status = ForwardToNext(pDevObj, pIrp);
	}
	return status;
}


//++
// Function:	ForwardPIrpToNext
//
// Description:
//		Forwards a power IRP to the next device in the stack.
//
//		Specialized for power IRPs.
//
// Arguments:
//		pDevObj - Passed from I/O Manager
//		pIrp - Passed from I/O Manager
//
// Return value:
//		the return value from the lower driver.	
//--
static NTSTATUS ForwardPIrpToNext(
	  IN PDEVICE_OBJECT	pDevObj,
	  IN PIRP			pIrp )
{
	PDEVICE_EXTENSION	pDevExt;

	pDevExt = (PDEVICE_EXTENSION) pDevObj->DeviceExtension;
	PoStartNextPowerIrp(pIrp);
	IoSkipCurrentIrpStackLocation(pIrp);
	return PoCallDriver(
		pDevExt->pTargStack,
		pIrp );
}


//++
// Function:	ForwardToNext
//
// Description:
//
//	Forwards an IRP targeted at the filter device to the filtered stack.
//
// Arguments:
//		pDevObj - Passed from I/O Manager
//		pIrp - Passed from I/O Manager
//
// Return value:
//		the return value from the lower driver.	
//--
static NTSTATUS ForwardToNext(
	  IN PDEVICE_OBJECT	pDevObj,
	  IN PIRP			pIrp )
{
	PDEVICE_EXTENSION	pDevExt;

	pDevExt = (PDEVICE_EXTENSION) pDevObj->DeviceExtension;

	IoSkipCurrentIrpStackLocation(pIrp);
	return IoCallDriver(
		pDevExt->pTargStack,
		pIrp );
}


//++
// Function:	GenericTrace
//
// Description:
//
//		Performs the minimal tracing for an IRP, if enabled, then
//		forwards it to the filtered stack.
//
// Arguments:
//
//		pDevObj - address of the filter device object.
//
//		pIrp - address of the IRP.
//
// Return value: NTSTATUS value returned by the filtered stack.
//--
static NTSTATUS GenericTrace(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp)
{
	PDEVICE_EXTENSION pDevExt = 
		(PDEVICE_EXTENSION) pDevObj->DeviceExtension;
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	UCHAR MajFun = pStack->MajorFunction;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	if (MajFun <= IRP_MJ_MAXIMUM_FUNCTION) { //One never knows...
		if ((pDevExt->dwMJFlags[MajFun]) & FCTL_TRACE_ON) {
			TraceIrpBasicInfo(pDevExt, "GenericTrace", pIrp);

			// Set the completion
			// routine, which will track the IRP.
			//
			IoCopyCurrentIrpStackLocationToNext(pIrp);
			IoSetCompletionRoutine(pIrp, GTCompletion, pDevExt, TRUE, 
				TRUE, TRUE);
			if (MajFun == IRP_MJ_POWER) {
				status = PoCallDriver(pDevExt->pTargStack, pIrp);
			} else {
				status = IoCallDriver(pDevExt->pTargStack, pIrp);
			}
		} else {

			// Tracing is off. Just forward the IRP.
			//
			status = ForwardIrpOrPIrp(pDevObj, pIrp);
		}
	} else {
		status = ForwardIrpOrPIrp(pDevObj, pIrp);

	}
	return status;
}


//++
// Function:	GetReadData
//
// Description:
//
//		Returns the data of a read IRP.
//
//		Looks into the filter device object to determine wether the
//		filtered device does direct or buffered I/O and obtains a 
//		pointer to the data block either from the IRP or from the MDL.
//
//		It relies on the fact that the buffering mode on the filter is
//		set to be equal to the one of the filtered device.
//
// Arguments:
//
//		pReadIrp - IRP address.
//
//		pDevExt - filter device extension address.
//
//		ppReadData - address of a ptr which will receive the data 
//			address.
//
//			Before using the data address, a caller must check the
//			data length: if it is 0, the data address is not valid.
//
//		pcbDataLen - address of a variable which will receive the data
//			length.
//
// Return value: none
//
//--
static VOID GetReadData(
	PIRP				pReadIrp, 
	PDEVICE_EXTENSION	pDevExt, 
	PVOID				*ppReadData, 
	SIZE_T				*pcbDataLen)
{
	*ppReadData = NULL;
	*pcbDataLen = 0;

	if (!NT_SUCCESS(pReadIrp->IoStatus.Status)) return;
	*pcbDataLen = pReadIrp->IoStatus.Information;
	if (!*pcbDataLen) return;

	// Determine the buffering mode
	//
	if (pDevExt->hdr.pDevice->Flags & DO_DIRECT_IO) {

		PMDL pMdl = pReadIrp->MdlAddress;
		PCHAR pBufStart = (PCHAR) MmGetSystemAddressForMdlSafe(pMdl, 
				LowPagePriority);
		if (pBufStart != NULL) {
			*ppReadData = pBufStart;
		} else {

			// This should never happen, because the client buffer
			// has already been locked in place by who issued the
			// read IRP.
			//
			*pcbDataLen = 0;
		}
	} else {

		// Buffered I/O
		//
		*ppReadData = pReadIrp->AssociatedIrp.SystemBuffer;
	}

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - GetReadData - Read data address: %#p, length: %#x\n",
		*ppReadData, *pcbDataLen);

	return;
}


//++
// Function:	GetRWIrpBuffer
//
// Description:
//
//	Returns the address of a data buffer for a read or write IRP.
//
// Arguments:
//
//	pIrp - IRP.
//
//	pDevObj - device object targeted by the IRP. The device must use
//		either buffered I/O or direct I/O (but not "neither" I/O).
//
//	ppBuffer - address of a ptr which will receive the address of the
//		data buffer.
//
// Return value: success or failure.
//
//		STATUS_INVALID_PARAMETER - if the device use "neither" I/O.
//--
static NTSTATUS GetRWIrpBuffer(
	PIRP			pIrp, 
	PDEVICE_OBJECT	pDevObj, 
	PVOID			*ppBuffer)
{
	if (pDevObj->Flags & DO_BUFFERED_IO) {
		*ppBuffer = pIrp->AssociatedIrp.SystemBuffer;
		return STATUS_SUCCESS;
	} else if (pDevObj->Flags & DO_DIRECT_IO) {
		PMDL pMdl = pIrp->MdlAddress;
		*ppBuffer = 
			MmGetSystemAddressForMdlSafe(pMdl, LowPagePriority);
		if (*ppBuffer == NULL) {
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
				"FLT - GetRWIrpBuffer - "
				"MmGetSystemAddressForMdlSafe() returned NULL\n");
			return STATUS_NO_MEMORY;
		}
		return STATUS_SUCCESS;
	} else {
		return STATUS_INVALID_PARAMETER;
	}
}


//++
// Function:	GetTargDevInfo
//
// Description:
//
//	Queries the target stack for device data, such as the device text,
//	etc.
//
//	Stores the retrieved information into the device extension of the
//	filter access device.
//
// Arguments:
//
//	pFaDevExt - pointer to the FA device extension
//
// Return value: success or failure code.
//--
static NTSTATUS GetTargDevInfo(
	PFADEVICE_EXTENSION	pFaDevExt)
{
	// We will send PNP IRPs to the target stack to query its
	// information.

	NTSTATUS status = STATUS_SUCCESS;
	KEVENT IrpComplEv;

	// Initialize the event used to wait for IRPs to complete
	//
	KeInitializeEvent(&IrpComplEv, NotificationEvent, FALSE);


	// Get the number of stack locations needed.
	//
	PDEVICE_EXTENSION pFltDevExt = pFaDevExt->pFilterDevExt;
	PDEVICE_OBJECT pTargStack = pFltDevExt->pTargStack;
	CCHAR StackSize = pTargStack->StackSize;

	// Note on the stack size.
	//
	// pTargStack->StackSize gives the # of stack locations needed by
	// the device below our filter.
	//
	// We will allocate an IRP with this number of stack locations,
	// so it will not have a stack location for the filter device.
	//
	// We don't need it, because we only need to pass the IRP down
	// the stack, with a completion routine attached, to get the
	// outcome. IoSetCompletionRoutine() actually sets the routine 
	// address into the stack location below the current one, so
	// this call does not need a stack location for the current device.
	//
	// By examining IoGetCurrentIrpStackLocation(),
	// IoGetNextIrpStackLocation() and IoSetNextIrpStackLocation()
	// in wdm.h, we find out that the stack location for the topmost
	// device is the one at the highest address.
	// IoGetCurrentIrpStackLocation() returns the value of a pointer
	// stored inside the IRP. Calling IoGetNextIrpStackLocation() 
	// returns the address of the current SL pointer minus the size of 
	// an SL, i. e., the SL for the device below us is at the next lower 
	// address.
	//
	// We also find that IoSetNextIrpStackLocation() decrements the
	// current SL pointer.
	//
	// We can put these facts together with how we use an IRP we 
	// are setting up: after the allocation, we can immediately 
	// call IoGetNextIrpStackLocation() to grab tha SL for the device
	// below us and set the relevant fields. This means the current SL
	// pointer has the address of the SL for the next device + the
	// size of an SL, because IoGetNextIrpStackLocation() just returns
	// current ptr minus SL size.
	//
	// Yet, since the SL for the device below us is the one at the 
	// highest address (when we don't allocate an extra SL for 
	// ourselves), this means the current SL pointer is actually 
	// pointing outside our IRP at whatever lays after it.
	//
	// This is confirmed by the DDK, which states:
	//
	// "If a driver allocates an I/O stack location of its own in a new 
	// IRP, the dispatch routine must call IoSetNextIrpStackLocation 
	// before it calls IoGetCurrentIrpStackLocation to set up context in
	// its own I/O stack location...".
	//
	// IoSetNextIrpStackLocation() just decrements the current SL 
	// pointer by the size of an SL and IoGetCurrentIrpStackLocation()
	// just returns the current pointer value. So, BEFORE using the
	// current pointer value, we must decrement it, because it's
	// pointing outside the IRP.
	//
	// In other words, a freshly initialized IRP has its current SL
	// pointer set to an address outside the IRP. If we know we have
	// reserved one extra SL for us, we can decrement it, with
	// IoSet..., otherwise we are forbidden to touch the current
	// SL (which actually does not exist) and must restrict ourselves
	// to using the next one, returned by IoGetNext... which returns
	// current ptr minus size.
	//
	// Probably, arraging SLs this way helps the system in detecting 
	// when some code is trying to use an IRP with too few SLs for a 
	// given device tree. As the IRP travels down the stack, the current
	// SL ptr is decremented, so the system can detect if it bumps 
	// against the beginning of the SLs array inside the IRP. If the 
	// system were to increment the SL ptr, it should have kept track of 
	// the SLs array size to perform the same check.

	// Allocate the IRP
	//
	PIRP pQueryIrp = IoAllocateIrp(StackSize, FALSE);
	if (pQueryIrp == NULL) return STATUS_NO_MEMORY;

	// Set the IRP up
	//
	pQueryIrp->Flags = 0;
	pQueryIrp->RequestorMode = KernelMode;
	pQueryIrp->Tail.Overlay.Thread = 0;
	pQueryIrp->IoStatus.Status = STATUS_NOT_SUPPORTED;

	// Set the I/O stack for the IRP_MN_QUERY_DEVICE_TEXT call.
	//
	PIO_STACK_LOCATION pStack = IoGetNextIrpStackLocation(pQueryIrp);
	pStack->MajorFunction = IRP_MJ_PNP;
	pStack->MinorFunction = IRP_MN_QUERY_DEVICE_TEXT;
	pStack->Flags = 0;
	pStack->DeviceObject = pTargStack;

	// Send the IRP. We'll wait for it to complete.
	//
	NTSTATUS IrpStatus = SendIrpSync(pQueryIrp, &IrpComplEv);
	if (NT_SUCCESS(IrpStatus)) {

		// Get the device text
		//
		if (pQueryIrp->IoStatus.Information) {
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
				"FLT - GetTargDevInfo - Filter: %#x, FA: %#x, "
				"device text: %S\n",
				pFaDevExt->pFilterDevExt->hdr.pDevice,
				pFaDevExt->hdr.pDevice,
				(PWCHAR) pQueryIrp->IoStatus.Information);
			CopyWString(
				(PWCHAR) &pFaDevExt->TargDevInfo.DeviceText, 
				(PWCHAR) pQueryIrp->IoStatus.Information,
				sizeof pFaDevExt->TargDevInfo.DeviceText);

			// We must free the storage for the string allocated by the
			// driver which processed our IRP, just as the I/O manager
			// would do.
			ExFreePool((PVOID) pQueryIrp->IoStatus.Information);

		}
	}

	// The status variable stores the overall status. Update it only
	// if it does not already hold an error value.
	//
	if (NT_SUCCESS(status)) status = IrpStatus;

	// Set the IRP up for th IRP_MQ_QUERY_ID/BusQueryDeviceId
	//
	IoReuseIrp(pQueryIrp, STATUS_NOT_SUPPORTED);
	// IoSetNextIrpStackLocation(pQueryIrp);
	pQueryIrp->Flags = 0;
	pQueryIrp->RequestorMode = KernelMode;
	pQueryIrp->Tail.Overlay.Thread = 0;
	pStack = IoGetNextIrpStackLocation(pQueryIrp);
	pStack->MajorFunction = IRP_MJ_PNP;
	pStack->MinorFunction = IRP_MN_QUERY_ID;
	pStack->Parameters.QueryId.IdType = BusQueryDeviceID;
	pStack->Flags = 0;
	pStack->DeviceObject = pTargStack;
	IrpStatus = SendIrpSync(pQueryIrp, &IrpComplEv);
	if (NT_SUCCESS(IrpStatus)) {
		if (pQueryIrp->IoStatus.Information) {
			CopyWString(
				(PWCHAR) &pFaDevExt->TargDevInfo.DeviceId, 
				(PWCHAR) pQueryIrp->IoStatus.Information,
				sizeof pFaDevExt->TargDevInfo.DeviceId);
			ExFreePool((PVOID) pQueryIrp->IoStatus.Information);
		}
	}
	if (NT_SUCCESS(status)) status = IrpStatus;

	// Set the IRP up for th IRP_MQ_QUERY_ID/BusQueryInstanceID
	//
	IoReuseIrp(pQueryIrp, STATUS_NOT_SUPPORTED);
	pQueryIrp->Flags = 0;
	pQueryIrp->RequestorMode = KernelMode;
	pQueryIrp->Tail.Overlay.Thread = 0;
	pStack = IoGetNextIrpStackLocation(pQueryIrp);
	pStack->MajorFunction = IRP_MJ_PNP;
	pStack->MinorFunction = IRP_MN_QUERY_ID;
	pStack->Parameters.QueryId.IdType = BusQueryInstanceID;
	pStack->Flags = 0;
	pStack->DeviceObject = pTargStack;
	IrpStatus = SendIrpSync(pQueryIrp, &IrpComplEv);
	if (NT_SUCCESS(IrpStatus)) {
		if (pQueryIrp->IoStatus.Information) {
			CopyWString(
				(PWCHAR) &pFaDevExt->TargDevInfo.InstanceId, 
				(PWCHAR) pQueryIrp->IoStatus.Information,
				sizeof pFaDevExt->TargDevInfo.InstanceId);
			ExFreePool((PVOID) pQueryIrp->IoStatus.Information);
		}
	}
	if (NT_SUCCESS(status)) status = IrpStatus;

	// Set the IRP up for th IRP_MQ_QUERY_ID/BusQueryHardwareIDs
	//
	IoReuseIrp(pQueryIrp, STATUS_NOT_SUPPORTED);
	// IoSetNextIrpStackLocation(pQueryIrp);
	pQueryIrp->Flags = 0;
	pQueryIrp->RequestorMode = KernelMode;
	pQueryIrp->Tail.Overlay.Thread = 0;
	pStack = IoGetNextIrpStackLocation(pQueryIrp);
	pStack->MajorFunction = IRP_MJ_PNP;
	pStack->MinorFunction = IRP_MN_QUERY_ID;
	pStack->Parameters.QueryId.IdType = BusQueryHardwareIDs;
	pStack->Flags = 0;
	pStack->DeviceObject = pTargStack;
	IrpStatus = SendIrpSync(pQueryIrp, &IrpComplEv);
	if (NT_SUCCESS(IrpStatus)) {
		if (pQueryIrp->IoStatus.Information) {
			CopyMultiWString(
				(PWCHAR) &pFaDevExt->TargDevInfo.HardwareIDs, 
				(PWCHAR) pQueryIrp->IoStatus.Information,
				sizeof pFaDevExt->TargDevInfo.HardwareIDs);
			ExFreePool((PVOID) pQueryIrp->IoStatus.Information);
		}
	}
	if (NT_SUCCESS(status)) status = IrpStatus;

	// Set the IRP up for th IRP_MQ_QUERY_ID/BusQueryCompatibleIDs
	//
	IoReuseIrp(pQueryIrp, STATUS_NOT_SUPPORTED);
	pQueryIrp->Flags = 0;
	pQueryIrp->RequestorMode = KernelMode;
	pQueryIrp->Tail.Overlay.Thread = 0;
	pStack = IoGetNextIrpStackLocation(pQueryIrp);
	pStack->MajorFunction = IRP_MJ_PNP;
	pStack->MinorFunction = IRP_MN_QUERY_ID;
	pStack->Parameters.QueryId.IdType = BusQueryCompatibleIDs;
	pStack->Flags = 0;
	pStack->DeviceObject = pTargStack;
	IrpStatus = SendIrpSync(pQueryIrp, &IrpComplEv);
	if (NT_SUCCESS(IrpStatus)) {
		if (pQueryIrp->IoStatus.Information) {
			CopyMultiWString(
				(PWCHAR) &pFaDevExt->TargDevInfo.CompatibleIDs, 
				(PWCHAR) pQueryIrp->IoStatus.Information,
				sizeof pFaDevExt->TargDevInfo.CompatibleIDs);
			ExFreePool((PVOID) pQueryIrp->IoStatus.Information);
		}
	}
	if (NT_SUCCESS(status)) status = IrpStatus;

	IoFreeIrp(pQueryIrp);

	return status;
}


//++
// Function:	GetWriteData
//
// Description:
//
//		Returns the data of a write IRP.
//
//		Looks into the filter device object to determine wether the
//		filtered device does direct or buffered I/O and obtains a 
//		pointer to the data block either from the IRP or from the MDL.
//
//		It relies on the fact that the buffering mode on the filter is
//		set to be equal to the one of the filtered device.
//
// Arguments:
//
//		pReadIrp - IRP address.
//
//		pDevExt - filter device extension address.
//
//		ppWriteData - address of a ptr which will receive the data 
//			address.
//
//			Before using the data address, a caller must check the
//			data length: if it is 0, the data address is not valid.
//
//		pcbDataLen - address of a variable which will receive the data
//			length.
//
// Return value: none
//
//--
static VOID GetWriteData(
	PIRP				pIrp, 
	PDEVICE_EXTENSION	pDevExt, 
	PVOID*				ppWriteData, 
	SIZE_T*				pcbDataLen)
{
	*ppWriteData = NULL;
	*pcbDataLen = 0;

	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	*pcbDataLen = pStack->Parameters.Write.Length;
	if (!*pcbDataLen) return;

	// Determine the buffering mode
	//
	if (pDevExt->hdr.pDevice->Flags & DO_DIRECT_IO) {

		PMDL pMdl = pIrp->MdlAddress;
		PCHAR pBufStart = (PCHAR) MmGetSystemAddressForMdlSafe(pMdl, 
				LowPagePriority);
		if (pBufStart != NULL) {
			*ppWriteData = pBufStart;
		} else {

			// This should never happen, because the client buffer
			// has already been locked in place by who issued the
			// read IRP.
			//
			*pcbDataLen = 0;
		}
	} else {

		// Buffered I/O
		//
		*ppWriteData = pIrp->AssociatedIrp.SystemBuffer;
	}

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - GetWriteData - Write data address: %#p, length: %#x\n",
		*ppWriteData, *pcbDataLen);

	return;
}


//++
// Function:	GTCompletion
//
// Description:
//
//		Completion routine for the generic tracing.
//
//		Stores the IRP and the current IO_STACK_LOCATION into the 
//		buffer.
//
//		Calls PoStartNextPowerIrp if the IRP is a power one.
//
// Arguments:
//
//
// Return value: 
//--
static NTSTATUS GTCompletion(
	PDEVICE_OBJECT		pDevObj,
	PIRP				pIrp,
	PVOID				Context)
{
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	UCHAR MajFun = pStack->MajorFunction;
	PDEVICE_EXTENSION pDevExt = 
		(PDEVICE_EXTENSION) Context;
	TraceIrpBasicInfo(pDevExt, "GTCompletion", pIrp);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"\tStatus = %#010x\n"
		"\tInformation = %#x\n",
		pIrp->IoStatus.Status,
		pIrp->IoStatus.Information);

	KLOCK_QUEUE_HANDLE klock;

	KeAcquireInStackQueuedSpinLock(&pDevExt->DataLock, &klock);

	InterceptIrpAndData(pIrp, NULL, 0, pDevExt);
	SendIrpDataToClt(pDevExt);

	KeReleaseInStackQueuedSpinLock(&klock);

	if (MajFun == IRP_MJ_POWER) PoStartNextPowerIrp(pIrp);
	if (pIrp->PendingReturned) IoMarkIrpPending(pIrp);
	return STATUS_SUCCESS;
}


//++
// Function:	InterceptIrpAndData
//
// Description:
//
//		Copies an IRP and its data (if any) into the internal
//		circular buffer.
//
//		It does not acquire the data spinlock, so the caller must
//		do it before invoking this function.
//
//		If it fails to store the data into the buffer,
//		sets the FSTATUS_DATA_LOST flag.
//
//		This function does not store partial data: either succeeds in
//		storing all the IRP or does not store anything.
//
//		It may set the FSTATUS_DATA_LOST flag also if older data is
//		discarded from the buffer to make room for the new one.
//
//		It stores:
//
//		- the IRP
//
//		- the current IO_STACK_LOCATION
//
//		- any data pointed by pData if cbDataLens is > 0.
//
// Arguments:
//
//		pIrp - Address of the IRP to copy.
//
//		pData - Address of the IRP data to copy. It is used only if
//			cbDataLen is > 0, otherwise it's ignored.
//
//		cbDataLen - length of the IRP data in bytes.
//
//		pDevExt - Address of the filter device extension.
//
// Return value: 
//
//		CB_STATUS_WARN_LOST - all or part of the data to intercept
//			have been lost.
//
//		STATUS_SUCCESS - all of the data succesfully copied.
//--
static VOID InterceptIrpAndData(
	PIRP				pIrp, 
	PVOID				pData, 
	SIZE_T				cbDataLen, 
	PDEVICE_EXTENSION	pDevExt)
{
	PCIRCULAR_BUFFER pBuf = &pDevExt->DataBuffer;
	NTSTATUS		status;

	// Allocate a buffer block to store the IRP with its header,
	// its I/O stack loc and its data, iF any.
	//
	status = CBAllocateBlock(
		pBuf,
		sizeof (IRP_HEADER) + sizeof (IRP) + sizeof(IO_STACK_LOCATION) + 
			cbDataLen
	);

	if (NT_ERROR(status)) {	// This should happen only if the size
								// is greater than the entire buffer 
								// size

		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - InterceptIrpAndData - Buffer allocation failed "
			"with status: %#010x\n", 
			status);
		// Set the data lost flag and quit
		//
		pDevExt->dwStatus |= FSTATUS_DATA_LOST;
		return;
	}

	if (status == CB_STATUS_WARN_DATA_LOST) {	// Data has been thrown 
		// out to make room into the buffer.

		pDevExt->dwStatus |= FSTATUS_DATA_LOST;
	}

	// Set up the IRP header and store it into the buffer.
	//
	IRP_HEADER IrpHea = {0};
	IrpHea.cbDataLen = cbDataLen;
	PCHAR pStore = (PCHAR) &IrpHea;
	SIZE_T cbBlockOffset = 0;
	SIZE_T cbStoreLength = sizeof IrpHea;

	// Don't check CBStore ret status, because we are sure we have 
	// allocated the block.
	//
	CBStore(pBuf, &pStore, &cbStoreLength, &cbBlockOffset, NULL);

	// Now store the IRP into the buffer.
	//
	pStore = (PCHAR) pIrp;
	cbStoreLength = sizeof (IRP);

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - InterceptIrpAndData - Storing IRP %#p into the "
		"buffer\n", pIrp);

	CBStore(pBuf, &pStore, &cbStoreLength, &cbBlockOffset, NULL);

	// Store the current stack location into the buffer.
	//
	pStore = (PCHAR) IoGetCurrentIrpStackLocation(pIrp);
	cbStoreLength = sizeof (IO_STACK_LOCATION);

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - InterceptIrpAndData - Storing current stack "
		"location %#p into the "
		"buffer\n", pStore);

	CBStore(pBuf, &pStore, &cbStoreLength, &cbBlockOffset, NULL);

	if (cbStoreLength = cbDataLen) {	// The = operator is 
										// intentional!
										// Check cbDataLen != 0 
										// and copy it into 
										// cbStoreLength.
		pStore = (PCHAR) pData;
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - InterceptIrpAndData - Storing %#x bytes of "
			"data at %#p into the "
			"buffer\n", cbDataLen, pStore);
		CBStore(pBuf, &pStore, &cbStoreLength, &cbBlockOffset, NULL);
	}
	return;
}


//++
// Function:	IoctlGetMJFFlags
//
// Description:
//
//		Gets the control flags for a major function code.
//
// Arguments:
//
//		pIrp - address of the IRP_MJ_CONTROL IRP specifying the flag
//			to be retreieved.
//
//		pFltDevExt - filter device extension.
//
// Return value: success or failure code.
//--
static NTSTATUS IoctlGetMJFFlags(
	PIRP				pIrp, 
	PDEVICE_EXTENSION	pFltDevExt)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	pIrp->IoStatus.Information = 0;
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	if (pStack->Parameters.DeviceIoControl.OutputBufferLength <
		sizeof (MJF_CONTROL)) {
		status = STATUS_INVALID_PARAMETER;
		goto QUIT;
	}
	PMJF_CONTROL pOutFlags = 
		(PMJF_CONTROL) pIrp->AssociatedIrp.SystemBuffer;
	UCHAR MajFun = pOutFlags->MajorFunction;
	if (MajFun > IRP_MJ_MAXIMUM_FUNCTION) {
		status = STATUS_INVALID_PARAMETER;
		goto QUIT;
	}
	pOutFlags->dwFlags = pFltDevExt->dwMJFlags[MajFun];
    status = STATUS_SUCCESS;
	pIrp->IoStatus.Information = sizeof (MJF_CONTROL);

QUIT:
	pIrp->IoStatus.Status = status;
	return status;
}


//++
// Function:	IoctlSetMJFFlags
//
// Description:
//
//		Sets the control flags for a major function code.
//
// Arguments:
//
//		pIrp - address of the IRP_MJ_CONTROL IRP specifying the flag
//			to be set.
//
//		pFltDevExt - filter device extension.
//
// Return value: success or failure code.
//--
static NTSTATUS IoctlSetMJFFlags(
	PIRP				pIrp, 
	PDEVICE_EXTENSION	pFltDevExt)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	pIrp->IoStatus.Information = 0;
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	if (pStack->Parameters.DeviceIoControl.InputBufferLength <
		sizeof (MJF_CONTROL)) {
		status = STATUS_INVALID_PARAMETER;
		goto QUIT;
	}
	PMJF_CONTROL pNewFlags = 
		(PMJF_CONTROL) pIrp->AssociatedIrp.SystemBuffer;
	UCHAR MajFun = pNewFlags->MajorFunction;
	if (MajFun == 0xff) {

		// 0xff means to set the flags for all the function codes.
		//
		for (INT i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++) {
			pFltDevExt->dwMJFlags[i] = pNewFlags->dwFlags;
		}
		status = STATUS_SUCCESS;
		goto QUIT;
	}
	if (MajFun > IRP_MJ_MAXIMUM_FUNCTION) {
		status = STATUS_INVALID_PARAMETER;
		goto QUIT;
	}
	pFltDevExt->dwMJFlags[MajFun] = pNewFlags->dwFlags;
    status = STATUS_SUCCESS;

QUIT:
	pIrp->IoStatus.Status = status;
	return status;
}


//++
// Function:	MainDispatch
//
// Description:
//
//		Main dispatch routine.
//
//		Detects wether the IRP is for the filter, the filter 
//		access or the master device and calls the corresponding dispatch 
//		routine.
//
// Arguments:
//
// Return value: 
//--
static NTSTATUS MainDispatch(
	PDEVICE_OBJECT		pDevObj,
	PIRP				pIrp)
{
	PDEVICE_EXTENSION	pDevExt;
	PIO_STACK_LOCATION	pStack;

	pDevExt = (PDEVICE_EXTENSION) pDevObj->DeviceExtension;

	switch (pDevExt->hdr.DevType) {
		case DT_FILTER:
			return FltMainDispatch(pDevObj, pIrp);
			break;
		case DT_FILTER_ACCESS:
			return FAMainDispatch(pDevObj, pIrp);
			break;
		 case DT_MASTER:
			 return MAMainDispatch(pDevObj, pIrp);
			 break;
		 default:
			pIrp->IoStatus.Status = STATUS_NOT_SUPPORTED;
			pIrp->IoStatus.Information = 0;
			IoCompleteRequest(pIrp, IO_NO_INCREMENT);
			return STATUS_NOT_SUPPORTED;
	}
}


//++
// Function:	MADispatchRead
//
// Description:
//
//	Read dispatch routine for the main device.
//
//	Returns to the client an ACCESS_DEVICES_LIST structure, which 
//	contains an array of TARGET_DEVICE_INFO, one for each
//	filter access device.
//
//	The client must supply a buffer of size >= sizeof 
//	ACCESS_DEVICES_LIST.
//
//	The member bIncomplete into ACCESS_DEVICES_LIST is set to
//	TRUE if there was not enough space to return all the
//	TARGET_DEVICE_INFOs available.
//
//	The member ReqBufSize is set to the total buffer size needed to
//	return all the TARGET_DEVICE_INFOs.
//
// Arguments:
//
//
// Return value: 
//--
static NTSTATUS MADispatchRead(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp)
{
	NTSTATUS status;

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - MADispatchRead - Entered. pIrp = %#p", pIrp);
	PIO_STACK_LOCATION pStack;
	PACCESS_DEVICES_LIST pAdl;
	status = CheckMAReadParams(pIrp, pDevObj, &pStack, &pAdl);

	if (!NT_SUCCESS(status)) return status;

	PMADEVICE_EXTENSION pMastDevExt = (PMADEVICE_EXTENSION) 
		pDevObj->DeviceExtension;

	// Compute the size the buffer must have to receive all the
	// data.
	//
	ULONG ReqBytes = FIELD_OFFSET(ACCESS_DEVICES_LIST, TargDevVect) +
		pMastDevExt->FADevsCount * sizeof (TARGET_DEVICE_INFO);
	if (pStack->Parameters.Read.Length < ReqBytes) {
		pAdl->bIncomplete = TRUE;
	} else {
		pAdl->bIncomplete = FALSE;
	}
	pAdl->ReqBufSize = ReqBytes;

	// Compute the # of TARGET_DEVICE_INFO that can fit into the buffer
	//
	pAdl->DevInfoCount = (pStack->Parameters.Read.Length -
		FIELD_OFFSET(ACCESS_DEVICES_LIST, TargDevVect)) / 
		sizeof (TARGET_DEVICE_INFO);

	// If the actual # of info struct is less, return the actual count.
	//
	if (pAdl->DevInfoCount > pMastDevExt->FADevsCount)
		pAdl->DevInfoCount = pMastDevExt->FADevsCount;

	// Compute the # of bytes transferred from the # of dev info
	//
	pIrp->IoStatus.Information = 
		FIELD_OFFSET(ACCESS_DEVICES_LIST, TargDevVect) + 
		pAdl->DevInfoCount *
		sizeof (TARGET_DEVICE_INFO);

	PLIST_ENTRY pDevInfoLink = pMastDevExt->FADevsList.Flink;

	// Copy only the TARGET_DEVICE_INFOs that fit.
	//
	for (ULONG i = 0; i < pAdl->DevInfoCount; i++) {
		PFADEVICE_EXTENSION pFADevExt =
			CONTAINING_RECORD(pDevInfoLink, FADEVICE_EXTENSION,
				ListEntry);
		pAdl->TargDevVect[i] = pFADevExt->TargDevInfo;
		pDevInfoLink = pDevInfoLink->Flink;
	}
	pIrp->IoStatus.Status = STATUS_SUCCESS;
	return STATUS_SUCCESS;
}


//++
// Function:	MajorFunctionDes
//
// Description:
//
//	Decodes the MajorFunction member of the stack location.
//
// Arguments:
//
//	MajorFunction - value to the decode.
//
//	pszDecodeBuf - output buffer
//
//	cbBufSize - buffer sizein bytes.
//
// Return value: 
//--
static PSTR MajorFunctionDes(
	 UCHAR		MajorFunction,
	 PSTR		pszDecodeBuf,
	 SIZE_T		cbBufSize)
{
	return DecodeEnum(
		MajorFunction,
		0,
		pszMajorFunctionV,
		sizeof pszMajorFunctionV / sizeof(PSTR),
		pszDecodeBuf,
		cbBufSize);
}




//++
// Function:	MAMainDispatch
//
// Description:
//
//	Master device main dispatch routine.
//
// Arguments:
//
//
// Return value: 
//--
static NTSTATUS MAMainDispatch(
	PDEVICE_OBJECT		pDevObj, 
	PIRP				pIrp)
{
	NTSTATUS status;
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	switch (pStack->MajorFunction) {
		case IRP_MJ_CLEANUP:
			status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0;
			pIrp->IoStatus.Status = status;
			break;
		case IRP_MJ_CLOSE:
			status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0;
			pIrp->IoStatus.Status = status;
			break;
		case IRP_MJ_CREATE:
			status = STATUS_SUCCESS;
			pIrp->IoStatus.Information = 0;
			pIrp->IoStatus.Status = status;
			break;
		case IRP_MJ_READ:
			status = MADispatchRead(pDevObj, pIrp);
			break;
		default:
			status = STATUS_NOT_SUPPORTED;
			pIrp->IoStatus.Information = 0;
			pIrp->IoStatus.Status = status;
	}
	if (status != STATUS_PENDING) {
		IoCompleteRequest(
			pIrp, IO_NO_INCREMENT);
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - MAMainDispatch - Client IRP %#p completed with "
			"status %#010x\n", 
			pIrp,
			pIrp->IoStatus.Status);
	}
	return status;
}


//++
// Function:	MinorFunctionDes
//
// Description:
//
//	Decodes the MinorFunction member of the stack location.
//
// Arguments:
//
//	MajorFunction - major function to which the minor function relates.
//
//	MinorFunction - value to decode.
//
//	pszDecodeBuf - output buffer.
//
//	cbBufSize - buffer size in bytes.
//
// Return value: address of pszDecodeBuf.
//--
static PSTR MinorFunctionDes(
	UCHAR		MajorFunction,
	UCHAR		MinorFunction,
	PSTR		pszDecodeBuf,
	SIZE_T		cbBufSize)
{
	switch (MajorFunction) {
		case IRP_MJ_PNP:
			return DecodeEnum(
				MinorFunction,
				0,
				pszPnpMinorFunctionV,
				sizeof pszPnpMinorFunctionV / sizeof (PSTR),
				pszDecodeBuf,
				cbBufSize);
			break;
		case IRP_MJ_POWER:
			return DecodeEnum(
				MinorFunction,
				0,
				pszPowerMinorFunctionV,
				sizeof pszPowerMinorFunctionV / sizeof (PSTR),
				pszDecodeBuf,
				cbBufSize);
			break;
		default:
			RtlStringCbPrintfA(
				pszDecodeBuf,
				cbBufSize,
				"%#x",
				MinorFunction);
			return pszDecodeBuf;
	}
}



//++
// Function:	MoveIrpDataToCltIrp
//
// Description:
//
//		Moves the information of an intercepted IRP into the output 
//		buffer of a client IRP (a read targeted at the filter access
//		device).
//
//		Extracts from the internal buffer the oldest
//		intercepted IRP, along with its data, if any.
//
//		Moves the extracted IRP + data into the client buffer.
//
//		If the client buffer is not large enough to hold all of the
//		IRP data, the tail of the data is lost. This condition is
//		notified to the client, into a control variable returned along
//		with the IRP data.
//
//		The data extracted are discarded from the internal buffer. Even 
//		if the tail of the data is lost, all of the data are removed
//		from the buffer, so that, upon returning from this routine,
//		the buffer content will begin with a new IRP preceeded by
//		its header, or the buffer will be empty.
//
//		The data returned into the client IRP buffer has the following
//		layout:
//
//		- an IRP_HEADER structure
//		- the IRP
//		- the I/O stack location that was current at the moment the
//		IRP was intercepted.
//		- the IRP data if any.
//
//		If not all of the IRP data could fit into the client buffer,
//		the dwFlags member of the IRP_HEADER will have the
//		IRPSTAT_DATA_LOST flag set.
//
//		There are no padding bytes between these structure, therefore
//		they are not guaranteed to be properly aligned. A client
//		should copy these data into properly aligned instances with
//		memcpy or similar functions.
//
//		Sets IoStatus.Status and IoStatus.Information of the client
//		IRP accordingly to the outcome.
//
//
// Arguments:
//
//		pDevExt - address of the filter device extension, which stores 
//			the data structures used to access the internal buffer.
//
//		pCltIrp - address of the client IRP.
//
//
// Return value: none.
//--
static VOID MoveIrpDataToCltIrp(
	PDEVICE_EXTENSION	pDevExt, 
	PIRP				pCltIrp)
{
	NTSTATUS status;

	// The client IRP is for the filter access device, which does
	// direct I/O.
	//
	PMDL pMdl = pCltIrp->MdlAddress;
	PCHAR pOutBuf = (PCHAR) MmGetSystemAddressForMdlSafe(pMdl, 
			LowPagePriority);
	if (pOutBuf == NULL) {

		// This should never happen, because the MDL has already been
		// locked by the I/O manager.
		//
		pCltIrp->IoStatus.Status = STATUS_NO_MEMORY;
		pCltIrp->IoStatus.Information = 0;
		return;
	}
	PIO_STACK_LOCATION pIoStack = IoGetCurrentIrpStackLocation(pCltIrp);
	SIZE_T cbRemaining = pIoStack->Parameters.Read.Length;

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - MoveIrpDataToCltIrp - Client IRP %#p: buffer %#p, "
		"len: %#x\n",
		pCltIrp,
		pOutBuf,
		cbRemaining);

	// We know that the data block is header + IRP + I/O stack +
	// (optionally) data.
	//
	// We also know that cbRemaining is >= the length of header, IRP and
	// I/O stack, because this is checked when client IRPs are received.
	//
	// Let's start by transferring the header.
	//
	PCIRCULAR_BUFFER pCbuf = &pDevExt->DataBuffer;

	// Extract the IRP header into a local variable, because we will 
	// need to access it here.
	//
	IRP_HEADER irpHeader;
	PCHAR pTmpRecvPointer = (PCHAR) &irpHeader;
	SIZE_T tmpRemaining = sizeof irpHeader;
	SIZE_T cbBytesToRead = sizeof irpHeader;
	SIZE_T cbXtractOffset = 0;
	status = CBXtractFromStart(
		pCbuf,
		&cbXtractOffset,
		&pTmpRecvPointer,
		&tmpRemaining,
		&cbBytesToRead,
		NULL);
	if (!NT_SUCCESS(status)) {

		// This should not happen. It means that there is no data
		// into the buffer, which should be prevented by design.
		//
		pCltIrp->IoStatus.Status = STATUS_UNSUCCESSFUL;
		pCltIrp->IoStatus.Information = 0;
		return;
	}

	// Look into the header to see if there are data bytes following 
	// the IRP and set a flag if the buffer isn't big enough.
	//
	SIZE_T maxDataLen  = cbRemaining - sizeof (IRP_HEADER) -
		sizeof (IRP) - sizeof(IO_STACK_LOCATION);
	if (irpHeader.cbDataLen > maxDataLen) {
		irpHeader.dwFlags |= IRPSTAT_DATA_LOST;

		// Adjust the data length returned to the client.
		//
		irpHeader.cbDataLen = maxDataLen;
	}

	// Copy the header content into the output buffer.
	//
	RtlCopyMemory(pOutBuf, &irpHeader, sizeof irpHeader);
	pOutBuf += sizeof irpHeader;
	cbRemaining -= sizeof irpHeader;

	// Extract the remainder of the data block directly into the output 
	// buffer.
	//
	// We are sure we have at least the IRP and the I/O stack location 
	// to tansfer. Optionally, we'll transfer some data, if there are 
	// any and if there's room in the client buffer.
	//
	CBXtractFromStart(
		pCbuf,
		&cbXtractOffset,
		&pOutBuf,
		&cbRemaining,
		NULL,
		NULL);

	// cbRemaining was originally set to the client buffer size, so we
	// can compute the number of bytes transferred.
	//
	pCltIrp->IoStatus.Information = pIoStack->Parameters.Read.Length -
		cbRemaining;
	pCltIrp->IoStatus.Status = STATUS_SUCCESS;


	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - MoveIrpDataToCltIrp - Passed %#x bytes of data to "
		"IRP %#p\n", 
		pCltIrp->IoStatus.Information, pCltIrp);

	// Discard the data block from the buffer
	//
	CBFreeOldestDataBlock(pCbuf);
	return;
}


//++
// Function:	QueueCltIrp
//
// Description:
//
//		Queues an IRP targeted at the filter access device in the 
//		internal queue.
//
//		If the queuing succeeds, marks the IRP as pending, because
//		it will be completed later.
//
//		Otherwise sets an error code into the IRP and returns the
//		same error to the caller.
//
// Arguments:
//
//		pDevExt - Filter device extension.
//
//		pCltIrp - IRP for the FA device.
//
// Return value: an error value if the IRP queuing fails or if it is 
//		canceled.
//
//		STATUS_PENDING otherwise.
//--
static NTSTATUS QueueCltIrp(
	PDEVICE_EXTENSION	pDevExt,
	PIRP	pCltIrp)
{
	IoSetCancelRoutine(pCltIrp, FACancel);
	IoMarkIrpPending(pCltIrp);
	InsertTailList(
		&pDevExt->pIrpList,
		&pCltIrp->Tail.Overlay.ListEntry);

	// Abount cancellation: this function is called inside the dispatch
	// routine, therefore phase 1 cannot begin while inside of it,
	// because it doesn't begin until the dispatch routine returns.
	//
	// We can therefore be sure that, if cancellation does occur, this
	// happens only after we have set up our cancel routine
	//
	return STATUS_PENDING;
}


//++
// Function:	QueueCltIrpForCompletion
//
// Description:
//
//		Places the client IRP into the internal completion queue and
//		requestes a DPC which will complete the queued IRPs.
//
//		Completion will occur in a DPC, to avoid doing it while
//		holding the data spinlock and while processing a filtered IRP.
//
//		The internal completion queue is used to guarantee client IRPs 
//		are completed in the same order they were issued.
//
// Arguments:
//
//		pDevExt - address of the filter device extension, which stores 
//			the data structures used to access the internal queue.
//
//		pCltIrp - address of the client IRP.
//
//
// Return value: none.
//--
static VOID QueueCltIrpForCompletion(
	PDEVICE_EXTENSION	pDevExt,
	PIRP				pCltIrp)
{
	InsertTailList(
		&pDevExt->pWaitingComplIrpList,
		&pCltIrp->Tail.Overlay.ListEntry);
	KeInsertQueueDpc(
		&pDevExt->ComplDpc,
		NULL,
		NULL);

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - QueueCltIrpForCompletion - Client IRP %#p queued "
		"for completion\n", 
		pCltIrp);
}


//++
// Function:	ReadCompletion
//
// Description:
//		Completion routine for IRP_MJ_READ requests.
//
//		Stores the IRP and its data in the internal buffer.
//
//		If there are client IRPs waiting for the intercepted data,
//		moves the data into their buffers and schedules them for
//		completion.
//
// Arguments:
//
// Return value:
//--
static NTSTATUS ReadCompletion(
	PDEVICE_OBJECT		pDevObj,
	PIRP				pIrp,
	PVOID				pContext)
{
	PDEVICE_EXTENSION pDevExt = 
		(PDEVICE_EXTENSION) pDevObj->DeviceExtension;

	if (pDevExt->dwMJFlags[IRP_MJ_READ] & FCTL_TRACE_ON) {
		TraceIrpBasicInfo(pDevExt, "ReadCompletion", pIrp);
		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"\tStatus = %#010x\n"
			"\tInformation = %#x\n",
			pIrp->IoStatus.Status,
			pIrp->IoStatus.Information);

		// Get the IRP data, if any.
		//
		PVOID pReadData;
		SIZE_T cbReadDataLen;
		GetReadData(pIrp, pDevExt, &pReadData, &cbReadDataLen);

		KLOCK_QUEUE_HANDLE	LockHandle;
		KeAcquireInStackQueuedSpinLock(
			&pDevExt->DataLock,
			&LockHandle);
		InterceptIrpAndData(pIrp, pReadData, cbReadDataLen, pDevExt);
		SendIrpDataToClt(pDevExt);
		KeReleaseInStackQueuedSpinLock(&LockHandle);
	}

	if (pIrp->PendingReturned) IoMarkIrpPending(pIrp);
	return STATUS_SUCCESS;
}


//++
// Function:	SendIrpDataToClt
//
// Description:
//
//		Checks wether there are intercepted IRPs in the buffer and 
//		waiting client IRPs and, if so, passes the intercepted
//		IRPs to the client ones and schedules them for completion.
//
//		It does not acquire the data spinlock, so the caller must
//		do it before invoking this function.
//
// Arguments:
//
//		pDevExt - ptr to device extension.
//
// Return value: --
//--
static VOID SendIrpDataToClt(
	PDEVICE_EXTENSION	pDevExt)
{
	PLIST_ENTRY pIrpList = &pDevExt->pIrpList;

	// Pass the intercepted IRPs and their data to any
	// waiting client IRP.
	//
	while (!IsListEmpty(pIrpList)) {
		if (CBDataBufferEmpty(&pDevExt->DataBuffer)) break;
		PLIST_ENTRY pFirst = RemoveHeadList(pIrpList);

		// About IRP cancellation: we are removing the IRP from the
		// queue, so, should the I/O manager cancel it, the cancel 
		// routine will not process it.
		//
		// This is OK, because from now on, the IRP will not be held
		// into a queue waiting for something (namely, the presence
		// of intercepted data). It will instead, be completed. So,
		// we are not interested in its cancellation: what's important 
		// is not to hold indefinitely into a queue an IRP which must be 
		// cancelled.
		//
		// True, the IRP will be queued waiting for the DPC which will
		// complete it, but this is bound to happen. That is the
		// IRP will not be held indefinitely, waiting for some event
		// to occur, as is the case when it's in the queue waiting for
		// intercepted data.

		PIRP pClientIrp = CONTAINING_RECORD(pFirst, IRP, 
			Tail.Overlay.ListEntry);

		DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
			"FLT - SendIrpDataToClt - Client IRP %#p dequeued\n", 
			pClientIrp);

		MoveIrpDataToCltIrp(pDevExt, pClientIrp);
		QueueCltIrpForCompletion(pDevExt, pClientIrp);
	}
	return;
}


//++
// Function:	SendIrpSync
//
// Description:
//
//	Sends an IRP to the next device in the stack and waits for it 
//	to complete, using the provided PKEVENT.
//
//	The completion routine used returns STATUS_MORE_PROCESSING_REQUIRED.
//
// Arguments:
//
//	pIrp - address of the IRP to send
//
//	pEvent - address of the event to use to wait for the event to
//		complete.
//
// Return value: the NTSTATUS returned by the called device.
//--
static NTSTATUS SendIrpSync(
	PIRP		pIrp,
	PKEVENT		pEvent)
{
	KeClearEvent (pEvent);
	IoSetCompletionRoutine(pIrp, SendIrpSyncCompletion, pEvent, TRUE, 
		TRUE, TRUE);
	PIO_STACK_LOCATION pNextStack = IoGetNextIrpStackLocation(pIrp);
	NTSTATUS status = IoCallDriver(
		pNextStack->DeviceObject,
		pIrp);
	if (status == STATUS_PENDING) {
		KeWaitForSingleObject(pEvent, Executive, KernelMode, FALSE, NULL);
		status = pIrp->IoStatus.Status;
	}
	return status;
}


//++
// Function:	SendIrpSyncCompletion
//
// Description:
//
//	Completion routine used by SendIrpSync.
//
//	Signals the event passed into the Context param an returns
//	STATUS_MORE_PROCESSING_REQUIRED.
//
// Arguments:
//
//	Context - address of the KEVENT to signal.
//
// Return value: 
//--
static NTSTATUS SendIrpSyncCompletion(
	PDEVICE_OBJECT	pDevObj,
	PIRP			pIrp,
	PVOID			Context)
{
	KeSetEvent((PKEVENT) Context, IO_NO_INCREMENT, FALSE);
	return STATUS_MORE_PROCESSING_REQUIRED;
}


//++
// Function:	TraceIrpBasicInfo
//
// Description:
//
//	Sends to the debugger basic information on an IRP and on the filter
//	device targeted by the IRP.
//
// Arguments:
//
//	pDevExt - address of a filter device extension.
//
//	pszCallingFunction - name of the calling function which will be
//		printed into the debug message.
//
//	pIrp - IRP to trace.
//
// Return value: none.
//--
static VOID TraceIrpBasicInfo(
	PDEVICE_EXTENSION		pFltDevExt, 
	PSTR					pszCallingFunction,
	PIRP					pIrp)
{
	PIO_STACK_LOCATION pStack = 
		IoGetCurrentIrpStackLocation(pIrp);
	CHAR szBuf[101];
	CHAR szBufMin[101];

	// Retrieve the device text and IDs
	//
	PTARGET_DEVICE_INFO pTargInf = &pFltDevExt->pFADevExt->TargDevInfo;
	PWCHAR pwszDevText = pTargInf->DeviceText;
	PWCHAR pwszHwID = pTargInf->HardwareIDs;
	PWCHAR pwszInstID = pTargInf->InstanceId;
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"FLT - %s - Tracing IRP %#p\n"
		"\tMajorFunction = %s\n"
		"\tMinorFunction = %s\n"
		"\tFilter %#p\n"
		"\ttarget text    %S\n"
		"\ttarget HW ID   %S\n"
		"\ttarget inst ID %S\n",
		pszCallingFunction,
		pIrp,
		MajorFunctionDes(pStack->MajorFunction, szBuf, 
			sizeof szBuf), 
		MinorFunctionDes(pStack->MajorFunction, 
			pStack->MinorFunction, szBufMin, sizeof szBufMin),
			pFltDevExt->hdr.pDevice,
		pwszDevText,
		pwszHwID,
		pwszInstID
		);
}