// Functions whose source code is the same for different platforms 
// (i. e. architecture + OS version) but must be compiled in a different
// translation unit for each platform.
//

#pragma warning(disable:6209)

#define FORMAT_ITEM(	\
	wszBuffer,			\
	wszFormat,			\
	Item,				\
	ppFmtBuf,			\
	pcbOffset,			\
	pcbBufSize,			\
	hHeap,				\
	bRet)				\
						\
	StringCbPrintfW(	\
		wszBuffer,		\
		sizeof wszBuffer, \
		wszFormat,		\
		Item);			\
	bRet = AppendString(wszBuffer, ppFmtBuf, pcbOffset, pcbBufSize, \
		hHeap);


//----------------------------------------------------------------------
//
// Static variables
// ----------------

static PWSTR pwszDevicePowerStateV[] = {
	L"PowerDeviceUnspecified",
	L"PowerDeviceD0",
	L"PowerDeviceD1",
	L"PowerDeviceD2",
	L"PowerDeviceD3",
	L"PowerDeviceMaximum"
};

static PWSTR pwszDeviceRelationTypeV[] = {
	L"BusRelations",
	L"EjectionRelations",
	L"PowerRelations",
	L"RemovalRelations",
	L"TargetDeviceRelation",
	L"SingleBusRelations"
};

static PWSTR pwszFsInfoClassV[] = {
	L"FileFsVolumeInformation",
	L"FileFsLabelInformation"
    L"FileFsSizeInformation",       
    L"FileFsDeviceInformation",     
    L"FileFsAttributeInformation",  
    L"FileFsControlInformation",    
    L"FileFsFullSizeInformation",   
    L"FileFsObjectIdInformation",   
    L"FileFsDriverPathInformation", 
    L"FileFsVolumeFlagsInformation",
    L"FileFsMaximumInformation"
};

static PWSTR pwszPowerActionV[] = {
	L"PowerActionNone",
	L"PowerActionReserved",
	L"PowerActionSleep",
	L"PowerActionHibernate",
	L"PowerActionShutdown",
	L"PowerActionShutdownReset",
	L"PowerActionShutdownOff",
	L"PowerActionWarmEject"
};

static PWSTR pwszPowerStateTypeV[] = {
	L"SystemPowerState",
	L"DevicePowerState"
};

static PWSTR pwszSystemPowerStateV[] = {
	L"PowerSystemUnspecified",
	L"PowerSystemWorking",
	L"PowerSystemSleeping1",
	L"PowerSystemSleeping2",
	L"PowerSystemSleeping3",
	L"PowerSystemHibernate",
	L"PowerSystemShutdown",
	L"PowerSystemMaximum"
};

//----------------------------------------------------------------------
//
// Function prototypes
// -------------------

static PWSTR DecodeDevicePowerState(
	PWSTR					wszOutBuf, 
	SIZE_T					cbBufSize,
	DEVICE_POWER_STATE		DevPState);

static PWSTR DecodeFsInfoClass(
	PWSTR					wszOutBuf, 
	SIZE_T					cbBufSize,
	FS_INFORMATION_CLASS	FsInfoClass);

static PWSTR DecodePowerAction(
	PWSTR			wszOutBuf, 
	SIZE_T			cbBufSize,
	POWER_ACTION	Action);

static PWSTR DecodePowerStateType(
	PWSTR				wszOutBuf, 
	SIZE_T				cbBufSize,
	POWER_STATE_TYPE	PSType);

static PWSTR DecodeRelType(
	PWSTR					wszOutBuf, 
	SIZE_T					cbBufSize,
	DEVICE_RELATION_TYPE	DRType);

static PWSTR DecodeSystemPowerState(
	PWSTR					wszOutBuf, 
	SIZE_T					cbBufSize,
	SYSTEM_POWER_STATE		SysPState);

static BOOL FmtIopCreate(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap);

static BOOL FmtIopDeviceControl(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap);

static BOOL FmtIopQueryDeviceRelations(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap);

static BOOL FmtIopQueryInformation(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap);

static BOOL FmtIopQueryVolumeInformation(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap);

static BOOL FmtIopRead(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap);

static BOOL FmtIopSetInformation(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap);

static BOOL FmtIopWaitWake(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap);

static BOOL FmtIopWrite(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap);

static BOOL FormatIosParameters(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap);


//----------------------------------------------------------------------
//
// Function definitions
// --------------------


//----------------------------------------------------------------------
static PWSTR DecodeDevicePowerState(
	PWSTR					wszOutBuf, 
	SIZE_T					cbBufSize,
	DEVICE_POWER_STATE		DevPState)
{
	return DecodeEnum(
		DevPState,
		0,
		pwszDevicePowerStateV,
		sizeof pwszDevicePowerStateV / sizeof (PWSTR),
		wszOutBuf,
		cbBufSize);
}


//----------------------------------------------------------------------
static PWSTR DecodeFsInfoClass(
	PWSTR					wszOutBuf, 
	SIZE_T					cbBufSize,
	FS_INFORMATION_CLASS	FsInfoClass)
{
	return DecodeEnum(
		FsInfoClass,
		1, 
		pwszFsInfoClassV,
		sizeof pwszFsInfoClassV / sizeof (PWSTR), 
		wszOutBuf, 
		cbBufSize);
}


//----------------------------------------------------------------------
static PWSTR DecodePowerAction(
	PWSTR			wszOutBuf, 
	SIZE_T			cbBufSize,
	POWER_ACTION	Action)
{
	return DecodeEnum(
		Action,
		0, 
		pwszPowerActionV,
		sizeof pwszPowerActionV / sizeof (PWSTR), 
		wszOutBuf, 
		cbBufSize);
}

//----------------------------------------------------------------------
static PWSTR DecodePowerStateType(
	PWSTR				wszOutBuf, 
	SIZE_T				cbBufSize,
	POWER_STATE_TYPE	PSType)
{
	return DecodeEnum(
		PSType,
		0,
		pwszPowerStateTypeV,
		sizeof pwszPowerStateTypeV / sizeof (PWSTR),
		wszOutBuf,
		cbBufSize);
}


//----------------------------------------------------------------------
static PWSTR DecodeRelType(
	PWSTR					wszOutBuf, 
	SIZE_T					cbBufSize,
	DEVICE_RELATION_TYPE	DRType)
{
	return DecodeEnum(
		DRType,
		0,
		pwszDeviceRelationTypeV,
		sizeof pwszDeviceRelationTypeV / sizeof (PWSTR),
		wszOutBuf,
		cbBufSize);
}


//----------------------------------------------------------------------
static PWSTR DecodeSystemPowerState(
	PWSTR					wszOutBuf, 
	SIZE_T					cbBufSize,
	SYSTEM_POWER_STATE		SysPState)
{
	return DecodeEnum(
		SysPState,
		0,
		pwszSystemPowerStateV,
		sizeof pwszSystemPowerStateV / sizeof (PWSTR),
		wszOutBuf,
		cbBufSize);
}


//----------------------------------------------------------------------
static BOOL FmtIopCreate(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{
	BOOL	bRet;
	WCHAR	wszItemBuf[FMTIRP_ITEM_BUF_SIZE];

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Create.SecurityContext =             %#p",
		pIoStack->Parameters.Create.SecurityContext,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Create.Options =                     %#x",
		pIoStack->Parameters.Create.Options,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Create.FileAttributes =              %#x",
		pIoStack->Parameters.Create.FileAttributes,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Create.ShareAccess =                 %#x",
		pIoStack->Parameters.Create.ShareAccess,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Create.EaLength	 =                %#x",
		pIoStack->Parameters.Create.EaLength,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	return TRUE;
}


//----------------------------------------------------------------------
static BOOL FmtIopDeviceControl(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{
	BOOL	bRet;
	WCHAR	wszItemBuf[FMTIRP_ITEM_BUF_SIZE];

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.DeviceIoControl.OutputBufferLength = %#x",
		pIoStack->Parameters.DeviceIoControl.OutputBufferLength,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.DeviceIoControl.InputBufferLength =  %#x",
		pIoStack->Parameters.DeviceIoControl.InputBufferLength,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.DeviceIoControl.IoControlCode =      %#x",
		pIoStack->Parameters.DeviceIoControl.IoControlCode,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.DeviceIoControl.Type3InputBuffer =   %#p",
		pIoStack->Parameters.DeviceIoControl.Type3InputBuffer,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	return TRUE;
}


//----------------------------------------------------------------------
static BOOL FmtIopQueryDeviceRelations(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{
	BOOL	bRet;
	WCHAR	wszRelType[FMTIRP_ITEM_BUF_SIZE];
	WCHAR	wszItemBuf[FMTIRP_ITEM_BUF_SIZE];

	DecodeRelType(wszRelType, sizeof wszRelType, 
		pIoStack->Parameters.QueryDeviceRelations.Type);

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.QueryDeviceRelations.Type =          %s",
		wszRelType,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	return TRUE;
}


//----------------------------------------------------------------------
static BOOL FmtIopQueryInformation(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{
	BOOL	bRet;
	WCHAR	wszItemBuf[FMTIRP_ITEM_BUF_SIZE];

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.QueryFile.Length =                   %#x",
		pIoStack->Parameters.QueryFile.Length,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.QueryFile.FileInformationClass =     %d",
		pIoStack->Parameters.QueryFile.FileInformationClass,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	return TRUE;
}


//----------------------------------------------------------------------
static BOOL FmtIopQueryPower(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{
	BOOL	bRet;
	WCHAR	wszDecoded[FMTIRP_ITEM_BUF_SIZE];
	WCHAR	wszItemBuf[FMTIRP_ITEM_BUF_SIZE];

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Power.SystemContext =                %#x",
		pIoStack->Parameters.Power.SystemContext,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	DecodePowerStateType(wszDecoded, sizeof wszDecoded, 
		pIoStack->Parameters.Power.Type);
	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Power.Type =                         %s",
		wszDecoded,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	DecodeSystemPowerState(wszDecoded, sizeof wszDecoded, 
		pIoStack->Parameters.Power.State.SystemState);
	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Power.State.SystemState =            %s",
		wszDecoded,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	DecodeDevicePowerState(wszDecoded, sizeof wszDecoded, 
		pIoStack->Parameters.Power.State.DeviceState);
	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Power.State.DeviceState =            %s",
		wszDecoded,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	DecodePowerAction(wszDecoded, sizeof wszDecoded, 
		pIoStack->Parameters.Power.ShutdownType);
	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Power.ShutdownType =                 %s",
		wszDecoded,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	return TRUE;
}




//----------------------------------------------------------------------
static BOOL FmtIopQueryVolumeInformation(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{
	BOOL	bRet;
	WCHAR	wszDecoded[FMTIRP_ITEM_BUF_SIZE];
	WCHAR	wszItemBuf[FMTIRP_ITEM_BUF_SIZE];

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.QueryVolume.FsInformationClass =     %s",
		DecodeFsInfoClass(wszDecoded, sizeof wszDecoded, 
			pIoStack->Parameters.QueryVolume.FsInformationClass),
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------
static BOOL FmtIopRead(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{
	BOOL	bRet;
	WCHAR	wszItemBuf[FMTIRP_ITEM_BUF_SIZE];

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Read.Length =                        %#x",
		pIoStack->Parameters.Read.Length,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Read.Key =                           %#x",
		pIoStack->Parameters.Read.Key,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Read.ByteOffset =                    %#I64x",
		pIoStack->Parameters.Read.ByteOffset.QuadPart,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	return TRUE;
}


//----------------------------------------------------------------------
static BOOL FmtIopSetInformation(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{
	BOOL	bRet;
	WCHAR	wszItemBuf[FMTIRP_ITEM_BUF_SIZE];

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.SetFile.Length =                     %#x",
		pIoStack->Parameters.SetFile.Length,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.SetFile.FileInformationClass =       %d",
		pIoStack->Parameters.SetFile.FileInformationClass,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.SetFile.FileObject =                 %#p",
		pIoStack->Parameters.SetFile.FileObject,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.SetFile.ReplaceIfExists =            %d",
		pIoStack->Parameters.SetFile.ReplaceIfExists,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.SetFile.AdvanceOnly =                %d",
		pIoStack->Parameters.SetFile.AdvanceOnly,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.SetFile.ClusterCount =               %#x",
		pIoStack->Parameters.SetFile.ClusterCount,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.SetFile.DeleteHandle =               %#x",
		pIoStack->Parameters.SetFile.DeleteHandle,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	return TRUE;
}


//----------------------------------------------------------------------
static BOOL FmtIopWaitWake(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{
	BOOL	bRet;
	WCHAR	wszDecoded[FMTIRP_ITEM_BUF_SIZE];
	WCHAR	wszItemBuf[FMTIRP_ITEM_BUF_SIZE];

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.WaitWake.PowerState =                %s",
		DecodeSystemPowerState(
			wszDecoded,
			sizeof wszDecoded,
			pIoStack->Parameters.WaitWake.PowerState),
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	return TRUE;
}

//----------------------------------------------------------------------
static BOOL FmtIopWrite(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{
	BOOL	bRet;
	WCHAR	wszItemBuf[FMTIRP_ITEM_BUF_SIZE];

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Write.Length =                       %#x",
		pIoStack->Parameters.Write.Length,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Write.Key =                          %#x",
		pIoStack->Parameters.Write.Key,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	FORMAT_ITEM(
		wszItemBuf, 
		L"Parameters.Write.ByteOffset =                   %#I64x",
		pIoStack->Parameters.Write.ByteOffset,
		ppFmtBuf,
		pcbOffset,
		pcbBufSize,
		hHeap,
		bRet)
	if (!bRet) return FALSE;

	return TRUE;
}


//----------------------------------------------------------------------
static BOOL FormatIosParameters(
	PCHAR*				ppFmtBuf, 
	SIZE_T*				pcbOffset, 
	PIO_STACK_LOCATION	pIoStack, 
	SIZE_T*				pcbBufSize, 
	HANDLE				hHeap)
{

	switch (pIoStack->MajorFunction) {
		case IRP_MJ_CREATE:
			return FmtIopCreate(ppFmtBuf, pcbOffset, pIoStack, 
				pcbBufSize, hHeap);
			break;

		case IRP_MJ_DEVICE_CONTROL:
		case IRP_MJ_INTERNAL_DEVICE_CONTROL:

			return FmtIopDeviceControl(ppFmtBuf, pcbOffset, pIoStack, 
				pcbBufSize, hHeap);
			break;

		case IRP_MJ_PNP:
			switch (pIoStack->MinorFunction) {
				case IRP_MN_QUERY_DEVICE_RELATIONS:

					return FmtIopQueryDeviceRelations(ppFmtBuf, pcbOffset, pIoStack, 
						pcbBufSize, hHeap);
					break;
			}

			break;

		case IRP_MJ_POWER:
			switch (pIoStack->MinorFunction) {

				case IRP_MN_QUERY_POWER:
				case IRP_MN_SET_POWER:

					return FmtIopQueryPower(ppFmtBuf, pcbOffset, pIoStack, 
						pcbBufSize, hHeap);

					break;

				case IRP_MN_WAIT_WAKE:

					return FmtIopWaitWake(ppFmtBuf, pcbOffset, pIoStack, 
						pcbBufSize, hHeap);

					break;
			}
			break;

		case IRP_MJ_QUERY_INFORMATION:

			return FmtIopQueryInformation(ppFmtBuf, pcbOffset, pIoStack, 
				pcbBufSize, hHeap);
			break;

		case IRP_MJ_QUERY_VOLUME_INFORMATION:

			return FmtIopQueryVolumeInformation(ppFmtBuf, pcbOffset, pIoStack, 
				pcbBufSize, hHeap);
			break;

		case IRP_MJ_READ:

			return FmtIopRead(ppFmtBuf, pcbOffset, pIoStack, 
				pcbBufSize, hHeap);
			break;

		case IRP_MJ_SET_INFORMATION:

			return FmtIopSetInformation(ppFmtBuf, pcbOffset, pIoStack, 
				pcbBufSize, hHeap);
			break;

		case IRP_MJ_WRITE:

			return FmtIopWrite(ppFmtBuf, pcbOffset, pIoStack, 
				pcbBufSize, hHeap);
			break;

	}
	return TRUE;
}


//----------------------------------------------------------------------
static BOOL FormatIoStackGeneric(
	PCHAR		pIoStackData,
	PWCHAR*		ppFStack)
{
	IO_STACK_LOCATION	IoStack;

	memcpy(&IoStack, pIoStackData, sizeof IoStack);

	HANDLE hHeap = GetProcessHeap();

	if (hHeap == NULL) return FALSE;

	SIZE_T BufSize = FMTIRP_INITIAL_BUF_SIZE;
	PCHAR pFIoStack = (PCHAR) AllocFromHeap(hHeap, BufSize);

	if (pFIoStack == NULL) return FALSE;
	
	BOOL bRet = FALSE;
	SIZE_T cbOffset = 0;
	WCHAR wszItemBuf[FMTIRP_ITEM_BUF_SIZE];
	WCHAR wszDecodeBuf[FMTIRP_ITEM_BUF_SIZE];
	SIZE_T cbItemLen;

	FORMAT_ITEM(
		wszItemBuf,
		L"MajorFunction =                                 %s",
		MajorFunctionDes(
			IoStack.MajorFunction,
			wszDecodeBuf,
			sizeof wszDecodeBuf),
		&pFIoStack,
		&cbOffset,
		&BufSize,
		hHeap,
		bRet)
	if (!bRet)
		goto CLEANUP;

	FORMAT_ITEM(
		wszItemBuf,
		L"MinorFunction =                                 %s",
		MinorFunctionDes(
			IoStack.MajorFunction,
			IoStack.MinorFunction,
			wszDecodeBuf,
			sizeof wszDecodeBuf),
		&pFIoStack,
		&cbOffset,
		&BufSize,
		hHeap,
		bRet)
	if (!bRet)
		goto CLEANUP;


	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"Flags =                                         %#x",
		IoStack.Flags);

	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIoStack, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"Control =                                       %#x",
		IoStack.Control);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIoStack, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	if (!FormatIosParameters(&pFIoStack, &cbOffset, &IoStack, &BufSize, 
		hHeap))
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"DeviceObject =                                  %#p",
		IoStack.DeviceObject);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIoStack, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"FileObject =                                    %#p",
		IoStack.DeviceObject);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIoStack, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;


	// Add the extra null terminator which marks the text end
	//
	wszItemBuf[0] = L'\0';
	cbItemLen = sizeof (WCHAR);
	if (!CopyToBuffer(&pFIoStack, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	// If we made it here, success.
	//
	*ppFStack = (PWCHAR) pFIoStack;
	bRet = TRUE;

CLEANUP:
	if (!bRet) {
		HeapFree(hHeap, 0, pFIoStack);
	}
	return bRet;
}


//----------------------------------------------------------------------
static BOOL FormatIrpGeneric(
	PCHAR		pIrpData, 
	PWCHAR		*ppFIrp)
{
	BOOL		bRet ;
	SIZE_T		BufSize;
	HANDLE		hHeap;
	IRP			Irp;
	WCHAR		wszItemBuf[FMTIRP_ITEM_BUF_SIZE];
	PCHAR		pFIrp;


	// We cannot be sure that the address in pIrpData satisfies the
	// alignment requirement for the IRP type, so we allocate Irp
	// on the stack, which is properly aligned and then we copy
	// the data pointed by pIrpData into it.
	//
	memcpy(&Irp, pIrpData, sizeof Irp);

	hHeap = GetProcessHeap();

	if (hHeap == NULL) return FALSE;

	BufSize = FMTIRP_INITIAL_BUF_SIZE;
	pFIrp = (PCHAR) AllocFromHeap(hHeap, BufSize);

	if (pFIrp == NULL) return FALSE;
	
	bRet = FALSE;
	SIZE_T cbOffset = 0;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"MdlAddress =                                    %#p",
		Irp.MdlAddress);
	SIZE_T cbItemLen;
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);

	// Count the null terminator into the length
	//
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"Flags =                                         %#x",
		Irp.Flags);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"AssociatedIrp.MasterIrp =                       %#p",
		Irp.AssociatedIrp.MasterIrp);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"AssociatedIrp.SystemBuffer =                    %#p",
		Irp.AssociatedIrp.SystemBuffer);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"IoStatus.Status =                               %#x",
		Irp.IoStatus.Status);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"IoStatus.Information =                          %#x",
		Irp.IoStatus.Information);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"RequestorMode =                                 %d",
		Irp.RequestorMode);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"PendingReturned =                               %d",
		Irp.PendingReturned);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"Cancel =                                        %d",
		Irp.Cancel);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"CancelIrql =                                    %d",
		Irp.CancelIrql);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"CancelRoutine =                                 %#p",
		Irp.CancelRoutine);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"UserBuffer =                                    %#p",
		Irp.UserBuffer);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	StringCbPrintfW(
		wszItemBuf,
		sizeof wszItemBuf,
		L"Tail.Overlay.Thread =                           %#p",
		Irp.Tail.Overlay.Thread);
	StringCbLength(wszItemBuf, STRSAFE_MAX_CCH, (size_t*) &cbItemLen);
	cbItemLen += sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	// Add the extra null terminator which marks the text end
	//
	wszItemBuf[0] = L'\0';
	cbItemLen = sizeof (WCHAR);
	if (!CopyToBuffer(&pFIrp, &cbOffset, (PCHAR) wszItemBuf, cbItemLen, 
		&BufSize, hHeap)) 
		goto CLEANUP;

	// If we made it here, success.
	//
	*ppFIrp = (PWCHAR) pFIrp;
	bRet = TRUE;

CLEANUP:
	if (!bRet) {
		HeapFree(hHeap, 0, pFIrp);
	}
	return bRet;
}
