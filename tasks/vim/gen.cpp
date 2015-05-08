#include <cstdio>

unsigned seed = 0x2123;

unsigned rand() {
	seed = (seed * 0x93af7 ^ 0x7da95b88) + 0x156a8e5f;
	return seed;
}

int main() {
	for (int i = 0; i < 500 * 1024 * 1024; ++i) {
		int r = rand() % (126 - 32 + 1 + 1);
		if (!r)
			putchar('\n');
		else
			putchar(31 + r);
	}
	return 0;
}
