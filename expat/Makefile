CC=gcc
# If you know what your system's byte order is, define BYTE_ORDER:
# use -DBYTE_ORDER=12 for little-endian byte order;
# use -DBYTE_ORDER=21 for big-endian (network) byte order.
# -DXML_NS adds support for checking of lexical aspects of XML namespaces spec
# -DXML_MIN_SIZE makes a smaller but slower parser
CFLAGS=-O2 -Ixmltok -Ixmlparse -DXML_NS
# Use one of the next two lines; unixfilemap is better if it works.
FILEMAP_OBJ=xmlwf/unixfilemap.o
#FILEMAP_OBJ=xmlwf/readfilemap.o
OBJS=xmltok/xmltok.o \
  xmltok/xmlrole.o \
  xmlparse/xmlparse.o \
  xmlparse/hashtable.o \
  $(FILEMAP_OBJ)
EXE=

#deleted 
#  xmlwf/codepage.o \
#  xmlwf/xmlwf.o \
#  xmlwf/xmlfile.o \

all: xmlwf/xmlwf$(EXE)

xmlwf/xmlwf$(EXE): $(OBJS)
	@echo "This makefile severely doctored by Wolfgang Müller"
#	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) xmlwf/xmlwf$(EXE) xmltok/nametab.h gennmtab/gennmtab$(EXE)

distclean: clean


xmltok/nametab.h: gennmtab/gennmtab$(EXE)
	rm -f $@
	gennmtab/gennmtab$(EXE) >$@

gennmtab/gennmtab$(EXE): gennmtab/gennmtab.c
	$(CC) $(CFLAGS) -o $@ gennmtab/gennmtab.c

xmltok/xmltok.o: gennmtab/gennmtab$(EXE) xmltok/nametab.h

check:
	@echo "There is no expat check"

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

install: all
	echo "There is nothing to be installed in this directory"