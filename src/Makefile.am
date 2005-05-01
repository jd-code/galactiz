## 
## $Id$
## Galactiz Copyright (C) 2005 Cristelle Barillon & Jean-Daniel Pauget
## displaying a galaxy
##
## galactiz@disjunkt.com  -  http://galactiz.disjunkt.com/
## 
## This file is part of Galactiz.
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

# SUBDIRS = bzouz

# was in the first shmilblick (SDLgears)
# EXTRA_DIST = \
#	autogen.sh gears.c

bin_PROGRAMS = galactiz

##		    graphutils.c jeuchar.c graphprim.cpp galactiz.cpp \
##		    samplesonge.cpp audio.cpp conv_ieee_ext.c readsample.cpp asequencer.cpp \

galactiz_SOURCES = rcfile.cpp \
		    tdgal.cpp \
		 morespin.cpp \
	      fakexmlread.cpp \
	         sxd_read.cpp \
		     main.cpp

galactiz_LDFLAGS = --enable-auto-import

galactiz_LDADD = @SDL_LIBS@ @GL_LIBS@ @GRAPEFRUIT_LIBS@ @CAROUBE_LIBS@

## galactiz_LDADD = @GL_LIBS@

