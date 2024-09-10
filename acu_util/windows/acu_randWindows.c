#include "..\acu_rand.h"

uint32_t rotl(const uint32_t x, int k) {
    return (x << k) | (x >> (32 - k));
}