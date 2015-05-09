from sys import argv


if len(argv) != 2:
    print("Invalid parameters")
    exit()

with open(argv[1], "rb") as hfile:
    buffer = hfile.read()

result = []
for i in range(len(buffer) - 11):
    if (buffer[i] == 0xf4) and (buffer[i+1] == 0x7e) and (buffer[i+2] == 0x08) and (buffer[i+3] == 0x00):
        result.append(chr(buffer[i+10]))

print("".join(result))

