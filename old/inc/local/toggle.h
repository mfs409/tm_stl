#ifdef NO_TM
#define TM_SAFE_ATT
#else
#define TM_SAFE_ATT __attribute__((transaction_safe))
#endif
