// COMP1521 19T2 assignment2
// Heap Management System in C 
// Written by Xingjian Li, z5190719, July/Aug 2019
// Written by XJ

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myHeap.h"

/** minimum total space for heap */
#define MIN_HEAP 4096
/** minimum amount of space for a free Chunk (excludes Header) */
#define MIN_CHUNK 32

#define ALLOC 0x55555555
#define FREE  0xAAAAAAAA

/// Types:
typedef unsigned int  uint;
typedef unsigned char byte;
typedef uintptr_t     addr; // an address as a numeric type

/** The header for a chunk. */
typedef struct header {
	uint status;    /**< the chunk's status -- ALLOC or FREE */
	uint size;      /**< number of bytes, including header */
	byte data[];    /**< the chunk's data -- not interesting to us */
} header;

/** The heap's state */
struct heap {
	void  *heapMem;     /**< space allocated for Heap */
	uint   heapSize;    /**< number of bytes in heapMem */
	void **freeList;    /**< array of pointers to free chunks */
	uint   freeElems;   /**< number of elements in freeList[] */
	uint   nFree;       /**< number of free chunks */
};

/// Variables:

/** The heap proper. */
static struct heap Heap;

/// Local Functions:
static addr heapMaxAddr (void);
static uint heap_size_regulation (int size);

/** Initialise the Heap. */
int initHeap (int size)
{
	Heap.heapSize = heap_size_regulation (size); // legal heap size
	
	// allocate region of memory of heap size
	// 1) Heap.heapMem points to the first byte of the allocated region
	// 2) zeroes out the entire region 
	Heap.heapMem = calloc(Heap.heapSize, sizeof(byte)); 
	if (Heap.heapMem == NULL)
	{
		return -1;
	}

	// initialise the region to be a single large free-space chunk
	Heap.nFree = 1;
	header *initial_chunk = (header *) Heap.heapMem;
	initial_chunk->status = FREE;
	initial_chunk->size = Heap.heapSize;

	// allocate freeList array
	Heap.freeList = calloc((Heap.heapSize / MIN_CHUNK), sizeof(header*)); 
	if (Heap.freeList == NULL)
	{
		return -1;
	}
	Heap.freeList[0] = initial_chunk;
	Heap.freeElems = 1;

	return 0; // on successful initialisation
}

// legal heap size is returned
static uint heap_size_regulation (int size)
{
	if (size < MIN_HEAP) // minimum heap size control (4096 bytes)
	{
		return MIN_HEAP;
	}
	
	switch (size % 4) // round UP to the nearest multiple of 4
	{
		case 3: 
			return size + 1;
		case 2: 
			return size + 2;
		case 1:
			return size + 3;
		default: // multiple of 4
			return size;
	}
}

/** Release resources associated with the heap. */
void freeHeap (void)
{
	free (Heap.heapMem);
	free (Heap.freeList);
}

/** Allocate a chunk of memory large enough to store `size' bytes. */
void *myMalloc (int size)
{
	/// TODO ///

	return NULL; // this just keeps the compiler quiet
}

/** Deallocate a chunk of memory. */
void myFree (void *obj)
{
	/// TODO ///
}

/** Return the first address beyond the range of the heap. */
static addr heapMaxAddr (void)
{
	return (addr) Heap.heapMem + Heap.heapSize;
}

/** Convert a pointer to an offset in the heap. */
int heapOffset (void *obj)
{
	addr objAddr = (addr) obj;
	addr heapMin = (addr) Heap.heapMem;
	addr heapMax =        heapMaxAddr ();
	if (obj == NULL || !(heapMin <= objAddr && objAddr < heapMax))
		return -1;
	else
		return (int) (objAddr - heapMin);
}

/** Dump the contents of the heap (for testing/debugging). */
void dumpHeap (void)
{
	int onRow = 0;

	// We iterate over the heap, chunk by chunk; we assume that the
	// first chunk is at the first location in the heap, and move along
	// by the size the chunk claims to be.
	addr curr = (addr) Heap.heapMem;
	while (curr < heapMaxAddr ()) {
		header *chunk = (header *) curr;

		char stat;
		switch (chunk->status) {
		case FREE:  stat = 'F'; break;
		case ALLOC: stat = 'A'; break;
		default:
			fprintf (
				stderr,
				"myHeap: corrupted heap: chunk status %08x\n",
				chunk->status
			);
			exit (1);
		}

		printf (
			"+%05d (%c,%5d)%c",
			heapOffset ((void *) curr),
			stat, chunk->size,
			(++onRow % 5 == 0) ? '\n' : ' '
		);

		curr += chunk->size;
	}

	if (onRow % 5 > 0)
		printf ("\n");
}
