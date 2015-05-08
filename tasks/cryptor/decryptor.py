from sys import argv


if len(argv) != 2:
    print("Invalid parameters")
    exit()

with open(argv[1], "rb") as hfile:
    buffer = hfile.read()

decrypted = bytearray()
for i in range(len(buffer)):
    current = buffer[i] 
    current ^= 0x07
    current -= 4
    if current < 0:
        current += 256
    current ^= 0x05
    decrypted.append(current)

with open(argv[1], "wb") as hfile:
    hfile.write(decrypted)