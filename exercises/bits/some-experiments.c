#include <stdio.h>
#include <stdint.h>

/* Write a function setbits(x, p, n, y) that returns x with the n bits 
that begin at p set to the rightmost n bits of y, leaving the other bits unchanged */

void setbits(int *x, int p, int n, int y);

/* Write a function that swaps the 4 upper bits and lower 4 bits (nibbles) of an 8-bit unsigned integer */

void swap_nibbles(uint8_t *x);

/* Write a function that counts the number of 1s in an unsigned 32-bit integer */

int count_ones(uint32_t x);
int kr_countsetbits(uint32_t x);

/* Write a function that reverses the bits of an 32-bit unsigned integer */

uint32_t reverse_bits(uint32_t x);

int main(void) {
        int x = 0b10101111, y = 0b00001100;
        setbits(&x, 4, 3, y);
        printf("%d\n", x);

	uint8_t z = 0b10100011;
	swap_nibbles(&z);
	printf("%d\n", z == 0b00111010);
	
	uint32_t t = 314551;
	printf("%d\n", kr_countsetbits(t)); // should be 11
	
	uint32_t p = 1;
        p = reverse_bits(p);
        printf("%u, %u\n", p, ~0U);
	
	return 0;
}

void setbits(int *x, int p, int n, int y) { 
	 *x = (~(((1 << n) - 1) << (p - n + 1)) & *x) | ((((1 << n) - 1) & y) << (p - n + 1)) ;
}

void swap_nibbles(uint8_t *x) {
	uint8_t mask = (1 << 4) - 1;
	*x = ((mask & *x) << 4) | (((mask << 4) & *x) >> 4);
}

int count_ones(uint32_t x) {
        int c = 0;
        for (int k = 0; k < 32; k++)
                if (x & (1 << k))
                        c++;

        return c;
}

int kr_countsetbits(uint32_t x) {
	int c = 0;
	while (x) {
		x &= (x - 1);
		c++;
	}

	return c;	
}

uint32_t reverse_bits(uint32_t x) {
	uint32_t result = 0;
	for (int i = 0; i < 32; i++) {
		result <<= 1;
		result |= (x & 1);
		x >>= 1;
	}

	return result;
}
