all:
	cd deque && BITS=64 $(MAKE)
	cd deque && BITS=32 $(MAKE)
	cd list && BITS=64 $(MAKE)
	cd list && BITS=32 $(MAKE)
	cd map && BITS=64 $(MAKE)
	cd map && BITS=32 $(MAKE)
	cd pair && BITS=64 $(MAKE)
	cd pair && BITS=32 $(MAKE)
	cd string && BITS=64 $(MAKE)
	cd string && BITS=32 $(MAKE)
	cd tuple && BITS=64 $(MAKE)
	cd tuple && BITS=32 $(MAKE)
	cd unordered_map && BITS=64 $(MAKE)
	cd unordered_map && BITS=32 $(MAKE)
	cd unordered_multiset && BITS=64 $(MAKE)
	cd unordered_multiset && BITS=32 $(MAKE)
	cd unordered_set && BITS=64 $(MAKE)
	cd unordered_set && BITS=32 $(MAKE)
	cd vector && BITS=64 $(MAKE)
	cd vector && BITS=32 $(MAKE)

clean:
	cd deque && $(MAKE) clean
	cd list && $(MAKE) clean
	cd map && $(MAKE) clean
	cd pair && $(MAKE) clean
	cd string && $(MAKE) clean
	cd tuple && $(MAKE) clean
	cd unordered_map && $(MAKE) clean
	cd unordered_multiset && $(MAKE) clean
	cd unordered_set && $(MAKE) clean
	cd vector && $(MAKE) clean
