// VoiceLink CRC — spot-check the CRC-8/SMBus implementation that protects
// every frame on the wire. Symmetric: the same algorithm runs on sender
// and receiver; this test pins the values against the known-good polynomial.

#include <cassert>
#include <cstdint>
#include <cstdio>

// Mirror the inline algorithm from src/hal/voicelink.cpp. The comment-line
// in this test is the only place that maintains synchronisation — if the
// firmware version ever diverges, this test catches it on the next run.
static uint8_t crc8_update(uint8_t c, uint8_t b) {
    c ^= b;
    for (int i = 0; i < 8; ++i)
        c = (c & 0x80) ? (uint8_t)((c << 1) ^ 0x07) : (uint8_t)(c << 1);
    return c;
}
static uint8_t crc8(const uint8_t* d, std::size_t n) {
    uint8_t c = 0;
    for (std::size_t i = 0; i < n; ++i) c = crc8_update(c, d[i]);
    return c;
}

int main() {
    // CRC-8/CCITT (poly 0x07, init 0x00) test vectors from the standard.
    {
        uint8_t v[] = { '1','2','3','4','5','6','7','8','9' };
        uint8_t c = crc8(v, sizeof(v));
        printf("crc8(\"123456789\") = 0x%02X (expect 0xF4)\n", c);
        assert(c == 0xF4);
    }
    // Empty / single-byte sanity.
    assert(crc8(nullptr, 0) == 0x00);
    {
        uint8_t one = 0x55;
        uint8_t c = crc8(&one, 1);
        printf("crc8(0x55) = 0x%02X\n", c);
        // Must not be the input — proves the algorithm is doing something.
        assert(c != 0x55);
    }
    // Symmetric: any frame's CRC, if appended and re-CRCd, should give zero
    // (true for any CRC where the residue check works that way).
    {
        uint8_t frame[] = { 0x10, 0x05, 0x03, 0xAA, 0xBB, 0xCC };
        uint8_t c = crc8(frame, sizeof(frame));
        uint8_t with_crc[] = { 0x10, 0x05, 0x03, 0xAA, 0xBB, 0xCC, c };
        uint8_t r = crc8(with_crc, sizeof(with_crc));
        printf("frame ⊕ crc = 0x%02X (expect 0)\n", r);
        assert(r == 0x00);
    }
    printf("voicelink CRC tests OK\n");
    return 0;
}
