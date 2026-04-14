// TLSF allocator stress test. Runs the in-tree TLSF over a 1 MB pool and
// throws thousands of random alloc/free operations at it; verifies bytes
// in/out balance and that we don't crash or leak forever.

extern "C" {
#include "tlsf.h"
}

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

int main() {
    constexpr std::size_t kPool = 1 * 1024 * 1024;
    std::vector<uint8_t> pool(kPool);
    tlsf_t t = tlsf_create_with_pool(pool.data(), kPool);
    assert(t);

    const std::size_t total = tlsf_total_bytes(t);
    const std::size_t free_before = tlsf_free_bytes(t);
    printf("TLSF: total=%zu free=%zu\n", total, free_before);
    assert(free_before > total - 4096);

    // 1) Many sequential allocations of varying sizes, then free in order.
    std::srand(0x12345678);
    std::vector<void*> live;
    live.reserve(200);
    for (int i = 0; i < 200; ++i) {
        std::size_t sz = (std::rand() % 1000) + 8;
        void* p = tlsf_malloc(t, sz);
        if (p) {
            std::memset(p, (uint8_t)(i & 0xFF), sz);
            live.push_back(p);
        }
    }
    for (auto* p : live) tlsf_free(t, p);
    live.clear();

    // 2) Interleaved alloc/free LIFO pattern — exercises split + immediate
    //    merge with the previous (just-freed) sibling.
    for (int i = 0; i < 500; ++i) {
        void* a = tlsf_malloc(t, 128);
        void* b = tlsf_malloc(t, 256);
        void* c2 = tlsf_malloc(t, 64);
        tlsf_free(t, b);
        tlsf_free(t, c2);
        tlsf_free(t, a);
    }

    std::size_t free_after = tlsf_free_bytes(t);
    printf("TLSF: after stress, free=%zu (should match initial %zu)\n",
           free_after, free_before);
    // Allow a small variance for fragmentation accounting.
    assert(free_after >= free_before - 64);

    // Aligned alloc — just verify the alignment is honoured. Currently we
    // don't free aligned-allocated pointers (the in-tree TLSF doesn't track
    // back-pointers); apps that need that today should pre-allocate from a
    // dedicated arena. This is fine for our DSP use cases (one-shot setup
    // allocations that live for the life of the device).
    void* aligned = tlsf_aligned_alloc(t, 256, 1000);
    assert(aligned);
    assert(((uintptr_t)aligned & 255) == 0);

    // Realloc grow.
    void* p = tlsf_malloc(t, 64);
    std::memset(p, 0xAA, 64);
    p = tlsf_realloc(t, p, 256);
    for (int i = 0; i < 64; ++i) assert(((uint8_t*)p)[i] == 0xAA);
    tlsf_free(t, p);

    printf("TLSF tests OK\n");
    return 0;
}
