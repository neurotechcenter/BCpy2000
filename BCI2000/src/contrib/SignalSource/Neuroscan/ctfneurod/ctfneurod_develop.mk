#
# Makefile for ctfneurod application.
#

#Get revision from revision file.
include $(CTFAPPDEVDIR)/revision

EXECUTABLE      =       ctfneurod

APPVPATH = \
	  $(CTFAPPDEVDIR)/ctfneurod/ACQTX  \

EXTRA_FLAGS     =       -DAPPNAME="\"$(EXECUTABLE)\"" \
				-DREVISION="\"$(APPREVISION)\"" \
				-DGDSQSYS \
				-DFIX_ADC_OFFSET




MAIN = ctfneurod.cc


CXXSRC  =       ACQ_MessagePacketQueue.cc \
		CtfNeuroSrv.cc \
		NeuroSrv.cc \
		SockStream.cc \
		EDFHeader.cc \
			$(MAIN)


XXOBJS  =\
			$(MAIN:.cc=.o)

OBJS    =\
			$(CXXSRC:.cc=.o) -static

all: ver $(EXECUTABLE)

include $(CTFMAKEDIR)/MakeAppVerbose.develop

