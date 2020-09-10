// KernelUtils declarations non visible to clients
//


// Ids of the possible architectures, used in precompiler directives
// (e. g. #if)
//
#define MACRO_ARCH_X86	1
#define MACRO_ARCH_X64	2

// Ids of the possible OS versions, used in precompiler directives
// (e. g. #if)
//
#define	MACRO_OSVER_XP		1
#define	MACRO_OSVER_LH		2
#define	MACRO_OSVER_WIN7	3


#define FMTIRP_INITIAL_BUF_SIZE 1024
#define FMTIRP_ITEM_BUF_SIZE	101

//++
// Function:	AllocFromHeap
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
PVOID AllocFromHeap(
	HANDLE		hHeap, 
	SIZE_T		BufSize);


//++
// Function:	
//
// Description:
//
//	Appends a null terminated string to the end of the buffer.
//
//	Updates *pcbOffset to point past the end of the string.
//
//	Extends the buffer if needed, updating *pcbBufSize an *ppFmtBuf.
//
// Arguments:
//
//
// Return value: 
//--
BOOL AppendString(
	PWSTR		wszItem, 
	PCHAR*		ppFmtBuf, 
	SIZE_T*		pcbOffset, 
	SIZE_T*		pcbBufSize, 
	HANDLE		hHeap);


//++
// Function:	CopyToBuffer
//
// Description:
//
//	Copies the data block pointed by pSource into the buffer pointed by
//	*ppBuffer. cbLen gives the length of the data to copy.
//
//	The data is copied at the address *ppBuffer + *pcbOffset. *pcbOffset
//	is incremented by cbLen.
//
//	The total size of the buffer is given by *pcbBufSize. If there's not
//	enough room in the buffer, a new buffer is allocated, the content
//	of the old one is copied into the new one and the old buffer is
//	freed.
//
//	*ppBuffer and *pcbBufSize are updated for the new buffer.
//
// Arguments:
//
//
// Return value: 
//--
BOOL CopyToBuffer(
	PCHAR*		ppBuffer, 
	SIZE_T*		pcbOffset,
	PCHAR		pSource, 
	SIZE_T		cbLen, 
	SIZE_T*		pcbBufSize, 
	HANDLE		hHeap);


//++
// Function:	DecodeEnum
//
// Description:
//
//
// Arguments:
//
//
// Return value: 
//--
PWSTR DecodeEnum(
	INT			Value, 
	INT			Base,
	PWSTR		pwszDecodeV[],
	UINT		VectLen,
	PWSTR		pwszOutBuf, 
	SIZE_T		cbBufSize);


//++
// Function:	ExtendBuffer
//
// Description:
//
//	Allocates a new buffer and copies the content of the old one.
//
// Arguments:
//
//
// Return value: 
//--
BOOL ExtendBuffer(
	PCHAR*		ppBuffer, 
	SIZE_T*		pcbBufSize, 
	SIZE_T		cbNeeded, 
	HANDLE		hHeap);
