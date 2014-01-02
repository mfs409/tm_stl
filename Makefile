#
# Let the user choose 32-bit or 64-bit compilation, but default to 32
#
BITS          ?= 32

#
# Use g++ in C++11 mode, TM enabled.
#
# NB: we use -MD instead of -MMD, because we want to see all the system
# header dependencies.
#
# NB: CXXFLAGS_TM gets us to local copies of the system headers;
#     CXXFLAGS_NOTM replaces transactions with a global mutex
CXX            = g++
CXXFLAGS      += -MD -O2 -fgnu-tm -ggdb -m$(BITS) -std=c++11
CXXFLAGS_TM   += -nostdinc -I/usr/include/ -Iplatform_inc -Istdinc -Ilibinc -Itm_stdinc/
CXXFLAGS_NOTM += -DNO_TM
LDFLAGS       += -m$(BITS) -lstdc++ -fgnu-tm

#
# For now we just have one file that builds our tests, but we compile it in
# two ways
#
CXXFILES       = list_bench
OFILES         = $(patsubst %, %_notm.o, $(CXXFILES)) \
                 $(patsubst %, %_tm.o, $(CXXFILES))
EXEFILES       = $(patsubst %.o, %, $(OFILES))

LIBFILES       = list
LIBOFILES      = $(patsubst %, %_tm.o, $(LIBFILES))

DEPS           = $(patsubst %.o, %.d, $(OFILES) $(LIBOFILES))

#
# Best to be safe...
#
.DEFAULT_GOAL  = all
.PRECIOUS: $(OFILES)
.PHONY: all clean

all: list_bench_tm list_bench_notm

%_tm.o: %.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_TM)

%_notm.o: %.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_NOTM) $(CXXFLAGS_TM)

list_bench_tm: $(LIBOFILES)
%_tm:%_tm.o
	@echo "[LD] $^ $(LIBOFILES) --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

%_notm:%_notm.o
	@echo "[LD] $^ --> $@"
	@$(CXX) $^ -o $@ $(LDFLAGS)

%_tm.o:tm_libstdc++-v3/c++98/%.cc
	@echo "[CXX] $< --> $@"
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_TM)

clean:
	@echo Cleaning up...
	@rm -f $(EXEFILES) $(OFILES) $(DEPS) $(LIBOFILES)

-include $(DEPS)
