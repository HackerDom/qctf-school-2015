#!/bin/bash

set -e
set -x

userdel vim
useradd -b /home -s `which vim` vim
echo -e "vim:vim" | chpasswd

JAIL="/home/services/school-2015/vim/jail"

rm -r "$JAIL"

mkdir "$JAIL"
debootstrap --arch i386 sid "$JAIL" http://http.debian.net/debian/
mkdir -p "$JAIL"/home/vim

proc "$JAIL/proc proc defaults 0 0" >> /etc/fstab
mount proc "$JAIL"/proc -t proc
echo "sysfs $JAIL/sys sysfs defaults 0 0" >> /etc/fstab
mount sysfs "$JAIL"/sys -t sysfs

cp /etc/hosts "$JAIL"/etc/hosts
cp /etc/passwd "$JAIL"/etc/passwd
cp /proc/mounts "$JAIL"/etc/mtab

chroot "$JAIL" apt-get update
chroot "$JAIL" apt-get install vim

echo -e "\n\nMatch User vim\n\tChrootDirectory $JAIL\n\tX11Forwarding no\n\tAllowTcpForwarding no" >> /etc/ssh/sshd_config

chown -R root:root "$JAIL"
chmod -R a-w /home/services/school-2015/vim/jail/

g++ -Wall -O2 gen.cpp -o gen
./gen > "$JAIL"/home/vim/flag.txt
