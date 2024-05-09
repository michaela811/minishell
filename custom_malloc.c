#include "lexer.h"

MemoryBlock *allocated_blocks = NULL;

void *custom_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size); // Call system MY_MALLOC
    if (ptr) {
		printf("Allocated %zu bytes at %p (File: %s, Line: %d)\n", size, ptr, file, line);
        MemoryBlock *block = malloc(sizeof(MemoryBlock));
        if (block) {
            block->address = ptr;
            block->size = size;
            block->file = file;
            block->line = line;
            block->next = allocated_blocks;
            allocated_blocks = block;
        }
    }
    return ptr;
}

// Custom memory deallocation function
void custom_free(void *ptr)
{
    if (!ptr) return;

    // Find the corresponding block in the list and remove it
    MemoryBlock *prev = NULL;
    MemoryBlock *curr = allocated_blocks;
    while (curr) {
        if (curr->address == ptr) {
            if (prev) {
                prev->next = curr->next;
            } else {
                allocated_blocks = curr->next;
            }
            free(curr->address);
            free(curr);
			printf("Freed memory at %p (File: %s, Line: %d)\n", ptr, curr->file, curr->line);
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    // If pointer not found in the list, it's likely an error
    fprintf(stderr, "Error: Attempted to free unallocated memory at %p\n", ptr);
}

// Function to check for memory leaks
void check_for_memory_leaks() {
    MemoryBlock *curr = allocated_blocks;
    while (curr) {
        fprintf(stderr, "Memory leak detected: %zu bytes allocated at %s:%d\n", curr->size, curr->file, curr->line);
        curr = curr->next;
    }
}

// Macros for easier use of custom_MY_MALLOC and custom_free
