#include <stdint.h>
#include <string.h>
#include "common.h"

static __attribute__((aligned(128))) uint64_t state[32];
static __attribute__((aligned(128))) uint64_t inputs[32];
static __attribute__((aligned(128))) uint64_t savei[32];
static __attribute__((aligned(128))) uint64_t bcs[8];

__attribute__((aligned(128))) uint64_t keccakf_rndc[] = {
			0x000000008000808bL, 0x800000000000008bL, 0x8000000000008089L, 0x8000000000008003L,
			0x8000000000008002L, 0x8000000000000080L, 0x000000000000800aL, 0x800000008000000aL,
			0x8000000080008081L, 0x8000000000008080L, 0x0000000080000001L, 0x8000000080008008L };

static uint64_t rotate1(uint64_t a) {
	return	(a << 1) | (a >> (64-1));
}

static uint64_t rotate64(uint64_t a, int v) {
	return	(a << v) | (a >> (64-v));
}

static uint64_t bitselect(uint64_t a, uint64_t b, uint64_t c) {
	return (a & ~c) | (b & c);
}

static inline void algo_k12(uint64_t *st,uint64_t *bc) {
	uint64_t t,c0;

	for(int round = 0; round < 12; ++round)  {
		bc[0] = st[0] ^ st[5] ^ st[10] ^ st[15] ^ st[20] ^ 	rotate1(st[2] ^ st[7] ^ st[12] ^ st[17] ^ st[22]);
		bc[1] = st[1] ^ st[6] ^ st[11] ^ st[16] ^ st[21] ^ 	rotate1(st[3] ^ st[8] ^ st[13] ^ st[18] ^ st[23]);
		bc[2] = st[2] ^ st[7] ^ st[12] ^ st[17] ^ st[22] ^  rotate1(st[4] ^ st[9] ^ st[14] ^ st[19] ^ st[24]);
		bc[3] = st[3] ^ st[8] ^ st[13] ^ st[18] ^ st[23] ^  rotate1(st[0] ^ st[5] ^ st[10] ^ st[15] ^ st[20]);
		bc[4] = st[4] ^ st[9] ^ st[14] ^ st[19] ^ st[24] ^ 	rotate1(st[1] ^ st[6] ^ st[11] ^ st[16] ^ st[21]);

		st[0] ^= bc[4];
		st[1] ^= bc[0];
		st[2] ^= bc[1];
		st[3] ^= bc[2];
		st[4] ^= bc[3];
		st[5] ^= bc[4];
		st[6] ^= bc[0];
		st[7] ^= bc[1];
		st[8] ^= bc[2];
		st[9] ^= bc[3];
		st[10] ^= bc[4];
		st[11] ^= bc[0];
		st[12] ^= bc[1];
		st[13] ^= bc[2];
		st[14] ^= bc[3];
		st[15] ^= bc[4];
		st[16] ^= bc[0];
		st[17] ^= bc[1];
		st[18] ^= bc[2];
		st[19] ^= bc[3];
		st[20] ^= bc[4];
		st[21] ^= bc[0];
		st[22] ^= bc[1];
		st[23] ^= bc[2];
		st[24] ^= bc[3];

		// Rho Pi
		t = st[1];
		c0 = st[10];st[10] = rotate64(t, 1); t = c0;
		c0 = st[7];st[7]   = rotate64(t, 3); t = c0;
		c0 = st[11];st[11] = rotate64(t, 6); t = c0;
		c0 = st[17];st[17] = rotate64(t, 10); t = c0;
		c0 = st[18];st[18] = rotate64(t, 15); t = c0;
		c0 = st[3];st[3]   = rotate64(t, 21); t = c0;
		c0 = st[5];st[5]   = rotate64(t, 28); t = c0;
		c0 = st[16];st[16] = rotate64(t, 36); t = c0;
		c0 = st[8];st[8]   = rotate64(t, 45); t = c0;
		c0 = st[21];st[21] = rotate64(t, 55); t = c0;
		c0 = st[24];st[24] = rotate64(t, 2); t = c0;
		c0 = st[4];st[4]   = rotate64(t, 14); t = c0;
		c0 = st[15];st[15] = rotate64(t, 27); t = c0;
		c0 = st[23];st[23] = rotate64(t, 41); t = c0;
		c0 = st[19];st[19] = rotate64(t, 56); t = c0;
		c0 = st[13];st[13] = rotate64(t, 8); t = c0;
		c0 = st[12];st[12] = rotate64(t, 25); t = c0;
		c0 = st[2];st[2]   = rotate64(t, 43); t = c0;
		c0 = st[20];st[20] = rotate64(t, 62); t = c0;
		c0 = st[14];st[14] = rotate64(t, 18); t = c0;
		c0 = st[22];st[22] = rotate64(t, 39); t = c0;
		c0 = st[9];st[9]   = rotate64(t, 61); t = c0;
		c0 = st[6];st[6]   = rotate64(t, 20); t = c0;
		c0 = st[1];st[1]   = rotate64(t, 44); t = c0;


		bc[0] = st[0];
		bc[1] = st[1];
		st[0] = bitselect(st[0] ^ st[2], st[0], st[1]);
		st[1] = bitselect(st[1] ^ st[3], st[1], st[2]);
		st[2] = bitselect(st[2] ^ st[4], st[2], st[3]);
		st[3] = bitselect(st[3] ^ bc[0], st[3], st[4]);
		st[4] = bitselect(st[4] ^ bc[1], st[4], bc[0]);

		bc[0] = st[5];
		bc[1] = st[6];
		bc[2] = bitselect(st[5] ^ st[7], st[5], st[6]);
		bc[3] = bitselect(st[6] ^ st[8], st[6], st[7]);
		st[5] = bc[2];
		st[6] = bc[3];

		bc[2] = bitselect(st[7] ^ st[9], st[7], st[8]);
		bc[3] = bitselect(st[8] ^ bc[0], st[8], st[9]);
		st[7] = bc[2];
		st[8] = bc[3];

		st[9] = bitselect(st[9] ^ bc[1], st[9], bc[0]);
		bc[0] = st[10];
		bc[1] = st[11];

		st[10] = bitselect(st[10] ^ st[12], st[10], st[11]);
		st[11] = bitselect(st[11] ^ st[13], st[11], st[12]);

		st[12] = bitselect(st[12] ^ st[14], st[12], st[13]);
		st[13] = bitselect(st[13] ^ bc[0],  st[13], st[14]);
		st[14] = bitselect(st[14] ^ bc[1],  st[14], bc[0]);

		bc[0] = st[15];
		bc[1] = st[16];
		bc[2] = bitselect(st[15] ^ st[17], st[15], st[16]);
		bc[3] = bitselect(st[16] ^ st[18], st[16], st[17]);

		st[15] = bc[2];
		st[16] = bc[3];
		bc[2] = bitselect(st[17] ^ st[19],st[17], st[18]);
		bc[3] = bitselect(st[18] ^ bc[0], st[18], st[19]);

		st[17] = bc[2];
		st[18] = bc[3];
		st[19] = bitselect(st[19] ^ bc[1], st[19], bc[0]);
		bc[0] = st[20];
		bc[1] = st[21];
		st[20] = bitselect(st[20] ^ st[22], st[20], st[21]);
		st[21] = bitselect(st[21] ^ st[23], st[21], st[22]);
		st[22] = bitselect(st[22] ^ st[24], st[22], st[23]);
		st[23] = bitselect(st[23] ^ bc[0],  st[23], st[24]);
		st[24] = bitselect(st[24] ^ bc[1],  st[24], bc[0]);

		//  Iota
		st[0] ^= keccakf_rndc[round];
	}
}

void mine(uint64_t nonce, uint64_t target, uint8_t *blob) {
    uint64_t  *ss = state;
    uint64_t  *is = inputs;
    unsigned int i = 0;

    for (i=0; i< 10; i++)
    	is[i] = ((uint64_t)blob[i*8]) | ((uint64_t)blob[i*8+1] << 8) | ((uint64_t)blob[i*8+2] << 16) | ((uint64_t)blob[i*8+3] << 24) | ((uint64_t)blob[i*8+4] << 32 | ((uint64_t)blob[i*8+5] << 40) | ((uint64_t)blob[i*8+6] << 48) |((uint64_t)blob[i*8+7] << 56));

    is[10] = 0x700;
    is[11] = 0x0;
    is[12] = 0x0;
    is[13] = 0x0;
    is[14] = 0x0;
    is[15] = 0x0;
    is[16] = 0x00L;
    is[17] = 0x00L;
    is[18] = 0x00L;
    is[19] = 0x00L;
    is[20] = 0x8000000000000000L;
    is[21] = 0x00L;
    is[22] = 0x00L;
    is[23] = 0x00L;
    is[24] = 0x00L;

    for (i=0; likely(i< _LOOPS_K12); i++, nonce++) {
    	memcpy(ss,is,25*8);
        ss[4] &= 0x00FFFFFFFFFFFFFFL;
        ss[4] |= (uint64_t)(nonce & 0xFF) << (24+32);
        ss[5] &= 0xFF00000000000000L;
        ss[5] |= nonce >> 8;
        algo_k12(ss,bcs);
        if (unlikely(ss[3] < target))
		send_nonce(nonce);
    }
}


