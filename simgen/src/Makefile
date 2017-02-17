ARCH = linux

ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS := $(shell root-config --libs)
ROOTGLIBS := $(shell root-config --glibs)

ifeq ($(ARCH),linux)
CXX = g++
CXXFLAGS = -O -Wno-deprecated -fPIC -m32 -fno-inline -Wno-write-strings
LD = g++
LDFLAGS = -O
SOFLAGS = -shared
INCLUDE = $(ROOTSYS)/include/
endif

CXXFLAGS += $(ROOTCFLAGS)
LIBS = $(ROOTLIBS) $(SYSLIBS)
GLIBS = $(ROOTGLIBS) $(SYSLIBS)
EXTRALIBS = -lMinuit -lPythia6 -lEGPythia6 -lEG -L$(PYTHIA6)

#-----------------------------------------------

CXXFILES = EventLoop.cxx
HEADERS =  EventLoop.h

#-----------------------------------------------

All: SimGen.so

SimGen.so: LinkDef.h $(HEADERS) $(CXXFILES)
	@echo "generating dictionary $@..."
	@rootcint -f SimGenDict.cxx -c $(HEADERS) LinkDef.h
	g++ $(CXXFILES) SimGenDict.cxx -shared -o SimGen.so $(CXXFLAGS) $(GLIBS) $(EXTRALIBS)

clean:
	@rm SimGen.so
