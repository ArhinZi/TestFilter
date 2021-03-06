// Functions specific to Windows XP on X86.
//
#include <windows.h>

#include <CodeAnalysis/warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#include <strsafe.h>
#pragma warning(pop)


#include "KrnlUtils.h"
#include "KUPrivate.h"

#define ARCHITECTURE	MACRO_ARCH_X64
#define	CST_OSVER		MACRO_OSVER_LH
#include "KrnlDefs.h"
#include "WlhX64.h"

#include "PlatfDepCompile.cpp"

//----------------------------------------------------------------------
BOOL FormatIoStackWlhX64(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack)
{
	return FormatIoStackGeneric(
		pStackData,
		ppFStack);
}


//----------------------------------------------------------------------
BOOL FormatIrpWlhX64(
	PCHAR		pIrpData, 
	PWCHAR		*ppFIrp)
{
	return FormatIrpGeneric(
		pIrpData,
		ppFIrp);
}


//----------------------------------------------------------------------
SIZE_T IoStackSizeWlhX64()
{
	return sizeof (IO_STACK_LOCATION);
}


//----------------------------------------------------------------------
SIZE_T IrpSizeWlhX64()
{
	return sizeof (IRP);
}