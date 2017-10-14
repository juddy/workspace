#!/bin/bash
#
# Workspace build script
#
# Fri Oct  4 00:54:19 MDT 2013
#
#####################################

echo "----------------------------"
echo
echo " │ ┌─┐ ┌─┐ │ ┌─┐┌─┐┌─┐┌──┌──"
echo "││││ │ ├┬┘├┴┐└─┐├─┘├─┤│  ├─"
echo "└┴┘└─┘ │└─│ │└─┘│  │ │└──└──"
echo
echo "----------------------------"

aclocal 

automake 

./configure

make && sudo make install

echo "-----------------------------------------"
