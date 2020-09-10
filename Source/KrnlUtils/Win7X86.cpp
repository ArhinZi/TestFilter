// Functions specific to Windows 7 on X64.
//
#include <windows.h>

#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#include <strsafe.h>
#pragma warning(pop)


#include "KrnlUtils.h"
#include "KUPrivate.h"

#define ARCHITECTURE	MACRO_ARCH_X86
#define	CST_OSVER		MACRO_OSVER_WIN7
#include "KrnlDefs.h"
#include "Win7X86.h"

#include "PlatfDepCompile.cpp"

//----------------------------------------------------------------------
BOOL FormatIoStackWin7X86(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack)
{
	return FormatIoStackGeneric(
		pStackData,
		ppFStack);
}



//----------------------------------------------------------------------
BOOL FormatIrpWin7X86(
	PCHAR		pIrpData, 
	PWCHAR		*ppFIrp)
{
	return FormatIrpGeneric(
		pIrpData,
		ppFIrp);
}


//----------------------------------------------------------------------
SIZE_T IoStackSizeWin7X86()
{
	return sizeof (IO_STACK_LOCATION);
}



//----------------------------------------------------------------------
SIZE_T IrpSizeWin7X86()
{
	return sizeof (IRP);
}
