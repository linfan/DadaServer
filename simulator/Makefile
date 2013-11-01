GCC=g++
APP=dada

all: lib tcp_simulator udp_simulator

lib: $(wildcard ../lib/*.cc)
	$(GCC) -o lib$(APP).so $^ -ldl -shared -fPIC

tcp_simulator: tcp_simulator.cc
	$(GCC) -o $(APP)_tcp_simulator $^ -lpthread -ldl -L. -ldada -fPIC -I../lib

udp_simulator: udp_simulator.cc
	$(GCC) -o $(APP)_udp_simulator $^ -lpthread -ldl -L. -ldada -fPIC -I../lib

clean :
	rm -f *.o $(APP)_tcp_simulator $(APP)_udp_simulator

