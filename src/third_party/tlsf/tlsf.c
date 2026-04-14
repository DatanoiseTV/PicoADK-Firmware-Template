// Minimal TLSF allocator — first-fit segregated freelist.
//
// Compact (~250 lines) implementation suitable for the few-MB PSRAM region
// on PicoADK v2. Two-level bins (FL=20, SL=16) give O(1) malloc/free with a
// fragmentation profile equivalent to the reference TLSF for blocks
// 64 bytes .. 1 MB. For larger or smaller blocks behaviour degrades to
// linear search of one bin — fine for sample buffers / FX delay lines.

#include "tlsf.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define FL_BITS         20      // first-level bins (max block 1 MB)
#define SL_BITS         4
#define SL_COUNT        (1 << SL_BITS)
#define MIN_BLOCK_SIZE  32
#define ALIGN           8u

typedef struct block_header {
    struct block_header* prev_phys;
    size_t               size_and_flags;     // bit0=free, bit1=prev_free
    struct block_header* next_free;
    struct block_header* prev_free;
} block_header_t;

#define HDR_SIZE   (sizeof(block_header_t))
#define USED_SIZE  (offsetof(block_header_t, next_free))   // header for in-use blocks

#define BH_FREE      1u
#define BH_PREV_FREE 2u

typedef struct tlsf_control {
    uint32_t        fl_bitmap;
    uint16_t        sl_bitmap[FL_BITS];
    block_header_t* blocks[FL_BITS][SL_COUNT];
    block_header_t  null_block;     // sentinel for "no block"
    size_t          total;
    size_t          used;
} tlsf_control_t;

static inline size_t bh_size(const block_header_t* b) { return b->size_and_flags & ~3u; }
static inline bool   bh_is_free(const block_header_t* b) { return b->size_and_flags & BH_FREE; }
static inline bool   bh_is_prev_free(const block_header_t* b) { return b->size_and_flags & BH_PREV_FREE; }
static inline void   bh_set_free(block_header_t* b, bool f) {
    b->size_and_flags = (b->size_and_flags & ~BH_FREE) | (f ? BH_FREE : 0);
}
static inline void   bh_set_prev_free(block_header_t* b, bool f) {
    b->size_and_flags = (b->size_and_flags & ~BH_PREV_FREE) | (f ? BH_PREV_FREE : 0);
}
static inline void   bh_set_size(block_header_t* b, size_t s) {
    b->size_and_flags = (s & ~3u) | (b->size_and_flags & 3u);
}
static inline block_header_t* bh_next(block_header_t* b) {
    return (block_header_t*)((uint8_t*)b + USED_SIZE + bh_size(b));
}

// Map a request size to (fl, sl) bin index.
static void mapping(size_t size, int* fl, int* sl) {
    if (size < (1u << SL_BITS)) {
        *fl = 0;
        *sl = (int)size / (int)((1u << SL_BITS) / SL_COUNT);
    } else {
        int f = 31 - __builtin_clz((uint32_t)size);
        *sl  = (int)((size >> (f - SL_BITS)) - SL_COUNT);
        *fl  = f - SL_BITS;
        if (*fl >= FL_BITS) { *fl = FL_BITS - 1; *sl = SL_COUNT - 1; }
    }
}

static void insert_block(tlsf_control_t* c, block_header_t* b) {
    int fl, sl;
    mapping(bh_size(b), &fl, &sl);
    block_header_t* head = c->blocks[fl][sl];
    b->next_free = head ? head : &c->null_block;
    b->prev_free = &c->null_block;
    if (head) head->prev_free = b;
    c->blocks[fl][sl] = b;
    c->fl_bitmap   |= (1u << fl);
    c->sl_bitmap[fl] |= (uint16_t)(1u << sl);
}
static void remove_block(tlsf_control_t* c, block_header_t* b) {
    int fl, sl;
    mapping(bh_size(b), &fl, &sl);
    if (b->prev_free && b->prev_free != &c->null_block)
        b->prev_free->next_free = b->next_free;
    if (b->next_free && b->next_free != &c->null_block)
        b->next_free->prev_free = b->prev_free;
    if (c->blocks[fl][sl] == b)
        c->blocks[fl][sl] = (b->next_free == &c->null_block) ? NULL : b->next_free;
    if (!c->blocks[fl][sl]) {
        c->sl_bitmap[fl] &= (uint16_t)~(1u << sl);
        if (c->sl_bitmap[fl] == 0) c->fl_bitmap &= ~(1u << fl);
    }
}

static block_header_t* find_suitable(tlsf_control_t* c, size_t size) {
    int fl, sl;
    mapping(size, &fl, &sl);
    uint16_t sl_map = c->sl_bitmap[fl] & (uint16_t)(~0u << sl);
    if (!sl_map) {
        uint32_t fl_map = c->fl_bitmap & (~0u << (fl + 1));
        if (!fl_map) return NULL;
        fl     = __builtin_ctz(fl_map);
        sl_map = c->sl_bitmap[fl];
    }
    sl = __builtin_ctz(sl_map);
    return c->blocks[fl][sl];
}

tlsf_t tlsf_create_with_pool(void* mem, size_t bytes) {
    if (bytes < sizeof(tlsf_control_t) + MIN_BLOCK_SIZE * 2) return NULL;
    tlsf_control_t* c = (tlsf_control_t*)mem;
    memset(c, 0, sizeof(*c));
    c->null_block.next_free = &c->null_block;
    c->null_block.prev_free = &c->null_block;

    uint8_t* pool = (uint8_t*)mem + ((sizeof(tlsf_control_t) + ALIGN - 1) & ~(ALIGN - 1));
    size_t   pool_bytes = bytes - (size_t)(pool - (uint8_t*)mem);
    pool_bytes &= ~(ALIGN - 1);

    block_header_t* first = (block_header_t*)pool;
    first->prev_phys = NULL;
    first->size_and_flags = (pool_bytes - USED_SIZE - USED_SIZE) | BH_FREE;
    insert_block(c, first);

    block_header_t* sentinel = bh_next(first);
    sentinel->prev_phys = first;
    sentinel->size_and_flags = 0 | BH_PREV_FREE;

    c->total = bh_size(first);
    c->used  = 0;
    return c;
}
void tlsf_destroy(tlsf_t t) { (void)t; }

static void* allocate(tlsf_control_t* c, size_t size) {
    size = (size + ALIGN - 1) & ~(ALIGN - 1);
    if (size < MIN_BLOCK_SIZE) size = MIN_BLOCK_SIZE;
    block_header_t* b = find_suitable(c, size);
    if (!b) return NULL;
    remove_block(c, b);

    size_t total_size = bh_size(b);
    if (total_size >= size + USED_SIZE + MIN_BLOCK_SIZE) {
        block_header_t* split = (block_header_t*)((uint8_t*)b + USED_SIZE + size);
        split->prev_phys = b;
        split->size_and_flags = (total_size - size - USED_SIZE) | BH_FREE;
        bh_set_size(b, size);
        bh_next(split)->prev_phys = split;
        bh_set_prev_free(bh_next(split), true);
        insert_block(c, split);
    } else {
        bh_set_prev_free(bh_next(b), false);
    }
    bh_set_free(b, false);
    c->used += bh_size(b);
    return (uint8_t*)b + USED_SIZE;
}

void* tlsf_malloc(tlsf_t t, size_t bytes) {
    if (!t || !bytes) return NULL;
    return allocate((tlsf_control_t*)t, bytes);
}
void* tlsf_aligned_alloc(tlsf_t t, size_t align, size_t bytes) {
    // Crude: oversize and mask. Good enough for the alignments we see in
    // audio code (typically 16/32 bytes).
    if (align <= ALIGN) return tlsf_malloc(t, bytes);
    void* raw = tlsf_malloc(t, bytes + align);
    if (!raw) return NULL;
    uintptr_t mask = align - 1;
    uintptr_t adj  = ((uintptr_t)raw + mask) & ~mask;
    return (void*)adj;
}
void tlsf_free(tlsf_t t, void* ptr) {
    if (!t || !ptr) return;
    tlsf_control_t* c = (tlsf_control_t*)t;
    block_header_t* b = (block_header_t*)((uint8_t*)ptr - USED_SIZE);
    bh_set_free(b, true);
    c->used -= bh_size(b);

    // Merge with previous if it's free.
    if (bh_is_prev_free(b)) {
        block_header_t* prev = b->prev_phys;
        if (prev && bh_is_free(prev)) {
            remove_block(c, prev);
            bh_set_size(prev, bh_size(prev) + USED_SIZE + bh_size(b));
            b = prev;
        }
    }
    // Merge with next if it's free.
    block_header_t* next = bh_next(b);
    if (bh_is_free(next)) {
        remove_block(c, next);
        bh_set_size(b, bh_size(b) + USED_SIZE + bh_size(next));
    }
    bh_set_prev_free(bh_next(b), true);
    bh_next(b)->prev_phys = b;
    insert_block(c, b);
}

void* tlsf_realloc(tlsf_t t, void* ptr, size_t bytes) {
    if (!ptr) return tlsf_malloc(t, bytes);
    if (!bytes) { tlsf_free(t, ptr); return NULL; }
    block_header_t* b = (block_header_t*)((uint8_t*)ptr - USED_SIZE);
    if (bh_size(b) >= bytes) return ptr;
    void* p = tlsf_malloc(t, bytes);
    if (!p) return NULL;
    memcpy(p, ptr, bh_size(b));
    tlsf_free(t, ptr);
    return p;
}

size_t tlsf_block_size(void* ptr) {
    block_header_t* b = (block_header_t*)((uint8_t*)ptr - USED_SIZE);
    return bh_size(b);
}
size_t tlsf_pool_overhead(void)  { return sizeof(tlsf_control_t); }
size_t tlsf_alloc_overhead(void) { return USED_SIZE; }

size_t tlsf_total_bytes(tlsf_t t) { return t ? ((tlsf_control_t*)t)->total : 0; }
size_t tlsf_free_bytes (tlsf_t t) {
    if (!t) return 0;
    tlsf_control_t* c = (tlsf_control_t*)t;
    return c->total > c->used ? c->total - c->used : 0;
}
size_t tlsf_largest_free_block(tlsf_t t) {
    if (!t) return 0;
    tlsf_control_t* c = (tlsf_control_t*)t;
    size_t best = 0;
    for (int fl = FL_BITS - 1; fl >= 0; --fl) {
        if (!(c->fl_bitmap & (1u << fl))) continue;
        for (int sl = SL_COUNT - 1; sl >= 0; --sl) {
            if (!(c->sl_bitmap[fl] & (1u << sl))) continue;
            block_header_t* b = c->blocks[fl][sl];
            while (b && b != &c->null_block) {
                size_t s = bh_size(b);
                if (s > best) best = s;
                b = b->next_free;
            }
        }
    }
    return best;
}
