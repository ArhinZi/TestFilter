// Definitions for the Driver
// ==========================
//
// (c), 2003 by Enrico Martignetti - All rights reserved.

//----------------------------------------------------------------------
//
// Defines
// -------

// Name of the logical filter device 
//
#define DRV_DEVICE_NAME			L"TestFilter"

// Name of the logical filter access device
//
#define FADEVICE_NAME			L"FiltAccess"

// Filter control flag bits definitions
//
#define FCTL_TRACE_ON			0x1

// Filter status flag bits definitions
//
#define	FSTATUS_DATA_LOST		0x1

// Filter Access device IOCTL codes
//
#define IOCTL_FILTER_GET_MJF_FLAGS	CTL_CODE(FILE_DEVICE_UNKNOWN, \
	0x800, METHOD_BUFFERED, FILE_READ_DATA)
#define IOCTL_FILTER_SET_MJF_FLAGS	CTL_CODE(FILE_DEVICE_UNKNOWN, \
	0x801, METHOD_BUFFERED, FILE_WRITE_DATA)

// IRP status flag bits definitions
//

// The IRP data are incomplete.
//
#define IRPSTAT_DATA_LOST		0x1

// Name of the master device
//
#define	MASTER_DEVICE_NAME		L"FilterMasterDevice"


// Buffer sizes for the device informational data
//
#define TEXT_BUFFER_SIZE	101

#define COMPATIBLE_ID_SIZE	TEXT_BUFFER_SIZE
#define DEVICE_ID_SIZE		TEXT_BUFFER_SIZE
#define DEVICE_TEXT_SIZE	TEXT_BUFFER_SIZE
#define FILT_BUF_SIZE	(PAGE_SIZE * 20)
#define HARDWARE_ID_SIZE	TEXT_BUFFER_SIZE
#define INSTANCE_ID_SIZE	TEXT_BUFFER_SIZE
#define NAME_BUF_SIZE	51


//----------------------------------------------------------------------
//
// Data Types
// ----------

// Header which preceeds an IRP in the data returned by the filter.
//
typedef struct _IRP_HEADER{

	// Additional info on the IRP that follows.
	//
	DWORD		dwFlags;

	// Length of the data block which follows the IRP.
	//
	SIZE_T		cbDataLen;
} IRP_HEADER, *PIRP_HEADER;

// Control variables for IOCTL codes to get/set the MFJ flags.
//
typedef struct _MJF_CONTROL {
	UCHAR		MajorFunction;
	DWORD		dwFlags;
} MJF_CONTROL, *PMJF_CONTROL;



// Informational data of the filtered device
//
typedef struct _TARGET_DEVICE_INFO {

	// Name of the symbolic link to the device
	//
	WCHAR				SymLink[NAME_BUF_SIZE + 1];

	// Buffer for the 'compatible IDs' of the filtered device
	//
	WCHAR				CompatibleIDs[COMPATIBLE_ID_SIZE];

	// Buffer for the 'device id' of the filtered device
	//
	WCHAR				DeviceId[DEVICE_ID_SIZE];

	// Buffer for the 'device text' of the filtered device
	//
	WCHAR				DeviceText[DEVICE_TEXT_SIZE];

	// Buffer for the 'hardware IDs' of the filtered device
	//
	WCHAR				HardwareIDs[HARDWARE_ID_SIZE];

	// Buffer for the 'instance id' of the filtered device
	//
	WCHAR				InstanceId[INSTANCE_ID_SIZE];
} TARGET_DEVICE_INFO, *PTARGET_DEVICE_INFO;


// Structure returned by master device read
//
typedef struct _ACCESS_DEVICES_LIST {

	// TRUE if there were more data than could fit into the buffer,
	//
	BOOLEAN		bIncomplete;

	// Number of TARGET_DEVICE_INFO instances.
	//
	ULONG		DevInfoCount;

	// Bytes needed to get all of the data.
	//
	SIZE_T		ReqBufSize;

	// Each element describes a filtered device.
	//
	TARGET_DEVICE_INFO	TargDevVect[1];
} ACCESS_DEVICES_LIST, *PACCESS_DEVICES_LIST;