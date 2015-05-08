#!/bin/bash

useradd -b /home -s /usr/bin/vim vim
echo -e "vim:vim" | chpasswd

mkdir -p /var/jail/{dev,etc,lib,lib64,usr,bin}
mkdir -p /var/jail/usr/bin
mkdir -p /var/jail/home/vim
mknod -m 666 /var/jail/dev/null c 1 3

cp /etc/ld.so.cache /var/jail/etc
cp /etc/ld.so.conf /var/jail/etc
cp /etc/nsswitch.conf /var/jail/etc
cp /etc/hosts /var/jail/etc

cp /usr/bin/vim /var/jail/usr/bin

chmod +x l2chroot.sh
./l2chroot.sh vim

echo -e "\n\nMatch User vim\n\tChrootDirectory /var/jail/\n\tX11Forwarding no\n\tAllowTcpForwarding no" >> /etc/ssh/sshd_config

g++ -Wall -O2 gen.cpp
./a.out > /var/jail/home/vim/flag.txt
