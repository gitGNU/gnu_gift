#! /bin/bash
echo `c++ -g -O2 CCommunicationHandler.o Server.o -lsqlplus -rdynamic -L/usr/local/lib /usr/lib/perl5/5.00503/i586-linux/auto/DynaLoader/DynaLoader.a -L/usr/lib/perl5/5.00503/i586-linux/CORE -lperl -lnsl -lndbm -lgdbm -ldb -ldl -lm -lc -lposix -lcrypt -o gift  ../expat/xmlparse/*.o ../expat/xmltok/*.o  -L../libInvertedFile/cc -L../libUnpublished/cc -L../libBayesian/cc -lInvertedFile -lm  `

