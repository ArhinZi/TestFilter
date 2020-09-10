// Circular buffer functions
//

//----------------------------------------------------------------------

// Defines
// -------

// Macro to define NTSTATUS values
//
#define MAKE_NTSTATUS(sev, fac, code) \
	(((sev) & 0x3) << 30) + \
	(1 << 29) + \
	(((fac) & 0x1fff) << 16) + \
	((code) & 0xffff)

// Custom NTSTATUS values
//
#define CB_STATUS_WARN_DATA_LOST	\
	MAKE_NTSTATUS(STATUS_SEVERITY_WARNING, 0, 1)


//----------------------------------------------------------------------

// Data types
// ----------
//

// Buffer control variables.
//
typedef struct _CIRCULAR_BUFFER {

	// Buffer start address
	//
	PCHAR		pBufStartAddr;

	// Buffer end address (address of last the buffer byte + 1)
	//
	PCHAR		pBufEndAddr;

	// Buffer size
	//
	SIZE_T		cbBufSize;

	// Data start address
	//
	PCHAR		pDataStart;

	// Data end address (address of next free byte)
	//
	PCHAR		pDataEnd;

	// Address of the current data block (the one into which
	// the storing functions write.
	//
	PCHAR		pCurrentBlock;

	// Free bytes available
	//
	SIZE_T		cbAvailable;

} CIRCULAR_BUFFER, *PCIRCULAR_BUFFER;





//----------------------------------------------------------------------

// Function prototypes
// -------------------
//


//++
// Function:	CBAllocateBlock
//
// Description:
//
//		Allocates a data block into the buffer.
//
//		If the buffer does not have enough free space, releases data 
//		from the block until the data block can be allocated.
//
//		If data is released, it is discarded in whole blocks.
//
//		If this function needs to discard data blocks, it does so
//		starting from the oldest ones.
//
//		If the block requested size is larger than the maximum amount 
//		that can fit into the buffer when it's empty, an error is 
//		returned. This is the only possible error condition.
//
// Arguments:
//
//		pBufCtl - address of the CIRCULAR_BUFFER structure with the 
//			buffer control variables.
//
//		cbBlockSize - requested size of the data block.
//
// Return value: 
//
//		STATUS_NO_MEMORY - the requested size is greater than the
//			maximum possible size.
//
//		CB_WARN_DATA_LOST - data has been discarded to make room
//			for the block.
//
//		STATUS_SUCCESS - block allocated, no data discarded.
//--
NTSTATUS CBAllocateBlock(
	PCIRCULAR_BUFFER	pBufCtl,
	SIZE_T				cbBlockSize	);


//++
// Function:	CBDataBufferEmpty
//
// Description:
//
//		Returns TRUE if the buffer contains no data.
//
// Arguments:
//
//		pBufCtl - Address of the buffer control structure.
//
// Return value: TRUE if the buffer is empty, false otherwise.
//--
BOOLEAN CBDataBufferEmpty(
	PCIRCULAR_BUFFER		pBufCtl);


//++
// Function:	CBFreeOldestDataBlock
//
// Description:
//
//		Discards the oldest data block in the buffer.
//
// Arguments:
//
//		pBufCtl - address of the buffer control structure.
//
// Return value: 
//--
VOID CBFreeOldestDataBlock(
	PCIRCULAR_BUFFER	pBufCtl);


//++
// Function:	CBInitBufferCtlVar
//
// Description:
//
//		Initializes the variables controlling a circular buffer. Stores 
//		the buffer address and length and sets the buffer as containing 
//		no data.
//
// Arguments:
//
//		pBufCtl - address of the CIRCULAR_BUFFER structure with the 
//			buffer control variables.
//
//		pBufAddr - address of the buffer memory area.
//
//		cbBufSize - length in bytes of the buffer memory area.
//
//
// Return value: none.
//--
VOID CBInitBufferCtlVar(
	PCIRCULAR_BUFFER		pBufCtl, 
	PVOID					pBufAddr,
	SIZE_T					cbBufSize);


//++
// Function:	CBMovePtrForward
//
// Description:
//
//		Moves a pointer forward inside the buffer address range of the
//		specified number of bytes. If the pointer passes the buffer
//		end, it wraps it to the beginning.
//
// Arguments:
//
//		pBufCtl - buffer control structure.
//
//		ppAddress - address of the pointer to be moved forward.
//
//		cbBytes - number of bytes to move the pointer forward.
//
// Return value: 
//
//		STATUS_INVALID_PARAMETER - ppAddress is NULL, or the *ppAddress
//			is not inside the buffer address range.
//
//		STATUS_SUCCESS - Ok.
//--
NTSTATUS CBMovePtrForward(
	PCIRCULAR_BUFFER		pBufCtl, 
	PCHAR					*ppAddress, 
	SIZE_T					cbBytes);


//++
// Function:	CBStore
//
// Description:
//
//		Stores a block of bytes into the current data block, which is
//		the one allocated by the last call to CBAllocateBlock.
//
//		If the amount of byte to store is greater than the data block
//		length, the data is truncated. This function never touches
//		buffer memory outside the current data block.
//
//		The cbOffset parameter specifies an offset into the current
//		block for the store.
//
//		Data can also be truncated when this offset is such that part of 
//		the data falls outside of the current block.
//
//		If the buffer is empty, i. e., there is no current data block,
//		this function returns an error.
//
//		Updates the pointers and offsets so that they can be reused
//		on a new call to this same function.
//
// Arguments:
//
//		pBuf - address of the buffer control variables.
//
//		ppData - address of a pointer which stores the address of the
//			data to be stored.
//
//			On exit, *ppData is set to the initial value + the number
//			of bytes stored.
//
//		pcbLen - address of a variable which holds the amount of byte
//			to store. On exit it is set to the initial value minus
//			the number of bytes actually stored.
//
//		pcbOffset - address of a variable storing the offset into the
//			current block for the store. On exit it's set to the initial
//			value plust the number of byte stored.
//
//		pcbBytesStored - address of a avariable which will receive the
//			number of bytes actually stored. This parameter can
//			be NULL.
//
// Return value:
//
//		STATUS_NO_MEMORY - There is no current data block, i. e., the
//			buffer is empty.
//
//		STATUS_INVALID_PARAMETER - *pcbOffset is greater or equal
//			the current block size
//
//		CB_STATUS_WARN_DATA_LOST - only part of the data was stored, 
//			because it would not fit into the current block.
//
//		STATUS_SUCCESS - all of the data was stored.
//
//--
NTSTATUS CBStore(
	PCIRCULAR_BUFFER	pBuf, 
	PCHAR				*ppData,
	SIZE_T				*pcbLen,
	SIZE_T				*pcbOffset,
	SIZE_T				*pcbBytesStored);


//++
// Function:	CBXtractFromStart
//
// Description:
//
//		Extracts data from the oldest data block in the buffer.
//
//		Extracting the data does not discard them from the buffer. It is
//		possible to call this function multiple times to xtract the same
//		data.
//
//		To discard the data, the entire data block must be discarded by
//		calling CBFreeOldestDataBlock().
//
//
// Arguments:
//
//	pBufCtl - address of the buffer control data structure.
//
//	pcbXtractOffset - address of a variable storing the offset into
//		the data block at which the extraction begins.
//		On exit, set to initial value plust number of bytes extracted.
//
//	ppDest - address of a variable storing the address of the 
//		destination buffer.
//
//		This function will add to *ppDest the number of bytes returned,
//		so that, on exit, its value will be the address of the next
//		free byte (or of the first byte past the buffer end).
//
//	pcbDestFree  - address of a variable which stores the number of
//		bytes that can be written into the destination buffer. 
//		This function will update it, subtracting from it the number of 
//		bytes returned. 
//
//		If the variable value is smaller than the amount of data 
//		requested by *pcbBytesToRead, only *pcbDestFree bytes are 
//		written, i. e., this function guarantees not to write past the 
//		end of the buffer. In this case, *pcbDestFree will be set to 0 
//		on 	exit.
//
//	pcbBytesToRead - address of a variable storing the number of bytes 
//		to return.
//
//		If pcbBytesToRead = NULL, all the bytes up to the end of 
//		the data block are returned.
//
//		No matter what the value of pcbBytesRead is, this function never
//		writes more than *pcbDestFree bytes at the destination address.
//
//		If pcbBytesToRead != NULL, *pcbBytesToRead on exit it is set to 
//		the initial value minus the number of bytes actually returned.
//
//		*pcbBytesToRead can be greater than the data block length 
//		and, if so, only the valid amount of data is returned.
//
//	pcbBytesRead - address of a variable which receives the number of
//		bytes actually written at the destination address. Can be NULL.
//
// Return value: 
//
//		STATUS_INVALID_PARAMETER - the buffer is empty (there's no data
//			block to read from).
//
//			*pcbXtractOffset is >= the size of the data block, which
//			is not allowed, since it represents an offset into the 
//			block.
//
//		STATUS_SUCCESS - success.
//--
NTSTATUS CBXtractFromStart(
	PCIRCULAR_BUFFER	pBufCtl,
	SIZE_T				*pcbXtractOffset,
	PCHAR				*ppDest,
	SIZE_T				*pcbDestFree,
	SIZE_T				*pcbBytesToRead,
	SIZE_T				*pcbBytesRead);
