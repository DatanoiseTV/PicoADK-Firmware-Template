// Minimal TLSF (Two-Level Segregated Fit) allocator — public API.
//
// Compact embedded port suitable for placing on top of PSRAM. Exposes the
// same shape of calls as the reference mattconte/tlsf so the implementation
// can be swapped later if more features are needed (memalign, walks,
// statistics, …).

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* tlsf_t;

tlsf_t tlsf_create_with_pool(void* mem, size_t bytes);
void   tlsf_destroy(tlsf_t tlsf);

void*  tlsf_malloc(tlsf_t tlsf, size_t bytes);
void*  tlsf_aligned_alloc(tlsf_t tlsf, size_t alignment, size_t bytes);
void   tlsf_free(tlsf_t tlsf, void* ptr);
void*  tlsf_realloc(tlsf_t tlsf, void* ptr, size_t bytes);

size_t tlsf_block_size(void* ptr);
size_t tlsf_pool_overhead(void);
size_t tlsf_alloc_overhead(void);

// Quick-and-dirty stats (best-effort, no defragmentation).
size_t tlsf_total_bytes(tlsf_t tlsf);
size_t tlsf_free_bytes(tlsf_t tlsf);
size_t tlsf_largest_free_block(tlsf_t tlsf);

#ifdef __cplusplus
}
#endif
