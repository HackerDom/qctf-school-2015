#include <cstdio>
#include "rand.h"

int main() {
	z_srand(0x2123);
	for (int i = 0; i < 5 * 1024 * 1024; ++i)
		putchar(rand(0, 255));
	return 0;
}
