#     -*- Mode: makefile -*-

#     GIFT, a flexible content based image retrieval system.
#     Copyright (C) 1998, 1999, 2000 CUI, University of Geneva

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

#
#Makefile for the inverted File library
#

CCCONFIG=sed "s:__PERL_LOCATION__:@PERL@:g;s:__BINDIR__:@bindir@:g;s:__DATADIR__:@datadir@:g;s:__LIBDIR__:@libdir@:g" < $< > $@

# noinst_LIBRARIES= libMRML.a

lib_LTLIBRARIES = libMRML.la

INCLUDES= -DHAS_BOOL \
	-DLINK_DYNAMICALLY\
	-I$(top_srcdir)/libUnpublished/include \
	-I$(top_srcdir)/libGIFTAcPerl/include -I$(top_srcdir)/libGIFTQuPerl/include \
	-I$(top_srcdir)/libGIFTAcInvertedFile/include -I$(top_srcdir)/libGIFTQuInvertedFile/include \
	-I$(top_srcdir)/libGIFTAcHierarchy/include -I$(top_srcdir)/libGIFTQuHierarchy/include \
	-I$(top_srcdir)/libGIFTQuBayesian/include -I$(top_srcdir)/libGIFTAcDistanceMatrix/include \
	-I$(top_srcdir)/libGIFTAcURL2FTS/include \
	-I$(top_srcdir)/libMRML/include \
	-I$(top_srcdir)/expat/xmlparse -I$(top_srcdir)/expat/xmltok 

libMRML_la_LDFLAGS=@PRE_INSTALL_LIBDIRS@  -version-info 0:1:0

#CAccessorAdminCollection.cc: CAccessorAdminCollection.pre-cc
#	$(CCCONFIG)

# libMRML_a_SOURCES= 

libMRML_la_SOURCES= \
	CAccessorElement.cc\
	\
	CAccessor.cc\
	CAccessorImplementation.cc\
	\
	CAccessorFactoryContainer.cc\
	CAccessorFactory.cc\
	CAFPlugin.cc\
	\
	CRelevanceLevel.cc \
	CRelevanceLevelList.cc \
	\
	CIDRelevanceLevelPair.cc \
	CIDRelevanceLevelPairList.cc \
	\
	CAccessorAdmin.cc \
	CAccessorAdminCollection.cc \
	CPropertySheetList.cc \
	CSessionManager.cc \
	\
	CAlgorithmCollection.cc \
	\
	CDynamicQueryFactory.cc\
	CQueryPlugin.cc\
	CStaticQueryFactory.cc\
	\
	GIFTExceptions.cc \
	\
	CAttributeList.cc \
	CXMLHelper.cc \
	CXMLElement.cc \
	\
	CAlgorithm.cc \
	\
	CQueryTreeNode.cc\
	CQueryTreeBuilder.cc\
	\
	CQuery.cc \
	CQMultiple.cc \
	\
	CMagic.cc\
	\
	CQueryParadigmMatcher.cc\
	\
	mrml_const.cc\
	\
	CXMLElementVisitor.cc\
	CXMLElementBuilder.cc\
	CXEVCommunication.cc\
	\
	CMutex.cc\
	\
	CCommunicationHandler.cc\
	\
	CTimeStampGenerator.cc\
	\
	CDebuggingMemoryManager.cc

#CLEANFILES=CAccessorAdminCollection.cc
