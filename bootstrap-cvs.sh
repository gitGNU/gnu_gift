#! /bin/sh 
#     GIFT, a flexible content based image retrieval system.
#     Copyright (C) 1998, 1999, 2000, 2001, University of Geneva

#     This program is free software; you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation; either version 2 of the License, or
#     (at your option) any later version.

#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.

#     You should have received a copy of the GNU General Public License
#     along with this program; if not, write to the Free Software
#     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#
# bootstrap for compile process
#

echo "This script generates a configure script for the GIFT"
echo "Use this if you have checked out the GIFT from CVS"

echo "This script will be using the following GNU tools:"
echo "---------------------------------------"
echo `automake --version`
echo `autoconf --version`
echo `libtool --version`
echo "---------------------------------------"

echo "running aclocal"
aclocal || (echo "bootstrap failed, because aclocal failed. exiting. " , exit 100; )
echo "...done"
echo "running automake -a"
automake -a || (echo "bootstrap failed, because automake failed. exiting." , exit 101; )
echo "...done"
echo "running autoconf"
autoconf || (echo "bootstrap failed, because autoconf failed. exiting." , exit 102; )
echo "...done"
echo "Now you should be able to run ./configure"
