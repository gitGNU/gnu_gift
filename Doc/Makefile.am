

gift-guide.info: gift-guide.sgml 
	-test "@SGML2INFO@" && @SGML2INFO@ gift-guide.sgml

configuring-and-hacking-the-gift.info: configuring-and-hacking-the-gift.sgml
	-test "@SGML2INFO@" && @SGML2INFO@ configuring-and-hacking-the-gift.sgml

gift-guide.txt: gift-guide.sgml 
	-test "@SGML2TXT@" gift-guide.sgml

configuring-and-hacking-the-gift.txt: configuring-and-hacking-the-gift.sgml
	-test "@SGML2TXT@" && @SGML2TXT@ configuring-and-hacking-the-gift.sgml

gift-guide.dvi: gift-guide.sgml
	-test "@SGML2LATEX@"  && @SGML2LATEX@ gift-guide.sgml

configuring-and-hacking-the-gift.dvi: configuring-and-hacking-the-gift.sgml
	-test "@SGML2LATEX@"  && @SGML2LATEX@ configuring-and-hacking-the-gift.sgml

system-doc: Doxyfile 
	-mkdir autoDoc
	-mkdir autoDoc/HTML
	-test "@DOXYGEN@" && @DOXYGEN@ 

noinst_DATA =  system-doc configuring-and-hacking-the-gift.info configuring-and-hacking-the-gift.txt configuring-and-hacking-the-gift.dvi gift-guide.dvi gift-guide.txt gift-guide.info

CLEANFILES= gift-guide.info gift-guide.dvi configuring-and-hacking-the-gift.info configuring-and-hacking-the-gift.dvi autoDoc/HTML/* autoDoc/rtf/* autoDoc/man/*/* autoDoc/latex/*

EXTRA_DIST= Doxyfile HTML TeX CodingRules GIFTClassGraph configuring-and-hacking-the-gift.sgml gift-guide.sgml