
parsedir=$(includedir)/expat/xmlparse

INCLUDES= -I../xmltok -DXML_NS

parse_HEADERS = hashtable.h xmlparse.h
noinst_LTLIBRARIES = lib_temp_xml_parse.la
lib_temp_xml_parse_la_LIBS = -k -l_temp_xml_tok
lib_temp_xml_parse_la_SOURCES = xmlparse.c  hashtable.c

