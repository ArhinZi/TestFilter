// Kernel helper functions and declarations
//

#if !defined(KRNLUTILS_INCLUDED)

#define KRNLUTILS_INCLUDED

#if defined(__cplusplus)

extern "C" {

#endif // #if defined(__cplusplus)

//----------------------------------------------------------------------

// Data types
// ----------

// Each value of this enumeration identifies a processor architecture.
//
typedef enum {
	ARCH_X86,
	ARCH_X64
} ARCHIT;

// Each value of this enumeration identifies a Windows version.
//
typedef enum {
	OSVER_XP,
	OSVER_VISTA,
	OSVER_WIN7
} OSVER;

//----------------------------------------------------------------------

// Functions
// ---------


//++
// Function:	FormatIoStack
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
BOOL FormatIoStack(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack, 
	ARCHIT		Arch, 
	OSVER		OsVer);


//++
// Function:	FormatIrp
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
BOOL FormatIrp(
	PCHAR		pIrpData,
	PWCHAR		*ppFIrp,
	ARCHIT		Arch,
	OSVER		OsVer);


//++
// Function:	FreeFmtIrp
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
VOID FreeFmtIrp(
	PWCHAR		pFIrp);


//++
// Function:	FreeFmtStack
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
VOID FreeFmtStack(
	PWCHAR		pFStack);


//++
// Function:	IoStackSize
//
// Description:
//
//	Returns the size of the IO_STACK_LOCATION structure for the 
//	specified processor architecture and Windows version.
//
// Arguments:
//
//
// Return value: 
//--
SIZE_T IoStackSize(
	ARCHIT		Arch, 
	OSVER		OsVer);


//++
// Function:	IrpSize
//
// Description:
//
//	Returns the size of the IRP structure for the 
//	specified processor architecture and Windows version.
//
// Arguments:
//
//
// Return value: 
//--
SIZE_T IrpSize(
	ARCHIT		Arch, 
	OSVER		OsVer);


//++
// Function:	MajorFunctionDes
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
LPWSTR	MajorFunctionDes(
	 UCHAR		MajorFunction,
	 PWSTR		pwszDecodeBuf,
	 SIZE_T		cbBufSize);


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
LPWSTR MinorFunctionDes(
	UCHAR		MajorFunction,
	UCHAR		MinorFunction,
	PWSTR		pwszDecodeBuf,
	SIZE_T		cbBufSize);


#if defined(__cplusplus)

}

#endif // #if defined(__cplusplus)



#endif // #if !defined(KRNLUTILS_INCLUDED)
