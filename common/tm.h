#pragma once

#include <mutex>
#include "../common/barrier.h"

extern barrier* global_barrier;

extern std::mutex global_mutex;

#ifdef NO_TM
#define BEGIN_TX {std::lock_guard<std::mutex> _g(global_mutex);
#define END_TX   }
#else
#define BEGIN_TX __transaction_atomic {
#define END_TX   }
#endif
