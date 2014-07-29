#
# Set up build folders
#
ODIR64 := ./obj64
ODIR32 := ./obj32
o64_folder := $(shell mkdir -p $(ODIR64))
o32_folder := $(shell mkdir -p $(ODIR32))

#
# Set up tools
#
CXX=gcc

#
# All paths and flags
#
INC64=-I../../libgcc -I../include/ -I./ -I../include/x86_64-unknown-linux-gnu -I../include -I./
INC64LITE=-I../include/x86_64-unknown-linux-gnu -I../include -I./ -I.. -I../../libiberty -I../../include

INC32=-I../../libgcc -I../include/ -I./ -I../include/x86_64-unknown-linux-gnu -I../include -I./
INC32LITE=-I../include/x86_64-unknown-linux-gnu -I../include -I./ -I.. -I../../libiberty -I../../include

FLAGS1=-D_GLIBCXX_SHARED -fno-implicit-templates -Wall -Wextra -Wwrite-strings -Wcast-qual -Wabi -fdiagnostics-show-location=once -ffunction-sections -fdata-sections
FLAGS2=-g -O2 -D_GNU_SOURCE
FLAGS3=-fPIC -DPIC -D_GLIBCXX_SHARED

BFLAGS1=
BFLAGS2=

#
# File names
#
NAMESx98 = array_type_info  atexit_arm bad_alloc bad_cast bad_typeid class_type_info del_op del_opnt del_opv del_opvnt dyncast eh_alloc eh_arm eh_call eh_catch eh_exception eh_globals eh_personality eh_term_handler eh_tm eh_type eh_unex_handler enum_type_info function_type_info fundamental_type_info guard_error hash_bytes new_opv new_opvnt pbase_type_info pmem_type_info pointer_type_info pure si_class_type_info tinfo tinfo2 vec vmi_class_type_info vterminate
NAMESx11 = atexit_thread bad_array_length bad_array_new eh_aux_runtime eh_ptr eh_terminate eh_throw guard nested_exception new_handler new_op new_opnt
NAMESiberty = cp-demangle
NAMESLIB = libsupc++convenience libsupc++

#
# Name manipulations
#
O64x98 = $(patsubst %, $(ODIR64)/%.o, $(NAMESx98))
O64x11 = $(patsubst %, $(ODIR64)/%.o, $(NAMESx11))
O64iberty = $(patsubst %, $(ODIR64)/%.o, $(NAMESiberty))

O32x98 = $(patsubst %, $(ODIR32)/%.o, $(NAMESx98))
O32x11 = $(patsubst %, $(ODIR32)/%.o, $(NAMESx11))
O32iberty = $(patsubst %, $(ODIR32)/%.o, $(NAMESiberty))

O64A = $(patsubst %, $(ODIR64)/%.a, $(NAMESLIB))
O32A = $(patsubst %, $(ODIR32)/%.a, $(NAMESLIB))

#
# Target info
#
.DEFAULT_GOAL = all
.PHONY: all clean

#
# Targets
#
all: $(O64A) $(O32A)

clean:
	rm -rf $(ODIR32) $(ODIR64)

$(O64x98): $(ODIR64)/%.o: %.cc
	$(CXX) $(BFLAGS1) $(BFLAGS2) -shared-libgcc -nostdinc++ $(INC64) $(FLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(FLAGS2) $(FLAGS3) -c $< -o $@

$(O64x11): $(ODIR64)/%.o: %.cc
	$(CXX) $(BFLAGS1) $(BFLAGS2) -shared-libgcc -nostdinc++ $(INC64) $(FLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(FLAGS2) $(FLAGS3) -std=gnu++11 -c $< -o $@

$(O64iberty): $(ODIR64)/%.o: ../../libiberty/%.c
	$(CXX) $(BFLAGS1) $(BFLAGS2) -DHAVE_CONFIG_H -D_GLIBCXX_SHARED $(INC64LITE) -g -O2 -DIN_GLIBCPP_V3 -Wno-error -fPIC -DPIC -c $< -o $@

$(ODIR64)/%.a: $(O64x98) $(O64x11) $(O64iberty)
	ar rc $@ $^
	ranlib $@

$(O32x98): $(ODIR32)/%.o: %.cc
	$(CXX) $(BFLAGS1) $(BFLAGS2) -shared-libgcc -nostdinc++ -m32 $(INC32) $(FLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(FLAGS2) $(FLAGS3) -c $< -o $@

$(O32x11): $(ODIR32)/%.o: %.cc
	$(CXX) $(BFLAGS1) $(BFLAGS2) -shared-libgcc -nostdinc++ -m32 $(INC32) $(FLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(FLAGS2) $(FLAGS3) -std=gnu++11 -c $< -o $@

$(O32iberty): $(ODIR32)/%.o: ../../libiberty/%.c
	$(CXX) $(BFLAGS1) $(BFLAGS2) -m32 -DHAVE_CONFIG_H -D_GLIBCXX_SHARED $(INC32LITE) -g -O2 -DIN_GLIBCPP_V3 -Wno-error -fPIC -DPIC -c $< -o $@

$(ODIR32)/%.a: $(O32x98) $(O32x11) $(O32iberty)
	ar rc $@ $^
	ranlib $@
