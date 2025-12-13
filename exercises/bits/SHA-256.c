  
/* 
   *     Intro:
   *     SHA-2 (Secure Hash Algorithm 2) is a set of cryptographic hash functions designed by the United States National Security Agency (NSA)
   *     and first published in 2001. They are built using the Merkle–Damgård construction, from a one-way compression function itself 
   *     built using the Davies–Meyer structure from a specialized block cipher. 
*/


/*

  * Take a message of arbitrary length (array of bytes)

  * Append a single 1 bit, followed by enough 0 bits so that the total length (in bits) ≡ 448 mod 512

  * Append the 64-bit big-endian length of the original message

  * Take each 512-bit block (64 bytes) of padded_msg

  * Break it into 16 words of 32 bits each (uint32_t W[16])

  * Each word is big-endian: most significant byte first

  * SHA-256 uses 64 words per block (W[0..63])

  * The first 16 words = packed words from the block (W[0..15])

  * Remaining words (W[16..63]) are computed with:

*/ 

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define MSG_LENGTH 48

uint32_t rotr(uint32_t x, uint32_t n);
uint32_t sigma0(uint32_t x);
uint32_t sigma1(uint32_t x);

uint8_t msg[MSG_LENGTH] = {
    0x2E, 0xF7, 0x9C, 0x41, 0xB3, 0x08, 0xCD, 0x6A,
    0x5F, 0xE2, 0x1D, 0x84, 0x77, 0xA9, 0x33, 0xCE,
    0x90, 0x4B, 0xDC, 0x25, 0x7A, 0xB6, 0x01, 0xE8,
    0x59, 0xC4, 0x3F, 0x92, 0x68, 0xD5, 0x0B, 0x74,
    0xAF, 0x26, 0x99, 0x52, 0xBD, 0x10, 0xE3, 0x7C,
    0x47, 0x8A, 0x15, 0xF0, 0x2A, 0x66, 0xC9, 0xB1
};

int main(void) {
        const uint64_t msg_bitlen = MSG_LENGTH * 8;
        const uint64_t padded_len = ((MSG_LENGTH + 9 + 63) / 64) / 64;
	uint8_t *padded_msg = malloc(padded_len * sizeof(uint8_t));
        
        for (int i = 0; i < padded_len; i++)
                padded_msg[i] = 0U;

	memcpy(padded_msg, msg, MSG_LENGTH);
        padded_msg[MSG_LENGTH] = 0x80;

        for (int i = 0; i < 8; i++)
                padded_msg[padded_len - i - 1] = (msg_bitlen >> (8 * i)) & 0xFF;

        uint32_t *words = (uint32_t *)padded_msg;
	for (int i = 0; i < 16; i++)
		words[i] = (padded_msg[4*i] << 24) | (padded_msg[4*i+1] << 16) |
				(padded_msg[4*i+2] << 8) | (padded_msg[4*i+3]);
	
	for (int i = 16; i < 64; i++)
		words[i] = sigma1(words[i-2]) + words[i-7] + sigma0(words[i-15]) + words[i-16];
	
	return 0;	
}

uint32_t rotr(uint32_t x, uint32_t n) {
	return (x >> n) | (x << (32 - n));
}

uint32_t sigma0(uint32_t x) {
	return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
}

uint32_t sigma1(uint32_t x) {
	return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
}
