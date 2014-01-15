#ifdef NO_TM
#define TRACE(x) printf("  [TRACE: %s] %s\n", x, __PRETTY_FUNCTION__)
#else
#define TRACE(x)
#endif
