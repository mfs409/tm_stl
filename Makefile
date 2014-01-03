#
# Set up directories
#
ODIR := obj
IDIR := inc
LDIR := lib
SDIR := src
output_folder := $(shell mkdir -p $(ODIR))

#
# Let the user choose 32-bit or 64-bit compilation, but default to 32
#
BITS          ?= 32

#
# Set up all filenames
#
CXXFILES       = list_bench assign cap ctor element iter modifier
LIBFILES       = list functexcept snprintf_lite
IFOLDERS       = platform_inc stdinc libinc tm_stdinc tm_platform_inc

TM_OFILES      = $(patsubst %, $(ODIR)/%_tm.o, $(CXXFILES) $(LIBFILES))
NOTM_OFILES    = $(patsubst %, $(ODIR)/%_notm.o, $(CXXFILES))

EXEFILES       = $(ODIR)/list_bench_tm $(ODIR)/list_bench_notm

DEPS           = $(patsubst %.o, %.d, $(TM_OFILES) $(NOTM_OFILES))

#
# Use g++ in C++11 mode, TM enabled.
#
# NB: we use -MD instead of -MMD, because we want to see all the system
# header dependencies.
CXX            = g++
CXXFLAGS      += -MD -O2 -fgnu-tm -ggdb -m$(BITS) -std=c++11 -nostdinc
CXXFLAGS      += -I/usr/include/ $(patsubst %, -I$(IDIR)/%, $(IFOLDERS))
CXXFLAGS_NOTM  = -DNO_TM
LDFLAGS       += -m$(BITS) -lstdc++ -fgnu-tm

#
# Best to be safe...
#
.DEFAULT_GOAL  = all
.PRECIOUS: $(patsubst %, $(ODIR)/%, $(TM_OFILES) $(NOTM_OFILES))
.PHONY: all clean

all: $(EXEFILES)

$(ODIR)/%_tm.o: $(SDIR)/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ODIR)/%_notm.o: $(SDIR)/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_NOTM)

$(ODIR)/list_bench_tm:   $(TM_OFILES)
$(ODIR)/%_tm:$(ODIR)/%_tm.o
	@echo "[LD] $^ $(LIBOFILES) --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

$(ODIR)/list_bench_notm: $(NOTM_OFILES)
$(ODIR)/%_notm:$(ODIR)/%_notm.o
	@echo "[LD] $^ --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

$(ODIR)/%_tm.o:$(LDIR)/tm_libstdc++-v3/c++98/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_TM)

$(ODIR)/%_tm.o:$(LDIR)/tm_libstdc++-v3/c++11/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_TM)

clean:
	@echo Cleaning up...
	@rm -rf $(ODIR)

-include $(DEPS)
