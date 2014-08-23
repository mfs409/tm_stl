all:
	@cd libstdc++ && $(MAKE)
	@cd libstdc++_tm && $(MAKE)
	@cd libstdc++_trace && $(MAKE)
	@cd validation && $(MAKE)

clean:
	@cd libstdc++ && $(MAKE) clean
	@cd libstdc++_tm && $(MAKE) clean
	@cd libstdc++_trace && $(MAKE) clean
	@cd validation && $(MAKE) clean
