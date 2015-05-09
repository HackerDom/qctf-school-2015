#include <windows.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
	printf("Invalid parameters!");
	return -1;
    }
    
    HANDLE hFile = CreateFileA(argv[1],  GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
	printf("Can't open file: %s", argv[1]);
	return -2;
    }

    LARGE_INTEGER size;
    memset(&size, 0, sizeof(LARGE_INTEGER));
    if (!GetFileSizeEx(hFile, &size))
    {
	printf("Can't get size of file: %s", argv[1]);
	CloseHandle(hFile);
	return -3;
    }

    if ((int) size.HighPart != 0)
    {
	printf("Error! File is too big!");
	CloseHandle(hFile);
	return -4;
    }

    BYTE* buffer;
    buffer =  (BYTE*) malloc(size.LowPart);
    if (buffer == NULL)
    {
	printf("Cant alloc memory!");
	CloseHandle(hFile);
        return -5;
    }
    memset(buffer, 0, size.LowPart);

    DWORD dwCheck = 0;
    BOOL bCheck = FALSE;
    bCheck = ReadFile(hFile, buffer, size.LowPart, &dwCheck, NULL);
    if (!bCheck || (dwCheck != size.LowPart))
    {
	printf("Can't read file! Errorcode: %d", (int) GetLastError());
	CloseHandle(hFile); 
	free(buffer);
	return -6;
    }

    for (int i = 0; i < (int) size.LowPart; i++)
    {
	buffer[i] ^= 0x05;
	buffer[i] += 4;
	buffer[i] ^= 0x07;	
    }

    if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
    {
	printf("Can't set file pointer! Errorcode: %d", (int) GetLastError());
	CloseHandle(hFile); 
	free(buffer);
	return -7;
    }

    bCheck = WriteFile(hFile, buffer, size.LowPart, &dwCheck, NULL);
    if (!bCheck || (dwCheck != size.LowPart))
    {
	printf("Can't write file! Errorcode: %d", (int) GetLastError());
	CloseHandle(hFile); 
	free(buffer);
	return -8;
    }

    printf("success");

    CloseHandle(hFile);
    free(buffer);

    return 0;
}
