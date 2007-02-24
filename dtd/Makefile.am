
noinst_DATA= ${top_builddir}/libMRML/cc/mrml_const.cc

#	
# making a new mrml_const.cc if mrml.dtd has been changed
#
${top_builddir}/libMRML/cc/mrml_const.cc: mrml.dtd
	$(PERL) ${top_builddir}/scripts/perl/gift-dtd-to-keywords.pl ${srcdir}/mrml.dtd mrml mrml_const charmer.mrml
	mv mrml_const.h ${top_builddir}/libMRML/include
	mv mrml_const.cc ${top_builddir}/libMRML/cc
	mv mrml_const.java ${top_builddir}/

EXTRA_DIST=mrml.dtd

data_DATA=mrml.dtd