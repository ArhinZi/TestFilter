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

#define ARCHITECTURE	MACRO_ARCH_X86
#define	CST_OSVER		MACRO_OSVER_XP
#include "KrnlDefs.h"
#include "WxpX86.h"

#include "PlatfDepCompile.cpp"


//----------------------------------------------------------------------
BOOL FormatIoStackWxpX86(
	PCHAR		pStackData, 
	PWCHAR*		ppFStack)
{
	return FormatIoStackGeneric(
		pStackData,
		ppFStack);
}


//----------------------------------------------------------------------
BOOL FormatIrpWxpX86(
	PCHAR		pIrpData, 
	PWCHAR		*ppFIrp)
{
	return FormatIrpGeneric(
		pIrpData,
		ppFIrp);
}


//----------------------------------------------------------------------
SIZE_T IoStackSizeWxpX86()
{
	return sizeof (IO_STACK_LOCATION);
}


//----------------------------------------------------------------------
SIZE_T IrpSizeWxpX86()
{
	return sizeof (IRP);
}