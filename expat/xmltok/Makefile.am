#     -*- mode: makefile -*-

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

# makefile for xml tokenizer
# the library wont be used, and the resulting objects will be 
# statically linked into the GIFT where needed

INCLUDES= -I../xmltok -DXML_NS
noinst_LTLIBRARIES = lib_temp_xml_tok.la

nametab.h:
	rm -f $@
	../gennmtab/gennmtab >$@


noinst_HEADERS = asciitab.h iasciitab.h latin1tab.h nametab.h utf8tab.h xmldef.h xmlrole.h xmltok.h xmltok_impl.h
lib_temp_xml_tok_la_SOURCES = xmlrole.c xmltok.c
EXTRA_DIST= xmltok_impl.c xmltok_ns.c