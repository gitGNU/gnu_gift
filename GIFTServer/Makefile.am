
# the bin programs to be made in this directory

SUBDIRS = gift-config gift-mrml-messages

# data_DATA= gift-config gift-mrml-messages

bin_PROGRAMS = gift gift-endianize gift-generate-inverted-file gift-modify-distance-matrix gift-one-minus

# generateDistanceMatrix
# should be generated, too, but
# at present this file is not maintained.
# it is not necessary for running the basic version of gift

# include directories for all programs in this dir

INCLUDES = -I$(top_srcdir)/libMRML/include -I$(top_srcdir)/libGIFTAcInvertedFile/include -I$(top_srcdir)/libGIFTAcURL2FTS/include -I$(srcdir) -I../expat/xmlparse -I../expat/xmltok -I.

# the gift server

#
# remark: The perl accessor has to be linked at compile time,
# as it is unlikely that the user has a shared perl library
# installed.
#
# The -L lines will be gone when libtool 1.4 is shipping
# 1.3.5 does needs them
#
#
gift_LDADD = ../expat/xmlparse/*.o ../expat/xmltok/*.o  \
	@PRE_INSTALL_LIBDIRS@ @PRE_INSTALL_LIBDIRS@\
	-L@UNPUBLISHED_DIR@ @UNPUBLISHED@ @BAYESIAN@ \
        -lGIFTQuPerl -lGIFTAcPerl -lMRML\
	@BAYESIAN_LD_ADD@\
	@PERL_LD_ADD@ 

# -lGIFTAcURL2FTS -lGIFTAcInvertedFile \
# -lGIFTAcHierarchy \
# -lGIFTQuInvertedFile  -lGIFTQuHierarchy

#@PERL_LD_ADD@

#`echo $LDFLAGS \`perl -MExtUtils::Embed -e ldopts  \`|sed 's:-Dbool=char::'` 


gift_SOURCES = Server.cc

# generate inverted file

gift_generate_inverted_file_LDADD = @PRE_INSTALL_LIBDIRS@  -L@UNPUBLISHED_DIR@ @BAYESIAN@ @UNPUBLISHED@ -lMRML\
	 -lGIFTAcURL2FTS -lGIFTAcInvertedFile -lGIFTQuInvertedFile\
	 -lGIFTAcPerl -lGIFTQuPerl\
	 -lGIFTAcHierarchy -lGIFTQuHierarchy\
	 @BAYESIAN_LD_ADD@\
	 @PERL_LD_ADD@ 

gift_generate_inverted_file_LDFLAGS = ../expat/xmlparse/*.o ../expat/xmltok/*.o

gift_generate_inverted_file_SOURCES=generateInvertedFile.cc 


# change the "endianicity" 
# (useful when exchanging files across a heterogeneous network)
# relies on files consisting of 4-byte-variables only
gift_endianize_SOURCES = endianize.cc

# Generate a distance matrix from a given database
# (this is needed for the scheme in libBayesian.
# we suggest this for research only. it was mainly
# for having a reasonably fast experimental platform
# that I (WM) did this.
# generateDistanceMatrix_SOURCES = generateDistanceMatrix.cc
# generateDistanceMatrix_LDADD = -L../libGIFTInvertedFile/cc -L../libBayesian/cc -lInvertedFile @BAYESIAN@  

# symmetrify raw distance matrix (or things like that)
gift_modify_distance_matrix_SOURCES = modifyDistanceMatrix.cc


# for curing sign (+-) errors in the distance matrix. 
# it reads a sequence of floats and puts out 
# (1-x) for each float x read
# YESSIR, this is a HACK. 
gift_one_minus_SOURCES = oneMinus.cc





# DO NOT DELETE THIS LINE -- make depend depends on it.
