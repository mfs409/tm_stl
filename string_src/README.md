std::string
======

All work on std::string is temporarily on hold.  std::string uses a reference
counting mechanism that is not currently transaction_safe (it uses volatiles
and builtin RMW instructions).

One possibility is to create std::tm_string, which uses TM for reference
counting.  Another possibility is to just re-implement all reference counting
with transactions.  I'm hopeful that there are other options, but since
neither of these proposals is non-trivial, we're going to put std::string on
hold until we come up with a better idea.
