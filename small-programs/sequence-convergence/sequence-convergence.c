#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#define EPSILON 1e-9
#define MAX_ITER 1000000

double a(unsigned int n) {
	return (double)1/log(n);
}

bool is_convergent(double (*seq)(unsigned int n), double *limit);

int main(void) {
        double limit;
        bool converges = is_convergent(a, &limit);

        if (converges)
                printf("The sequence a_n converges.\nIts limit is %.1lf\n", limit);
        else
                printf("The sequence a_n diverges.\n");

	
	return 0;
}

bool is_convergent(double (*seq)(unsigned int n), double *limit) {
        double diff;
        unsigned int window = 10;
         
        for (unsigned int i = 1; i + window < MAX_ITER; i++) {
                diff = fabs(seq(i+window)-seq(i)); 

                if (diff < EPSILON) {
			*limit = seq(MAX_ITER-window);
			return true;
		}

                if (!isfinite(seq(i))) return false;
        }

        return false;
}
