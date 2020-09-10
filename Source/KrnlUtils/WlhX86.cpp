// Functions specific to Windows Vista on X86.
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
#define	CST_OSVER		MACRO_OSVER_LH
#include "KrnlDefs.h"
#include "WlhX86.h"

#include "PlatfDepCompile.cpp"


//----------------------------------------------------------------------
BOOL FormatIoStackWlhX86(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack)
{
	return FormatIoStackGeneric(
		pStackData,
		ppFStack);
}


//----------------------------------------------------------------------
BOOL FormatIrpWlhX86(
	PCHAR		pIrpData, 
	PWCHAR		*ppFIrp)
{
	return FormatIrpGeneric(
		pIrpData,
		ppFIrp);
}


//----------------------------------------------------------------------
SIZE_T IoStackSizeWlhX86()
{
	return sizeof (IO_STACK_LOCATION);
}


//----------------------------------------------------------------------
SIZE_T IrpSizeWlhX86()
{
	return sizeof (IRP);
}