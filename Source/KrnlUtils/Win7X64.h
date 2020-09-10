// Declarations for functions specific to Win 7 on X64
//

//++
// Function:	FormatIoStackWin7X64
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
BOOL FormatIoStackWin7X64(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack);


//++
// Function:	FormatIrpWin7X64
//
// Description:
//
//	Formats the content of an IRP captured on a Windows 7 X64 system.
//
// Arguments:
//
// Return value: 
//--
BOOL FormatIrpWin7X64(
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
SIZE_T IoStackSizeWin7X64();




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
SIZE_T IrpSizeWin7X64();

