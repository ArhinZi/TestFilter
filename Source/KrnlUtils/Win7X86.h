// Declarations for functions specific to Win 7 on X86
//

//++
// Function:	FormatIoStackWin7X86
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
BOOL FormatIoStackWin7X86(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack);


//++
// Function:	FormatIrpWin7X86
//
// Description:
//
//	Formats the content of an IRP captured on a Windows 7 X86 system.
//
// Arguments:
//
// Return value: 
//--
BOOL FormatIrpWin7X86(
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
SIZE_T IoStackSizeWin7X86();




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
SIZE_T IrpSizeWin7X86();

