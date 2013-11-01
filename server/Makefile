GCC=g++
APP=dada
OUTFILE=$(APP)_server
SRC = $(wildcard *.cc)
OBJS = $(SRC:%.cc=%.o)

all: lib server

lib: $(wildcard ../lib/*.cc)
	$(GCC) -o lib$(APP).so $^ -ldl -shared -fPIC
	rm -f *.o

server: $(OBJS)
	$(GCC) -o $(OUTFILE) $^ -Wall -L${Boost_LIBRARY_DIRS} -L. \
		-lpthread -lboost_thread -lboost_system -ldl -ldada -fPIC

%.o: %.cc
	$(GCC) -c -o $@ $< -Wall -I../lib -g

clean :
	rm -f *.o $(OUTFILE)

install:
	sleep 1
