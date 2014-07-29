#ifdef NO_TM
#  include <cstdio>
#  define TRACE(x) printf("  [TRACE: %s] %s\n", x, __PRETTY_FUNCTION__)
#else
#  define TRACE(x)
#endif
