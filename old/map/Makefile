#
# Since each of the stl containers is transactionalized using the same
# methodology, we only need to specify the CXX files in the current folder,
# and the dependent CXX files in the lib tree, and then we can let a common
# Makefile handle all rules and other global declarations
#

CXXFILES       = bench member iter cap element modifier observer operations allocator

LIBFILES       = functexcept snprintf_lite

include ../common/common.mk
