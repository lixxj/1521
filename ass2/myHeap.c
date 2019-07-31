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

#define SUCCESS        0
#define FAILURE       -1

#define SET        	   1
#define CLEAR          0

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
	uint   freeElems;   /**< total number of elements in freeList[] */
	uint   nFree;       /**< number of free chunks */
};

/// Variables:

/** The heap proper. */
static struct heap Heap;

/// Local Functions:
static addr heapMaxAddr (void);
/// Additional helper functions:
static uint next_multiple_of_4 (int size);
static uint heap_size_regulation (int size);
static header *smallest_free_chunk_larger_than_size (int size);
static void insert_to_freeList (header *target_chunk);
static void delete_from_freeList (header *target_chunk);
static int binary_search (header *target, int low, int high, void *freeList[]);
static void adjchunks_merge (void);

/** Initialise the Heap. */
int initHeap (int size)
{
	Heap.heapSize = heap_size_regulation (size); // legal heap size
	
	// allocate region of memory of heap size 
	// 1) Heap.heapMem points to the first byte of the allocated region 
	// 2) zeroes out the entire region 
	Heap.heapMem = calloc(Heap.heapSize, sizeof(byte)); 
	if (Heap.heapMem == NULL) // fail to allocate heap
	{ 
		//fprintf(stderr, "Fatal: failed to allocate heap of %d bytes.\n", Heap.heapSize);
		return FAILURE;
   	}
	
	// initialise the region to be a single large free-space chunk
	Heap.nFree = 1;
	header *initial_chunk = (header *) Heap.heapMem;
	initial_chunk->status = FREE;
	initial_chunk->size = Heap.heapSize;

	// allocate freeList ARRAY
	Heap.freeElems = (Heap.heapSize / MIN_CHUNK);
	Heap.freeList = calloc(Heap.freeElems, sizeof(header*)); 
	if (Heap.freeList == NULL) // fail to allocate freeList
	{
		//fprintf(stderr, "Fatal: failed to allocate freeList of %u bytes.\n", Heap.freeElems);
		if (Heap.heapMem != NULL) free(Heap.heapMem); // avoid memory leak
		return FAILURE; 
	}
	Heap.freeList[0] = initial_chunk;

	return SUCCESS; // on successful initialisation
}

/** Allocate a chunk of memory large enough to store `size' bytes. */
void *myMalloc (int size)
{
	// exception: NULL size
	if (size < 1) return NULL;
	
	int chunk_size = next_multiple_of_4 (size) + sizeof(header);
	// obtain the smallest free chunk larger than or equals to legal malloc size
	header *sfchunk = smallest_free_chunk_larger_than_size (chunk_size);
	if (sfchunk == NULL) return NULL; // no chunk is available
	
	// next_multiple_of_4(N) + HeaderSize + MIN_CHUNK
	int split_entry = chunk_size + MIN_CHUNK; 

	if (sfchunk->size < split_entry) // no chunk split
	{
		sfchunk->status = ALLOC; // allocate the whole chunk
		delete_from_freeList (sfchunk); // delete allocated chunk from freeList
	}
	
	if (sfchunk->size >= split_entry) // chunk split 
	{
		delete_from_freeList (sfchunk); // delete whole chunk from freeList
		// the upper chunk become a new free chunk
		addr nfchunk_start = (addr)sfchunk + chunk_size;
		header *nfchunk = (header *)nfchunk_start;
		nfchunk->status = FREE;
		nfchunk->size = sfchunk->size - chunk_size;
		insert_to_freeList (nfchunk);
		// the lower chunk allocated for the request
		sfchunk->status = ALLOC;
		sfchunk->size = chunk_size;		
	}
	// return a pointer to the first usable byte of data in the chunk
	return (sfchunk + 1);
}

/** Deallocate a chunk of memory. */
void myFree (void *obj)
{
	if (obj == NULL) return; // exception: free(NULL)

	header *chunktbf = (header*)obj - sizeof(header);

	// an valid obj must be a pointer to the start of the data block in an allocated chunk
	if (chunktbf->status != ALLOC) // invalid obj  
	{
		fprintf(stderr, "Attempt to free unallocated chunk\n");
		exit(1);
	}

	chunktbf->status = FREE;
	insert_to_freeList (chunktbf);
	adjchunks_merge();
}

////////////////////////////
///// HELPER FUNCTIONS /////
////////////////////////////

// legal heap size is returned
static uint heap_size_regulation (int size)
{
	if (size < MIN_HEAP) return MIN_HEAP; // minimum heap size control (4096 bytes)
	
	return next_multiple_of_4 (size);
}

// round UP to the nearest multiple of 4
static uint next_multiple_of_4 (int size)
{
	switch (size % 4) 
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

// smallest free chunk larger than size is returned; NULL is returned if no chunk is available 
static header *smallest_free_chunk_larger_than_size (int size)
{
	header *sfchunk = NULL;
	int first_pointing_flag = CLEAR; // first time sfchunk points to a chunk
	for (header *curr = Heap.freeList[0]; (addr)curr < heapMaxAddr(); curr = (curr + curr->size))
	{
		if (first_pointing_flag == CLEAR) // first time sfchunk points to a chunk
		{
			if (curr->status == FREE && curr->size >= size)
			{
				sfchunk = curr;
				first_pointing_flag = SET;
			}
		} else // not first time sfchunk points to a chunk
		{
			if (curr->status == FREE && curr->size >= size && curr->size < sfchunk->size) 
			{
				sfchunk = curr;
			}
		}		
	}
	
	return sfchunk;
}

// insert the free target chunk to freeList
static void insert_to_freeList (header *target_chunk)
{
	int i;
	for (i = Heap.nFree - 1; (i >= 0 && (addr)Heap.freeList[i] > (addr)target_chunk); i--) 
	{
		Heap.freeList[i+1] = Heap.freeList[i];
	}
	Heap.freeList[i+1] = target_chunk;
	Heap.nFree++;
}

// delete the allocated target chunk from freeList
static void delete_from_freeList (header *target_chunk)
{
	// obtain index of target chunk in freeList
	int ichunk = binary_search (target_chunk, 0, Heap.nFree, Heap.freeList); 
	if (ichunk == -1) return; // target chunk not found in freeList
	
	// deletion process
	for (int i = ichunk; i < Heap.nFree - 1; i++) Heap.freeList[i] = Heap.freeList[i + 1];
	Heap.nFree--;
}

// index of target in freeList is returned; -1 is returned if target not found
static int binary_search (header *target, int low, int high, void *freeList[]) 
{
	if (high < low) return -1;
	
	int mid = (low + high) / 2;
	
	if ((addr)target == (addr)freeList[mid]) return mid;

	if ((addr)target > (addr)freeList[mid]) return binary_search (target, mid + 1, high, freeList);
	
	return binary_search (target, low, mid - 1, freeList); // target < freeList[mid]
}

// merge adjacent free chunks in the heap
static void adjchunks_merge (void)
{
	header *curr, *next;
	for (int i = 0; i < Heap.nFree - 1; i++)
	{
		curr = Heap.freeList[i];
		next = Heap.freeList[i+1];
		while ((addr)(curr + curr->size) == (addr)next) // two adjacent free chunks
		{ // merge process		
			curr->size = curr->size + next->size;
			next->status = 0;
			delete_from_freeList(next); 
			Heap.nFree--;
		}		
	}
}

///////////////////////////////////
///// END OF HELPER FUNCTIONS /////
///////////////////////////////////

////////////////////////////////
///// OTHER HEAP FUNCTIONS /////
////////////////////////////////

/** Release resources associated with the heap. */
void freeHeap (void)
{
	free (Heap.heapMem);
	free (Heap.freeList);
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

/** Return the first address beyond the range of the heap. */
static addr heapMaxAddr (void)
{
	return (addr) Heap.heapMem + Heap.heapSize;
}

///////////////////////////////////////
///// END OF OTHER HEAP FUNCTIONS /////
///////////////////////////////////////
