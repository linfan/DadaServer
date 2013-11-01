all:
	$(MAKE) all -C server
	$(MAKE) all -C simulator

clean:
	$(MAKE) clean -C server
	$(MAKE) clean -C simulator
