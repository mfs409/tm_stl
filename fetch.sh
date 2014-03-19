# get paths
echo -n "Enter the path to your installed include files (e.g., /usr/include/c++/4.9.0/) :> "
read -e stdinc

echo -n "Enter the path to your installed lib include files (e.g., /usr/lib/gcc/x86_64-unknown-linux-gnu/4.9.0/include) :> "
read -e libinc

echo -n "Enter the path to your installed platform include files (e.g., /usr/include/c++/4.9.0/x86_64-unknown-linux-gnu/) :> "
read -e platforminc

echo -n "Enter the path to your gcc trunk's libstdc++-v3 folder (e.g., /home/me/gcc_trunk/libstdc++-v3/src/) :> "
read -e libsrc

# confirm
echo "About to reset all header files... use ctrl-c now to abort, enter to continue ..."
read -e

# now make our directories
rm -rf inc lib
mkdir -p inc/libinc inc/platform_inc/bits inc/stdinc/backward inc/stdinc/bits inc/stdinc/debug inc/stdinc/ext inc/tm_stdinc/bits inc/tm_stdinc/ext lib/libstdc++-v3/c++11 lib/libstdc++-v3/c++98

# copy from libinc
for f in stdarg.h stdbool.h stddef.h
do
    cp $libinc/$f inc/libinc
done

# copy from platforminc/bits
for f in atomic_word.h c++allocator.h c++config.h c++locale.h cpu_defines.h ctype_base.h ctype_inline.h error_constants.h gthr-default.h gthr.h os_defines.h
do
    cp $platforminc/bits/$f inc/platform_inc/bits/
done

# copy from stdinc
for f in array atomic cassert cctype cerrno chrono clocale condition_variable cstdint cstdio cstdlib ctime cwchar cwctype exception functional future initializer_list ios iosfwd iostream istream limits list memory mutex new ostream ratio stdexcept streambuf string system_error thread tuple typeinfo type_traits utility map
do
    cp $stdinc/$f inc/stdinc/
done

# copy from stdinc/backward
for f in auto_ptr.h binders.h
do
    cp $stdinc/backward/$f inc/stdinc/backward/
done

# copy from stdinc/bits
for f in allocator.h alloc_traits.h atomic_base.h atomic_lockfree_defines.h basic_ios.h basic_ios.tcc basic_string.h basic_string.tcc char_traits.h concept_check.h cpp_type_traits.h cxxabi_forced.h exception_defines.h exception_ptr.h functexcept.h functional_hash.h hash_bytes.h ios_base.h istream.tcc list.tcc locale_classes.h locale_classes.tcc locale_facets.h locale_facets.tcc localefwd.h memoryfwd.h move.h nested_exception.h ostream_insert.h ostream.tcc parse_numbers.h postypes.h predefined_ops.h ptr_traits.h range_access.h regex_error.h shared_ptr_base.h shared_ptr.h stl_algobase.h stl_construct.h stl_function.h stl_iterator_base_funcs.h stl_iterator_base_types.h stl_iterator.h stl_pair.h stl_raw_storage_iter.h stl_relops.h stl_tempbuf.h stl_uninitialized.h streambuf_iterator.h streambuf.tcc stringfwd.h unique_ptr.h uses_allocator.h stl_map.h stl_multimap.h stl_tree.h
do
    cp $stdinc/bits/$f inc/stdinc/bits/
done

# copy from stdinc/debug
for f in debug.h
do
    cp $stdinc/debug/$f inc/stdinc/debug/
done

# copy from stdinc/ext
for f in aligned_buffer.h alloc_traits.h atomicity.h concurrence.h numeric_traits.h string_conversions.h type_traits.h
do
    cp $stdinc/ext/$f inc/stdinc/ext/
done

# transfer from stdinc/bits to tm_stdinc/bits
for f in stl_list.h
do
    cp $stdinc/bits/$f inc/tm_stdinc/bits/
done

# transfer from stdinc/ext to tm_stdinc/ext
for f in new_allocator.h
do
    cp $stdinc/ext/$f inc/tm_stdinc/ext/
done

# transfer from libsrc/c++11
for f in functexcept.cc  snprintf_lite.cc
do
    cp $libsrc/c++11/$f lib/libstdc++-v3/c++11/
done

# transfer from libsrc/c++98
for f in list.cc
do
    cp $libsrc/c++98/$f lib/libstdc++-v3/c++98/
done
