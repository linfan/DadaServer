GCC=g++
APP=dada

all: lib simulator

lib: $(wildcard ../lib/*.cc)
	$(GCC) -o lib$(APP).so $^ -ldl -shared -fPIC

simulator: $(wildcard *.cc)
	$(GCC) -o $(APP)_simulator $^ -lpthread -ldl -L. -ldada -fPIC -I../lib

clean :
	rm -f *.o $(APP)_simulator

install:
	cp $(APP)_simulator ~/.bin/
	cp lib$(APP).so ~/.lib/

uninstall:
	rm -f ~/.bin/$(APP)_simulator
	rm -f ~/.lib/lib$(APP).so
