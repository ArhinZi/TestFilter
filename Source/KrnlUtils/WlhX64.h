// Declarations for functions specific to Win Vista on X64
//


//++
// Function:	FormatIoStackWlhX86
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
BOOL FormatIoStackWlhX64(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack);


//++
// Function:	FormatIrpWlhX64
//
// Description:
//
//	Formats the content of an IRP captured on a Vista X64 system.
//
// Arguments:
//
// Return value: 
//--
BOOL FormatIrpWlhX64(
	PCHAR		pIrpData, 
	PWCHAR		*ppFIrp);



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
SIZE_T IoStackSizeWlhX64();


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
SIZE_T IrpSizeWlhX64();
