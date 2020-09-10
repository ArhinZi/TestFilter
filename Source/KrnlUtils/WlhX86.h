// Declarations for functions specific to Win Vista on X86
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
BOOL FormatIoStackWlhX86(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack);


//++
// Function:	FormatIrpWlhX86
//
// Description:
//
//	Formats the content of an IRP captured on a Vista X86 system.
//
// Arguments:
//
// Return value: 
//--
BOOL FormatIrpWlhX86(
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
SIZE_T IoStackSizeWlhX86();


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
SIZE_T IrpSizeWlhX86();