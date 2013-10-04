#!/bin/bash
#
# Workspace build script
#
# Fri Oct  4 00:54:19 MDT 2013
#
#####################################


./configure

colormake -j3

sudo make install
