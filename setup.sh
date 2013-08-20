#!/bin/bash


aclocal


autoheader 


libtoolize --force --automake


automake -a --gnu --include-deps

./configure && make

echo "-----------------------------"
echo "If everything went OK,"
echo "Run 'make install' as root."
echo "If it *didn't*, run 'make clean'"
echo "and re-run setup.sh."
echo "-----------------------------"
echo ; echo ; echo
