
directory.h:
	echo "Making directory.h"
	@rm -f directory.h;echo '#define __LIBDIR__ "@libdir@"' > directory.h ; echo '#define __PERL_LOCATION__ "@PERL@"' >> directory.h; echo '#define __BINDIR__ "@bindir@"' >> directory.h ; echo '#define __DATADIR__ "@datadir@"'    >> directory.h; echo '#define __LIBDIR__  "@libdir@"'     >> directory.h

all: directory.h

noinst_HEADERS= directory.h
CLEANFILES= directory.h