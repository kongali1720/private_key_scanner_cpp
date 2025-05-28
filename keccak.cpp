#include "keccak.h"
#include <cstring>
#include <sstream>
#include <iomanip>

#define ROUNDS 24
#define HASH_SIZE 32

typedef uint64_t u64;
static const u64 RC[ROUNDS] = {
    0x0000000000000001ULL, 0x0000000000008082ULL,
    0x800000000000808aULL, 0x8000000080008000ULL,
    0x000000000000808bULL, 0x0000000080000001ULL,
    0x8000000080008081ULL, 0x8000000000008009ULL,
    0x000000000000008aULL, 0x0000000000000088ULL,
    0x0000000080008009ULL, 0x000000008000000aULL,
    0x000000008000808bULL, 0x800000000000008bULL,
    0x8000000000008089ULL, 0x8000000000008003ULL,
    0x8000000000008002ULL, 0x8000000000000080ULL,
    0x000000000000800aULL, 0x800000008000000aULL,
    0x8000000080008081ULL, 0x8000000000008080ULL,
    0x0000000080000001ULL, 0x8000000080008008ULL
};

inline u64 ROTL64(u64 x, unsigned n) {
    return (x << n) | (x >> (64 - n));
}

void keccakf(u64 st[25]) {
    const int r[5] = {1, 3, 6, 10, 15};
    for (int round = 0; round < ROUNDS; ++round) {
        u64 b[5], t;

        for (int i = 0; i < 5; ++i)
            b[i] = st[i] ^ st[i + 5] ^ st[i + 10] ^ st[i + 15] ^ st[i + 20];

        for (int i = 0; i < 5; ++i) {
            t = b[(i + 4) % 5] ^ ROTL64(b[(i + 1) % 5], 1);
            for (int j = 0; j < 25; j += 5)
                st[j + i] ^= t;
        }

        u64 x = st[1], y;
        int j = 0;
        for (int i = 0; i < 24; ++i) {
            j = (i * 2 + 3 * i) % 5;
            y = st[r[i % 5]];
            st[r[i % 5]] = ROTL64(x, (i + 1) * (i + 2) / 2 % 64);
            x = y;
        }

        for (int j = 0; j < 25; j += 5) {
            u64 t[5];
            for (int i = 0; i < 5; ++i)
                t[i] = st[j + i];
            for (int i = 0; i < 5; ++i)
                st[j + i] = t[i] ^ ((~t[(i + 1) % 5]) & t[(i + 2) % 5]);
        }

        st[0] ^= RC[round];
    }
}

std::vector<uint8_t> keccak256(const std::vector<uint8_t>& input) {
    u64 st[25] = {0};
    size_t rate = 136;
    size_t len = input.size();

    std::vector<uint8_t> msg(input);
    msg.push_back(0x01);
    while ((msg.size() % rate) != (rate - 1)) {
        msg.push_back(0x00);
    }
    msg.push_back(0x80);

    for (size_t i = 0; i < msg.size(); i += rate) {
        for (size_t j = 0; j < rate / 8; ++j) {
            u64 t = 0;
            for (int k = 0; k < 8; ++k)
                t |= ((u64)msg[i + j * 8 + k]) << (8 * k);
            st[j] ^= t;
        }
        keccakf(st);
    }

    std::vector<uint8_t> hash(HASH_SIZE);
    for (size_t i = 0; i < HASH_SIZE / 8; ++i) {
        for (int j = 0; j < 8; ++j)
            hash[i * 8 + j] = (st[i] >> (8 * j)) & 0xFF;
    }

    return hash;
}

std::string keccak256Hex(const std::string& input) {
    std::vector<uint8_t> bytes(input.begin(), input.end());
    std::vector<uint8_t> hash = keccak256(bytes);

    std::ostringstream oss;
    for (auto byte : hash)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    return oss.str();
}
