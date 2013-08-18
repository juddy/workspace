#!/bin/bash


aclocal


autoheader 


libtoolize --force --automake


automake -a --gnu --include-deps

./configure && make

echo "-----------------------------"
echo "Run 'make install' as root."
echo "-----------------------------"
echo ; echo ; echo
