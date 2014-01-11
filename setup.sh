#!/bin/bash

yautomake=$(which automake) ; if [ $? - ne 0 ] then echo "install automake" ; exit 1 ; fi

yaclocal=$(which aclocal) ; if [ $? - ne 0 ] then echo "install aclocal" ; exit 1 ; fi

yautoheader=$(which autoheader) ; if [ $? - ne 0 ] then echo "install automake" ; exit 1 ; fi

ylibtoolize=$(which libtoolize)  ; if [ $? - ne 0 ] then echo "install libtool" ; exit 1 ; fi

ylibtoolize --force --automake

yautomake -a --gnu --include-deps

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
