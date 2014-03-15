#
# Let the user choose 32-bit or 64-bit compilation, but default to 32
#
BITS          ?= 32

#
# Directory Names
#
ODIR := ./obj
IDIR := ../inc
LDIR := ../lib
SDIR := ./
output_folder := $(shell mkdir -p $(ODIR))

#
# Include folder names
#
IFOLDERS       = platform_inc stdinc libinc

#
# Names of files that the compiler generates
#
EXEFILES       = $(ODIR)/bench_tm $(ODIR)/bench_notm
TM_OFILES      = $(patsubst %, $(ODIR)/%_tm.o, $(CXXFILES) $(LIBFILES))
NOTM_OFILES    = $(patsubst %, $(ODIR)/%_notm.o, $(CXXFILES) $(LIBFILES))
DEPS           = $(patsubst %.o, %.d, $(TM_OFILES) $(NOTM_OFILES))

#
# Use g++ in C++11 mode, TM enabled.
#
# NB: we use -MD instead of -MMD, because we want to see all the system
# header dependencies... we're cloning the system headers and modifying them,
# and want changes to force recompilation.
CXX            = g++
CXXFLAGS       = -MD -O2 -fgnu-tm -ggdb -m$(BITS) -std=c++11 -nostdinc \
                 -include ../inc/local/trace.h -include ../inc/local/toggle.h \
                 -I/usr/include/ $(patsubst %, -I$(IDIR)/%, $(IFOLDERS))
CXXFLAGS_NOTM  = -DNO_TM
LDFLAGS        = -m$(BITS) -lstdc++ -fgnu-tm

#
# Best to be safe...
#
.DEFAULT_GOAL  = all
.PRECIOUS: $(TM_OFILES) $(NOTM_OFILES)
.PHONY: all clean

#
# Goal is to build two executables
#
all: $(EXEFILES)

#
# Rules for building .o files from sources
#
$(ODIR)/%_tm.o: $(SDIR)/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/%_tm.o:$(LDIR)/libstdc++-v3/c++98/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/%_tm.o:$(LDIR)/libstdc++-v3/c++11/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/%_notm.o: $(SDIR)/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_NOTM)

$(ODIR)/%_notm.o:$(LDIR)/libstdc++-v3/c++98/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_NOTM)

$(ODIR)/%_notm.o:$(LDIR)/libstdc++-v3/c++11/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_NOTM)

#
# Rules for building executables
#
$(ODIR)/bench_tm:   $(TM_OFILES)
$(ODIR)/%_tm:$(ODIR)/%_tm.o
	@echo "[LD] $^ --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

$(ODIR)/bench_notm: $(NOTM_OFILES)
$(ODIR)/%_notm:$(ODIR)/%_notm.o
	@echo "[LD] $^ --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

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
