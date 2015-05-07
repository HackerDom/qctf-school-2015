#!/usr/bin/env python

# Copyright (C) 2009 Thialfihar (thi@thialfihar.org)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# http://www.gnu.org/licenses/

import base64
import struct
import sys

# need pyasn for DER parsing and generating
from pyasn1.type import univ
from pyasn1.codec.der import decoder, encoder


def read_int(buffer, i):
    "Read 32bit integer from buffer."

    (l,) = struct.unpack('!I', buffer[i:i + 4])
    i += 4
    return (l, i)


def read_chunk(buffer, i):
    "Read chunk from buffer."

    # first grab length of chunk
    (l, i) = read_int(buffer, i)
    if l > 1000000:
        # just in case... if this happens, then something is way off
        raise Exception("got chunk length of %d, that's certainly too long" % l)

    # read chunk of length l
    (s,) = struct.unpack('!' + '%ds' % l, buffer[i:i + l])
    i += l
    return (s, i)


def unpack_bigint(buffer):
    "Turn binary chunk into integer."

    v = 0
    for c in buffer:
        v *= 256
        v += ord(c)

    return v


def pack_bigint(v):
    "Pack integer into binary chunk."

    chunk = ''
    rest = v
    while rest:
        chunk = chr(rest % 256) + chunk
        rest //= 256

    # add a zero byte if the highest bit is 1, so it won't be negative
    if ord(chunk[0]) & 128:
        chunk = chr(0) + chunk

    return chunk


def write_rsa_pub(filename, n, e, host):
    """Write RSA public key file. Structure:

       ssh-rsa base64data user@host

       base64data: [7]ssh-rsa[len][e-data][len][n-data]
    """

    e_str = pack_bigint(e)
    n_str = pack_bigint(n)
    # pack e and n properly into the raw data
    raw = struct.pack('!I7sI%dsI%ds' % (len(e_str), len(n_str)), 7, 'ssh-rsa',
                                        len(e_str), e_str, len(n_str), n_str)
    # assemble file content and save it
    content = "ssh-rsa %s %s\n" % (base64.b64encode(raw), host)
    file(filename, 'w').write(content)


def write_rsa_pri(filename, n, e, d, p, q, e1, e2, c):
    """Write RSA private key file. Structure:

       -----BEGIN RSA PRIVATE KEY-----
       base64data
       -----END RSA PRIVATE KEY-----

       base64data DER structure:

       RSAPrivateKey ::= SEQUENCE {
         version Version,
         modulus INTEGER,         -- n
         publicExponent INTEGER,  -- e
         privateExponent INTEGER, -- d
         prime1 INTEGER,          -- p
         prime2 INTEGER,          -- q
         exponent1 INTEGER,       -- d mod (p - 1)
         exponent2 INTEGER,       -- d mod (q - 1)
         coefficient INTEGER      -- q^-1 mod p
       }
    """

    seq = (
        univ.Integer(0),
        univ.Integer(n),
        univ.Integer(e),
        univ.Integer(d),
        univ.Integer(p),
        univ.Integer(q),
        univ.Integer(e1),
        univ.Integer(e2),
        univ.Integer(c),
    )
    struct = univ.Sequence()
    for i in xrange(len(seq)):
        struct.setComponentByPosition(i, seq[i])

    # build DER structure
    raw = encoder.encode(struct)
    # encode to base64
    data = base64.b64encode(raw)

    # chop data up into lines of certain width
    width = 64
    chopped = [data[i:i + width] for i in xrange(0, len(data), width)]
    # assemble file content
    content = """-----BEGIN RSA PRIVATE KEY-----
%s
-----END RSA PRIVATE KEY-----
""" % '\n'.join(chopped)
    file(filename, 'w').write(content)


def build_fancy_ssh_key():
    n = 26240039799439281277428991018816126472584192712357885635313159618017867599553946910086493824697325697459899057037252321992434312383526020179103669324033172240080083504036221922015855600754563085717218550168161255435496850472698530773001974651033983647351279774260999491275857855184240838513799742065892227247500196946486743887238163332335265778058266229624732673600138507643201455967290783154676397787677091064108104899760962349320575164417986884080576662581242348768103625357724955359787090235358588550254354354517795469206122862585274648362207432821350507008845767866686240836257288458101839822991339180170660270339
    p = 163493857190096272089714326824687833016230055253025944605019043980668505352847867467836176472809138237630251341555249787949745988391345622816563382634316979998428913478943333115469580963083309536717317636885711244757578668516876376648175000497963982248437505385251001367254125956049451693892145283722300414193
    q = n // p
    e = 65537

    print("n", n)

    phi_n = (p - 1) * (q - 1)

    import gmpy
    d = int(gmpy.invert(e, phi_n))

    print("d", d)

    print("mod", (e * d) % phi_n)
    e1 = d % (p - 1)
    e2 = d % (q - 1)
    print("e1", e1)
    print("e2", e2)

    c = int(gmpy.invert(q, p))
    print("c", c)

    # write new key files
    write_rsa_pub("id.pub", n, e, "qctf")
    write_rsa_pri("id", n, e, d, p, q, e1, e2, c)

    print 'new keys:\n%s\n%s\n' % ("id.pub", "id")


if __name__ == "__main__":
    key_pair = build_fancy_ssh_key()
