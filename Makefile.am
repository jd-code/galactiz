## 
## $Id$
## Galactiz Copyright (C) 2005 Cristelle Barillon & Jean-Daniel Pauget
## displaying a galaxy
##
## galactiz@disjunkt.com  -  http://galactiz.disjunkt.com/
## 
## This file is part of Galactiz
## 
## Galactiz is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
## 
## Galactiz is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with Galactiz; if not, write to the Free Software
## Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
## 
## You can also try the web at http://www.gnu.org/
##
## 
## galactiz@disjunkt.com  -  http://galactiz.disjunkt.com/

## probably redundant with somewhere else
## CFLAGS = -O2 -Wall

SUBDIRS = src

vimtest: all
	( export SDL_AUDIODRIVER=dsp && artsdsp ./src/galactiz )
##	./src/testjd

vimtestw32: all
	rm -f ./src/galactiz.exe
	mv ./src/galactiz ./src/galactiz.exe


##  additional file(s) to be removed when making dist...
DISTCLEANFILES = ./src/dpl ./src/crep ./src/galactiz.exe ./src/stderr.txt ./src/lastpop ./src/stdout.txt

