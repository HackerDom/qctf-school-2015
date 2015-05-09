#include <stdio.h>
#include <windows.h>

char magic_key[] = {0x8b, 0xaf, 0x11, 0x77, 0xb7, 0xa0, 0x60, 0x23, 0x67, 0x12, 0x54, 0xc1, 0x42, 0x83, 0x23, 0x11, 0x73};
const int magic_len = 17;
const int magic = 657;

void genFlag()
{
    for (int i = 0; i < magic_len - 1; i++)
    {
        for (int j = 0; j < magic; j++)
	{
            magic_key[i] ^= 19 & magic_key[i+1];    
	    magic_key[i] += 32 | magic_key[i+1];
            magic_key[i] ^= 27 & magic_key[i+1];
	    magic_key[i] += 56 | magic_key[i+1];
            magic_key[i] ^= 42 & magic_key[i+1];
	    magic_key[i] += 43 | magic_key[i+1];
            magic_key[i] ^= 95 & magic_key[i+1];
	    magic_key[i] += 74 | magic_key[i+1];
            magic_key[i] ^= 69 & magic_key[i+1];
	    magic_key[i] += 31 | magic_key[i+1];
	}
    }
}

int main ()
{    
    printf("Hello, dear Cracker!\nWrite your code:\n");
    char* buffer;
    buffer =  (char*) malloc(magic_len + 1);
    if (buffer == NULL)
    {
	printf("Cant alloc memory!");
        return -1;
    }
    memset(buffer, 0, magic_len + 1);

    fgets(buffer, magic_len + 1, stdin);
    genFlag();
    if (strcmp(buffer, magic_key) == 0)
	printf("Your flag here: QCTF%s\n", magic_key);
    else
	printf("Wrong! Try again!\n");

    free(buffer);

    return 0;
}
