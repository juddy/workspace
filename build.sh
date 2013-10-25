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
toilet -f future $(grep AC_INIT configure.ac | awk -F, '{print $2}')
echo
echo
echo "----------------------------"

sleep 2

toilet -f emboss "configure"
./configure


toilet -f emboss "make"
colormake -j3

toilet -f emboss "make install"
sudo make install

echo " │ ┌─┐ ┌─┐ │ ┌─┐┌─┐┌─┐┌──┌──"
echo "││││ │ ├┬┘├┴┐└─┐├─┘├─┤│  ├─"
echo "└┴┘└─┘ │└─│ │└─┘│  │ │└──└──"
echo
toilet -f future $(grep AC_INIT configure.ac | awk -F, '{print $2}')

echo "-----------------------------------------"
