#!/bin/bash
aclocal

autoheader 

libtoolize --force --automake

automake -a --gnu --include-deps

./configure && make

if [ $? -ne 0 ]
then

echo "-----------------------------"
echo "Run 'make clean'"
echo "and re-run setup.sh."
echo "-----------------------------"
echo ; echo ; echo

else

echo "-----------------------------"
echo "Run 'make install' as root."
echo "Otherwise run 'make clean'"
echo "and re-run $0"
echo "-----------------------------"
echo ; echo ; echo

fi
