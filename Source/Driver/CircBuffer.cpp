// Circular buffer functions
//


//----------------------------------------------------------------------

// Includes
// --------

extern "C" {
#include <NTDDK.h>
#include <ntstrsafe.h>
}

#include "CircBuffer.h"


//----------------------------------------------------------------------

// Data types
// ----------

// Header of a buffer data block.
//
typedef struct _CB_BLOCK_HEADER {

	// Size of the data block, not counting the header itself. This is
	// the size seen from the client. The block takes up cbSize + 
	// sizeof(CB_BLOCK_HEADER) bytes from the buffer.
	//
	SIZE_T		cbSize;

} CB_BLOCK_HEADER, *PCB_BLOCK_HEADER;


//----------------------------------------------------------------------

// Function definitions
// --------------------


//----------------------------------------------------------------------
NTSTATUS CBAllocateBlock(
	PCIRCULAR_BUFFER	pBufCtl,
	SIZE_T				cbBlockSize	)
{
	SIZE_T cbBytesNeeded = cbBlockSize + sizeof(CB_BLOCK_HEADER);

	// Ensure that the block can fit into the buffer.
	//
	if (cbBytesNeeded > pBufCtl->cbBufSize) {
		return STATUS_NO_MEMORY;
	}
	NTSTATUS status = STATUS_SUCCESS;

	// Discard data if the available space is not enough.
	//
	while (pBufCtl->cbAvailable < cbBytesNeeded) {
		CBFreeOldestDataBlock(pBufCtl);
		status = CB_STATUS_WARN_DATA_LOST;
	}
	PCHAR pBlockStart = pBufCtl->pDataEnd;

	// Note on pDataStart (buffer ctl structure): if the buffer is not 
	// empty, the new block is not the beginning of the data.
	// pDataStart already points to the beginning of the data and needs 
	// not to be updated.
	//
	// If the buffer is empty, the new block is also the beginning
	// of the data and pDataStart must point to it, however, given that 
	// for an empty buffer pDataStart = pDataEnd, it already has the 
	// correct address.
	//
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"CB - CBAllocateBlock - Allocating block: start %#p, length "
		"(not counting hdr) %#x bytes\n", pBlockStart, cbBlockSize);

	CBMovePtrForward(pBufCtl, &pBufCtl->pDataEnd, cbBytesNeeded);
	pBufCtl->cbAvailable -= cbBytesNeeded;
	pBufCtl->pCurrentBlock = pBlockStart;

	// Write the data block header at the block starting address
	//
	CB_BLOCK_HEADER blockHdr = {0};
	blockHdr.cbSize = cbBlockSize;
	PCHAR pHdr = (PCHAR) &blockHdr;
	for (INT i = 0; i < sizeof blockHdr; i++) {
		*pBlockStart = *(pHdr + i);
		CBMovePtrForward(pBufCtl, &pBlockStart, 1);
	}
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"CB - CBAllocateBlock - Bytes remaining: %#x\n",
		pBufCtl->cbAvailable);

	return status;
}


//----------------------------------------------------------------------
BOOLEAN CBDataBufferEmpty(
	PCIRCULAR_BUFFER		pBufCtl)
{
	return (pBufCtl->pDataStart == pBufCtl->pDataEnd);
}


//----------------------------------------------------------------------
VOID CBFreeOldestDataBlock(
	PCIRCULAR_BUFFER	pBufCtl)
{
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"CB - CBFreeOldestDataBlock - Avail bytes before: %#x\n",
		pBufCtl->cbAvailable);
	CB_BLOCK_HEADER OldestHdr;

	// Extract the block header. Keep in mind that it can wrap
	// around the end.
	//
	PCHAR pHdrVar = (PCHAR) &OldestHdr;
	PCHAR pHdr = pBufCtl->pDataStart;
	for (INT i = 0; i < sizeof OldestHdr; i++) {
		*(pHdrVar + i) = *pHdr;
		CBMovePtrForward(pBufCtl, &pHdr, 1);
	}

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"CB - CBFreeOldestDataBlock - Discarding block: start %#p, "
		"length (not counting "
		"hdr) %#x bytes\n", pBufCtl->pDataStart, OldestHdr.cbSize);

	SIZE_T cbBytesFreed = OldestHdr.cbSize + sizeof (CB_BLOCK_HEADER);
	CBMovePtrForward(pBufCtl, &pBufCtl->pDataStart, cbBytesFreed);
	pBufCtl->cbAvailable += cbBytesFreed;
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"CB - CBFreeOldestDataBlock - Avail bytes after:  %#x\n",
		pBufCtl->cbAvailable);
	return;
}



//----------------------------------------------------------------------
VOID CBInitBufferCtlVar(
	PCIRCULAR_BUFFER		pBufCtl, 
	PVOID					pBufAddr,
	SIZE_T					cbBufSize)
{
	pBufCtl->pBufStartAddr = (PCHAR) pBufAddr;
	pBufCtl->pBufEndAddr = ((PCHAR) pBufAddr) + cbBufSize;
	pBufCtl->cbBufSize = cbBufSize;
	pBufCtl->pDataStart = (PCHAR) pBufAddr;
	pBufCtl->pDataEnd = (PCHAR) pBufAddr;
	pBufCtl->cbAvailable = cbBufSize;

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"CB - CBInitBufferCtlVar - Buffer initialized\n");
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"    start:    %#p\n", pBufCtl->pBufStartAddr);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"    end:      %#p\n", pBufCtl->pBufEndAddr);
	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"    size:     %#x\n", pBufCtl->cbBufSize);

}


//----------------------------------------------------------------------
NTSTATUS CBMovePtrForward(
	PCIRCULAR_BUFFER		pBufCtl, 
	PCHAR					*ppAddress, 
	SIZE_T					cbBytes)
{
	if (ppAddress == NULL) return STATUS_INVALID_PARAMETER;

	if ((*ppAddress < pBufCtl->pBufStartAddr) ||
		(*ppAddress >= pBufCtl->pBufEndAddr)) 
		return STATUS_INVALID_PARAMETER;

	*ppAddress += cbBytes;
	if (*ppAddress >= pBufCtl->pBufEndAddr) {
		SIZE_T cbOverflow = *ppAddress - 
			pBufCtl->pBufEndAddr;
		// cbOverflow is the number of bytes that would fall past the 
		// end of the buffer.

		*ppAddress = pBufCtl->pBufStartAddr + cbOverflow;
	}
	return STATUS_SUCCESS;
}



//----------------------------------------------------------------------
NTSTATUS CBStore(
	PCIRCULAR_BUFFER	pBuf, 
	PCHAR				*ppData,
	SIZE_T				*pcbLen,
	SIZE_T				*pcbOffset,
	SIZE_T				*pcbBytesStored)
{
	if (CBDataBufferEmpty(pBuf)) return STATUS_NO_MEMORY;

	CB_BLOCK_HEADER curBlockHdr;

	// Extract the current block header. Advance a pointer into
	// the buffer with CBMovePtrForward, because the header can cross
	// the buffer end.
	//
	PCHAR pHdrVar = (PCHAR) &curBlockHdr;
	PCHAR pHdr = pBuf->pCurrentBlock;
	for (INT i = 0; i < sizeof curBlockHdr; i++) {
		*(pHdrVar + i) = *pHdr;
		CBMovePtrForward(pBuf, &pHdr, 1);
	}

	if (*pcbOffset >= curBlockHdr.cbSize) 
		return STATUS_INVALID_PARAMETER;
	// Compute the store address skipping the header and accounting for
	// the offset. Use CBMovePtrForward because we might be crossing
	// the buffer end.
	//
	PCHAR pStoreAddr = pBuf->pCurrentBlock;
	CBMovePtrForward(pBuf, &pStoreAddr, sizeof (CB_BLOCK_HEADER) +
		*pcbOffset);
	SIZE_T cbMaxStoreLen = curBlockHdr.cbSize - *pcbOffset;
	SIZE_T cbStoreLen = 
		*pcbLen < cbMaxStoreLen ? *pcbLen : cbMaxStoreLen ;


	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		     "CB - CBStore - Storing.\n"
	         "    Current block start:       %#p\n"
			 "    length (not counting hdr): %#x\n"
			 "    store addr:                %#p\n"
			 "    store length:              %#x\n"
			 "    src addr:                  %#p\n",
		pBuf->pCurrentBlock,
		curBlockHdr.cbSize, 
		pStoreAddr, 
		cbStoreLen, 
		*ppData);

	// Store the data, accounting for the fact that it may wrap around
	// the buffer end.
	//
	// cbContiguous = length of the contiguous data area from pStoreAddr
	// up to the buffer end.
	//
	SIZE_T cbContiguous = pBuf->pBufEndAddr - pStoreAddr;
	SSIZE_T block2ndChunkLen = cbStoreLen - cbContiguous;

	// Note: if there are other data blocks before the buffer end 
	// address, cbContiguous > cbStoreLen for sure, because we have 
	// already limited cbStoreLen inside the current block.

	if (block2ndChunkLen > 0) {

		// The data wraps around => two data tansfers.
		//
		RtlCopyMemory(pStoreAddr, *ppData, cbContiguous);
		RtlCopyMemory(pBuf->pBufStartAddr, (*ppData) + cbContiguous,
			block2ndChunkLen);
	} else {
		RtlCopyMemory(pStoreAddr, *ppData, cbStoreLen);
	}
	*ppData += cbStoreLen;
	*pcbLen -= cbStoreLen;
	*pcbOffset += cbStoreLen;
	if (pcbBytesStored != NULL) *pcbBytesStored = cbStoreLen;

	if (cbStoreLen < *pcbLen) return CB_STATUS_WARN_DATA_LOST;

	return STATUS_SUCCESS;
}


//----------------------------------------------------------------------
NTSTATUS CBXtractFromStart(
	PCIRCULAR_BUFFER	pBufCtl,
	SIZE_T				*pcbXtractOffset,
	PCHAR				*ppDest,
	SIZE_T				*pcbDestFree,
	SIZE_T				*pcbBytesToRead,
	SIZE_T				*pcbBytesRead)
{
	if (CBDataBufferEmpty(pBufCtl)) {
		return STATUS_INVALID_PARAMETER;
	}

	// Check that the offset is valid.
	//
	CB_BLOCK_HEADER blkHdr;

	// Extract the block header. The pointer into the buffer is
	// advanced calling CBMovePtrForward to account for the fact that 
	// the header could wrap around the buffer end.
	//
	PCHAR pHdrVar = (PCHAR) &blkHdr;
	PCHAR pHdr = pBufCtl->pDataStart;
	for (INT i = 0; i < sizeof blkHdr; i++) {
		*(pHdrVar + i) = *pHdr;
		CBMovePtrForward(pBufCtl, &pHdr, 1);
	}

	if (*pcbXtractOffset >= blkHdr.cbSize) 
		return STATUS_INVALID_PARAMETER;
	// Compute the amount of data requested. cbXLen will be the 
	// extraction length, cbAvailLen the available bytes from
	// the offset to the block end.
	//
	SIZE_T cbXLen;
	SIZE_T cbAvailLen = blkHdr.cbSize - *pcbXtractOffset;
	if (pcbBytesToRead == NULL) {

		// NULL means up to the end of the block.
		//
		cbXLen = cbAvailLen;
	} else if (*pcbBytesToRead > cbAvailLen) {
		cbXLen = cbAvailLen;
	} else {
		cbXLen = *pcbBytesToRead;
	}

	// Limit the amount of data based on *pcbDestFree.
	//
	cbXLen = (cbXLen < *pcbDestFree ? cbXLen : *pcbDestFree);

	// Compute the start address skipping the header and taking the
	// offset into account. Use CBMovePtrForward(), because we might
	// be crossing the end of the buffer.
	//
	PCHAR pXStart = pBufCtl->pDataStart;
	CBMovePtrForward(pBufCtl, &pXStart, sizeof (CB_BLOCK_HEADER) +
		*pcbXtractOffset);

	DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL,
		"CB - CBXtractFromStart - Xtracting.\n"
		"    current block start:       %#p\n"
		"    length (not counting hdr): %#x\n"
		"    source:                    %#p\n"
		"    dest:                      %#p\n"
		"    len:                       %#x\n", 
		pBufCtl->pDataStart, 
		blkHdr.cbSize,
		pXStart,
		*ppDest,
		cbXLen);

	// Return the data, accounting for the fact that it may wrap around
	// the buffer end.
	//
	// cbContiguous = length of the contiguous data area from pXStart up
	// to the buffer end.
	//
	SIZE_T cbContiguous = pBufCtl->pBufEndAddr - pXStart;
	SSIZE_T block2ndChunkLen = cbXLen - cbContiguous;

	// Note: if there are other data blocks before the buffer end 
	// address, cbContiguous > cbXLen for sure, because we have already
	// limited cbXLen inside the current block.

	if (block2ndChunkLen > 0) {

		// The data wraps around => two data tansfers.
		//
		RtlCopyMemory(*ppDest, pXStart, cbContiguous);
		RtlCopyMemory((*ppDest) + cbContiguous, pBufCtl->pBufStartAddr,
			block2ndChunkLen);
	} else {
		RtlCopyMemory(*ppDest, pXStart, cbXLen);
	}
	*pcbXtractOffset += cbXLen;
	*ppDest += cbXLen;
	*pcbDestFree -= cbXLen;
	if (pcbBytesToRead != NULL) *pcbBytesToRead -= cbXLen;
	if (pcbBytesRead != NULL) *pcbBytesRead = cbXLen;
	return STATUS_SUCCESS;
}
