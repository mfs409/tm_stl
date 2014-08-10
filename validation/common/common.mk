#
# Let the user choose 32-bit or 64-bit compilation, but default to 32
#
BITS          ?= 32

#
# Get configuration
#
include ../../config.mk

#
# Directory Names
#
ODIR := ./obj$(BITS)
output_folder := $(shell mkdir -p $(ODIR))

#
# Names of files that the compiler generates
#
EXEFILES       = $(ODIR)/bench_tm $(ODIR)/bench_notm $(ODIR)/bench_trace
TM_OFILES      = $(patsubst %, $(ODIR)/%_tm.o, $(CXXFILES))
NOTM_OFILES    = $(patsubst %, $(ODIR)/%_notm.o, $(CXXFILES))
TRACE_OFILES   = $(patsubst %, $(ODIR)/%_trace.o, $(CXXFILES))
DEPS           = $(patsubst %.o, %.d, $(TM_OFILES) $(NOTM_OFILES) $(TRACE_OFILES))

#
# Use g++ in C++11 mode to build the "original" nontransactional version of
# the code
#
# NB: we use -MD instead of -MMD, because we want to see all the system
# header dependencies... we're cloning the system headers and modifying them,
# and want changes to force recompilation.
CXX            = g++

CXXFLAGS_NOTM  = -MD -O2 -ggdb -m$(BITS) -std=c++11 -nostdinc                       \
                 -I/usr/include/ -I../../libstdc++/libstdc++-v3/include                \
                 -I../../libstdc++/libstdc++-v3/include/x86_64-unknown-linux-gnu       \
                 -I../../libstdc++/libstdc++-v3/libsupc++                              \
                 -I$(GCC410INSTALL)/lib/gcc/x86_64-unknown-linux-gnu/4.10.0/include \
                 -DNO_TM -pthread

CXXFLAGS_TM    = -MD -O2 -fgnu-tm -ggdb -m$(BITS) -std=c++11 -nostdinc              \
                 -I/usr/include/ -I../../libstdc++_tm/libstdc++-v3/include             \
                 -I../../libstdc++_tm/libstdc++-v3/include/x86_64-unknown-linux-gnu    \
                 -I../../libstdc++_tm/libstdc++-v3/libsupc++                           \
                 -I$(GCC410INSTALL)/lib/gcc/x86_64-unknown-linux-gnu/4.10.0/include \
                 -DUSE_TM -pthread

CXXFLAGS_TRACE = -MD -O2 -ggdb -m$(BITS) -std=c++11 -nostdinc                       \
                 -include ../../libstdc++_trace/trace.h                                \
                 -I/usr/include/ -I../../libstdc++_trace/libstdc++-v3/include          \
                 -I../../libstdc++_trace/libstdc++-v3/include/x86_64-unknown-linux-gnu \
                 -I../../libstdc++_trace/libstdc++-v3/libsupc++                        \
                 -I$(GCC410INSTALL)/lib/gcc/x86_64-unknown-linux-gnu/4.10.0/include \
                 -DNO_TM -pthread

LDFLAGS_NOTM   = -m$(BITS) -L../../libstdc++/libstdc++-v3/src/obj$(BITS) -lstdc++ -pthread
LDFLAGS_TM     = -m$(BITS) -fgnu-tm -L../../libstdc++_tm/libstdc++-v3/src/obj$(BITS) -lstdc++ -pthread
LDFLAGS_TRACE  = -m$(BITS) -L../../libstdc++_trace/libstdc++-v3/src/obj$(BITS) -lstdc++ -pthread

#
# Best to be safe...
#
.DEFAULT_GOAL  = all
.PRECIOUS: $(TM_OFILES) $(NOTM_OFILES) $(TRACE_OFILES)
.PHONY: all clean

#
# Goal is to build three executables
#
all: $(EXEFILES)

#
# Rules for building .o files from sources
#
$(ODIR)/%_tm.o: %.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS_TM)

$(ODIR)/%_notm.o: %.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS_NOTM)

$(ODIR)/%_trace.o: %.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS_TRACE)

#
# Rules for building executables
#
$(ODIR)/bench_tm: $(TM_OFILES)
$(ODIR)/%_tm:$(ODIR)/%_tm.o
	@echo "[LD] $^ --> $@"
	$(CXX) $^ -o $@ $(LDFLAGS_TM)

$(ODIR)/bench_notm: $(NOTM_OFILES)
$(ODIR)/%_notm:$(ODIR)/%_notm.o
	@echo "[LD] $^ --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS_NOTM)

$(ODIR)/bench_trace: $(TRACE_OFILES)
$(ODIR)/%_trace:$(ODIR)/%_trace.o
	@echo "[LD] $^ --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS_TRACE)

#
# We'll be lazy... to clean, we'll just clobber the build folder
#
clean:
	@echo Cleaning up...
	@rm -rf $(ODIR)

#
# Include dependencies
#
-include $(DEPS)
