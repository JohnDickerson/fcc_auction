CC=g++

# Set environmental variables for CPLEX based on your machine
MACHINE_NAME := $(shell hostname)
PLATFORM := $(shell uname)

ifeq ($(MACHINE_NAME), gs10239.sp.cs.cmu.edu)
SYSTEM = x86-64_sles10_4.1
CPLEXDIR = /usr0/home/jpdicker/ILOG/CPLEX_Studio_AcademicResearch122/cplex
CONCERTDIR = /usr0/home/jpdicker/ILOG/CPLEX_Studio_AcademicResearch122/concert
endif

# John Dickerson's home laptop
ifeq "$(PLATFORM)" "Darwin"
SYSTEM = x86-64_darwin
CPLEXDIR = /Users/spook/Applications/IBM/ILOG/CPLEX_Studio125/cplex
CONCERTDIR = /Users/spook/Applications/IBM/ILOG/CPLEX_Studio125/concert
#SYSTEM = x86-64_darwin9_gcc4.0
#CPLEXDIR = /Users/spook/ILOG/CPLEX_Studio122/cplex
#CONCERTDIR = /Users/spook/ILOG/CPLEX_Studio122/concert
endif



# CPLEX paths -- machine agnostic if the ifeq statements above worked
LIBFORMAT     = static_pic
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
EXDIR         = $(CPLEXDIR)/examples
EXSRC         = $(EXDIR)/src
EXINC         = $(EXDIR)/include
EXDATA        = $(EXDIR)/data


# Compile and link with CPLEX (and whatever other G++ flags)
CCFLAGS   = -fPIC -fexceptions -DNDEBUG -O3 -DIL_STD -g -Wall
CCFLAGS  += -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 
CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread -lstdc++   



SRCFILES=Driver.cc Auction.cc AuctionCPLEXMIP.cc CSV.cc
OBJFILES=$(SRCFILES:.cc=.o)
EXEFILE=fcc_auction

all: $(SRCFILES) $(EXEFILE)

$(EXEFILE):  $(OBJFILES)
	$(CC) $(CCFLAGS) -o $@ $(OBJFILES) $(CCLNFLAGS)

.cc.o:
	$(CC) -c $(CCFLAGS) -o $@ $<

clean:
	rm -f *.o *~ $(EXEFILE)
