import socket
from struct import pack, unpack
from ctypes import c_uint16

def calc_checksum(data):
    result = 0
    for i in range(0, len(data) - 1, 2):
        result += data[i+1] * 256 + data[i]
    if (len(data) % 2) != 0:
        result += data[-1]

    result = (result >> 16) + c_uint16(result).value
    result = ~(result + (result >> 16))
    result = c_uint16(result).value

    return result

def new_packet(request_id, symbol):
    request_id += 1
    request_header = pack('bbHHh', 8, 0, 0, request_id, 1)
    '''
    byte type = ICMP_ECHO_REQUEST
    byte code = 0
    unsigned short checksum - now is null
    unsigned short packet id
    sec. num is one, but it doesn't matter
    '''
    request_data = bytes([symbol])# + b'\x00' * 63
    chksum = calc_checksum(request_header + request_data)
    request_header = pack('bbHHh', 8, 0, chksum, request_id, 1)
    return request_header + request_data

flag_buf = b'concat domain name with HGYA3PCkoN'

target_ip = socket.gethostbyname("QCTF.ru")
sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.getprotobyname('icmp'))

counter = 1
for i in flag_buf:
    data = new_packet(counter, i)
    counter += 1

    sock.sendto(data, (target_ip, 1))

