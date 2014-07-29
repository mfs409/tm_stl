#
# Pull in global configuration
#
-include ../../../config.mk

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
LFLAGS64=-L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -fgnu-tm
LFLAGS32= -L/lib32 -L/usr/lib32 -fgnu-tm

IFLAGS1=-I../../libgcc -I../include/x86_64-unknown-linux-gnu -I../include -I../libsupc++
IFLAGS2=-I../include/backward

CFLAGS1=-fno-implicit-templates -Wall -Wextra -Wwrite-strings -Wcast-qual -Wabi -fdiagnostics-show-location=once -ffunction-sections -fdata-sections -fgnu-tm
CFLAGS2=-g -O2 -D_GNU_SOURCE -fgnu-tm
CFLAGS3=-fPIC -DPIC -D_GLIBCXX_SHARED -fgnu-tm

#
# Path to special crt*.o files
#
CRTPATH=$(GCC410INSTALL)/lib/gcc/x86_64-unknown-linux-gnu/4.10.0/

#
# File names
#
NAMESx98 = bitmap_allocator pool_allocator mt_allocator codecvt complex_io ctype globals_io hash_tr1 hashtable_tr1 ios ios_failure ios_init ios_locale list list-aux list-aux-2 list_associated list_associated-2 locale locale_init locale_facets localename math_stubs_float math_stubs_long_double stdexcept tree istream streambuf valarray atomicity codecvt_members collate_members ctype_configure_char ctype_members messages_members monetary_members numeric_members time_members basic_file c++locale allocator-inst ext-inst ios-inst iostream-inst istream-inst locale-inst misc-inst ostream-inst sstream-inst streambuf-inst wlocale-inst
NAMESx98custom = strstream concept-inst parallel_settings
NAMESx11 = chrono condition_variable debug functexcept functional future hash_c++0x limits mutex placeholders random regex shared_ptr snprintf_lite system_error thread fstream-inst string-inst wstring-inst
NAMESx11custom = hashtable_c++0x
NAMESsrc98 = compatibility compatibility-debug_list compatibility-debug_list-2
NAMESsrc11 = compatibility-c++0x compatibility-atomic-c++0x compatibility-thread-c++0x compatibility-chrono compatibility-condvar
NAMESA = libc++98convenience libc++11convenience
NAMESAA = libstdc++
NAMESSO = libstdc++.so.6.0.21

#
# Name manipulations
#
O64x98 = $(patsubst %, $(ODIR64)/%.o, $(NAMESx98) $(NAMESx98custom))
O64x98lite = $(patsubst %, $(ODIR64)/%.o, $(NAMESx98))
O64x11 = $(patsubst %, $(ODIR64)/%.o, $(NAMESx11) $(NAMESx11custom))
O64x11lite = $(patsubst %, $(ODIR64)/%.o, $(NAMESx11))
O64src98p = $(patsubst %, $(ODIR64)/pic_%.o, $(NAMESsrc98))
O64src11p = $(patsubst %, $(ODIR64)/pic_%.o, $(NAMESsrc11))
O64src98n = $(patsubst %, $(ODIR64)/nopic_%.o, $(NAMESsrc98))
O64src11n = $(patsubst %, $(ODIR64)/nopic_%.o, $(NAMESsrc11))
O64A   = $(patsubst %, $(ODIR64)/%.a, $(NAMESA))
O64AA  = $(patsubst %, $(ODIR64)/%.a, $(NAMESAA))
O64SO  = $(patsubst %, $(ODIR64)/%.so, $(NAMESSO))

O32x98 = $(patsubst %, $(ODIR32)/%.o, $(NAMESx98) $(NAMESx98custom))
O32x98lite = $(patsubst %, $(ODIR32)/%.o, $(NAMESx98))
O32x11 = $(patsubst %, $(ODIR32)/%.o, $(NAMESx11) $(NAMESx11custom))
O32x11lite = $(patsubst %, $(ODIR32)/%.o, $(NAMESx11))
O32src98p = $(patsubst %, $(ODIR32)/pic_%.o, $(NAMESsrc98))
O32src11p = $(patsubst %, $(ODIR32)/pic_%.o, $(NAMESsrc11))
O32src98n = $(patsubst %, $(ODIR32)/nopic_%.o, $(NAMESsrc98))
O32src11n = $(patsubst %, $(ODIR32)/nopic_%.o, $(NAMESsrc11))
O32A   = $(patsubst %, $(ODIR32)/%.a, $(NAMESA))
O32AA  = $(patsubst %, $(ODIR32)/%.a, $(NAMESAA))
O32SO  = $(patsubst %, $(ODIR32)/%.so, $(NAMESSO))

#
# Targets
#
all: $(O64SO) $(O64AA) $(O32SO) $(O32AA) 

clean:
	rm -rf $(ODIR32) $(ODIR64)

#
# c++98 folder, 64-bit
#
$(O64x98lite): $(ODIR64)/%.o: c++98/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     $(IFLAGS1) -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(CFLAGS2) -c $< $(CFLAGS3) -o $@

$(ODIR64)/strstream.o: c++98/strstream.cc
	$(CXX) -shared-libgcc  -nostdinc++     $(IFLAGS1) -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(CFLAGS2) $(IFLAGS2) -Wno-deprecated -c $<  $(CFLAGS3) -o $@

$(ODIR64)/concept-inst.o: c++98/concept-inst.cc
	$(CXX) -shared-libgcc  -nostdinc++     $(IFLAGS1) -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(CFLAGS2) -D_GLIBCXX_CONCEPT_CHECKS -fimplicit-templates -c $<  $(CFLAGS3) -o $@

$(ODIR64)/parallel_settings.o: c++98/parallel_settings.cc
	$(CXX) -shared-libgcc  -nostdinc++     $(IFLAGS1) -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(CFLAGS2) -D_GLIBCXX_PARALLEL -c $<  $(CFLAGS3) -o $@

$(ODIR64)/libc++98convenience.a: $(O64x98)
	ar rc $@ $^
	ranlib $@

#
# c++11 folder, 64-bit
#
$(O64x11lite): $(ODIR64)/%.o: c++11/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     $(IFLAGS1) -std=gnu++11 -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(CFLAGS2) -c $< $(CFLAGS3) -o $@

$(ODIR64)/hashtable_c++0x.o: c++11/hashtable_c++0x.cc
	$(CXX) -shared-libgcc  -nostdinc++     $(IFLAGS1) -std=gnu++11 -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(nodir $@)).lo $(CFLAGS2) -fimplicit-templates -c $<  $(CFLAGS3) -o $@

$(ODIR64)/libc++11convenience.a: $(O64x11)
	ar rc $@ $^
	ranlib $@

#
# ./ folder, 64-bit
#
$(O64src98p): $(ODIR64)/pic_%.o: c++98/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     $(IFLAGS1) -fPIC -DPIC $(CFLAGS1) -frandom-seed=$(basename $(nodir $<)).lo $(CFLAGS2) -c $<  $(CFLAGS3) -o $@

$(O64src98n): $(ODIR64)/nopic_%.o: c++98/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     $(IFLAGS1) -fPIC -DPIC $(CFLAGS1) -frandom-seed=$(basename $(nodir $<)).lo $(CFLAGS2) -c $< -o $@

$(O64src11p): $(ODIR64)/pic_%.o: c++11/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     $(IFLAGS1) -fPIC -DPIC $(CFLAGS1) -frandom-seed=$(basename $(nodir $<)).lo $(CFLAGS2) -std=gnu++11 -c $<  $(CFLAGS3) -o $@

$(O64src11n): $(ODIR64)/nopic_%.o: c++11/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     $(IFLAGS1) -fPIC -DPIC $(CFLAGS1) -frandom-seed=$(basename $(noidr $<)).lo $(CFLAGS2) -std=gnu++11 -c $< -o $@

#
# 64-bit link rules
#
$(O64SO): $(O64A) $(O64src98p) $(O64src11p)
	cp gnu.ver libstdc++-symbols.ver64.tmp
	chmod +w libstdc++-symbols.ver64.tmp
	if test "x" != x; then \
		if grep '^# Appended to version file.' \
		/dev/null > /dev/null 2>&1; then \
			cat  >> libstdc++-symbols.ver64.tmp; \
		else \
			sed -n '1,/DO NOT DELETE/p' libstdc++-symbols.ver64.tmp > tmp.top; \
			sed -n '/DO NOT DELETE/,$$p' libstdc++-symbols.ver64.tmp > tmp.bottom; \
			cat tmp.top  tmp.bottom > libstdc++-symbols.ver64.tmp; \
			rm tmp.top tmp.bottom; \
		fi; \
	fi
	/bin/grep -E -v '^[ 	]*#(#| |$$)' libstdc++-symbols.ver64.tmp | gcc        -E -P -include ../config.h - > libstdc++-symbols.ver64 || (rm -f libstdc++-symbols.ver64 ; exit 1)
	rm -f libstdc++-symbols.ver64.tmp
	$(CXX) -shared-libgcc  -nostdinc++  -shared -nostdlib /usr/lib/x86_64-linux-gnu/crti.o $(CRTPATH)/crtbeginS.o  $(O64src98p) $(O64src11p) -Wl,--whole-archive ../libsupc++/$(ODIR64)/libsupc++convenience.a $(O64A) -Wl,--no-whole-archive  -lm $(LFLAGS64) -lc -lgcc_s $(CRTPATH)/crtendS.o /usr/lib/x86_64-linux-gnu/crtn.o  -Wl,-O1 -Wl,-z -Wl,relro -Wl,--gc-sections -Wl,--version-script=libstdc++-symbols.ver64   -Wl,-soname -Wl,$(nodir $@) -o $@
	rm libstdc++-symbols.ver64

$(O64AA): $(O64A) $(O64src98n) $(O64src11n)
	mkdir -p $(ODIR64)/extra
	cd $(ODIR64)/extra && ar x ../../../libsupc++/$(ODIR64)/libsupc++convenience.a
	ar rc $@  $(O64src98n) $(O64src11n) $(ODIR64)/extra/*.o $(O64x98) $(O64x11)
	ranlib $@
	rm -rf $(ODIR64)/extra

#
# c++98 folder, 32-bit
#
$(O32x98lite): $(ODIR32)/%.o: c++98/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     -m32 $(IFLAGS1) -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(CFLAGS2) -c $< $(CFLAGS3) -o $@

$(ODIR32)/strstream.o: c++98/strstream.cc
	$(CXX) -shared-libgcc  -nostdinc++     -m32 $(IFLAGS1) -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(CFLAGS2) $(IFLAGS2) -Wno-deprecated -c $<  $(CFLAGS3) -o $@

$(ODIR32)/concept-inst.o: c++98/concept-inst.cc
	$(CXX) -shared-libgcc  -nostdinc++     -m32 $(IFLAGS1) -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(CFLAGS2) -D_GLIBCXX_CONCEPT_CHECKS -fimplicit-templates -c $<  $(CFLAGS3) -o $@

$(ODIR32)/parallel_settings.o: c++98/parallel_settings.cc
	$(CXX) -shared-libgcc  -nostdinc++     -m32 $(IFLAGS1) -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(CFLAGS2) -D_GLIBCXX_PARALLEL -c $<  $(CFLAGS3) -o $@

$(ODIR32)/libc++98convenience.a: $(O32x98)
	ar rc $@ $^
	ranlib $@

#
# c++11 folder, 32-bit
#
$(O32x11lite): $(ODIR32)/%.o: c++11/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     -m32 $(IFLAGS1) -std=gnu++11 -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(notdir $@)).lo $(CFLAGS2) -c $< $(CFLAGS3) -o $@

$(ODIR32)/hashtable_c++0x.o: c++11/hashtable_c++0x.cc
	$(CXX) -shared-libgcc  -nostdinc++     -m32 $(IFLAGS1) -std=gnu++11 -D_GLIBCXX_SHARED $(CFLAGS1) -frandom-seed=$(basename $(nodir $@)).lo $(CFLAGS2) -fimplicit-templates -c $<  $(CFLAGS3) -o $@

$(ODIR32)/libc++11convenience.a: $(O32x11)
	ar rc $@ $^
	ranlib $@

#
# ./ folder, 32-bit
#
$(O32src98p): $(ODIR32)/pic_%.o: c++98/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     -m32 $(IFLAGS1) -fPIC -DPIC $(CFLAGS1) -frandom-seed=$(basename $(nodir $<)).lo $(CFLAGS2) -c $<  $(CFLAGS3) -o $@

$(O32src98n): $(ODIR32)/nopic_%.o: c++98/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     -m32 $(IFLAGS1) -fPIC -DPIC $(CFLAGS1) -frandom-seed=$(basename $(nodir $<)).lo $(CFLAGS2) -c $< -o $@

$(O32src11p): $(ODIR32)/pic_%.o: c++11/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     -m32 $(IFLAGS1) -fPIC -DPIC $(CFLAGS1) -frandom-seed=$(basename $(nodir $<)).lo $(CFLAGS2) -std=gnu++11 -c $<  $(CFLAGS3) -o $@

$(O32src11n): $(ODIR32)/nopic_%.o: c++11/%.cc
	$(CXX) -shared-libgcc  -nostdinc++     -m32 $(IFLAGS1) -fPIC -DPIC $(CFLAGS1) -frandom-seed=$(basename $(noidr $<)).lo $(CFLAGS2) -std=gnu++11 -c $< -o $@

#
# 32-bit link rules
#
$(O32SO): $(O32A) $(O32src98p) $(O32src11p)
	cp gnu.ver libstdc++-symbols.ver32.tmp
	chmod +w libstdc++-symbols.ver32.tmp
	if test "x" != x; then \
		if grep '^# Appended to version file.' \
		/dev/null > /dev/null 2>&1; then \
			cat  >> libstdc++-symbols.ver32.tmp; \
		else \
			sed -n '1,/DO NOT DELETE/p' libstdc++-symbols.ver32.tmp > tmp.top; \
			sed -n '/DO NOT DELETE/,$$p' libstdc++-symbols.ver32.tmp > tmp.bottom; \
			cat tmp.top  tmp.bottom > libstdc++-symbols.ver32.tmp; \
			rm tmp.top tmp.bottom; \
		fi; \
	fi
	/bin/grep -E -v '^[ 	]*#(#| |$$)' libstdc++-symbols.ver32.tmp | gcc     -m32   -E -P -include ../config.h - > libstdc++-symbols.ver32 || (rm -f libstdc++-symbols.ver32 ; exit 1)
	rm -f libstdc++-symbols.ver32.tmp
	$(CXX) -shared-libgcc  -nostdinc++   -m32   -shared -nostdlib /usr/lib32/crti.o $(CRTPATH)/32/crtbeginS.o  $(O32src98p) $(O32src11p) -Wl,--whole-archive ../libsupc++/$(ODIR32)/libsupc++convenience.a $(O32A) -Wl,--no-whole-archive   -lm $(LFLAGS32) -lc -lgcc_s $(CRTPATH)/32/crtendS.o /usr/lib32/crtn.o  -m32 -Wl,-O1 -Wl,-z -Wl,relro -Wl,--gc-sections -m32 -Wl,--version-script=libstdc++-symbols.ver32   -Wl,-soname -Wl,$(nodir $@) -o $@
	rm libstdc++-symbols.ver32

$(O32AA): $(O32A) $(O32src98n) $(O32src11n)
	mkdir -p $(ODIR32)/extra
	cd $(ODIR32)/extra && ar x ../../../libsupc++/$(ODIR32)/libsupc++convenience.a
	ar rc $@  $(O32src98n) $(O32src11n) $(ODIR32)/extra/*.o $(O32x98) $(O32x11)
	ranlib $@
	rm -rf $(ODIR32)/extra
