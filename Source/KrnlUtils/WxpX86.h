// Declarations for functions specific to Win XP on X86
//


//++
// Function:	FormatIoStackWxpX86
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
BOOL FormatIoStackWxpX86(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack);


//++
// Function:	FormatIrpWxpX86
//
// Description:
//
//	Formats the content of an IRP captured on XP X86 system.
//
// Arguments:
//
// Return value: 
//--
BOOL FormatIrpWxpX86(
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
SIZE_T IoStackSizeWxpX86();


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
SIZE_T IrpSizeWxpX86();