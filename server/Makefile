GCC=g++
APP=dada
OUTFILE=$(APP)_server
LIBFILE=lib$(APP).so
SRC = $(wildcard *.cc)
OBJS = $(SRC:%.cc=%.o)

all: lib server

lib: $(wildcard ../lib/*.cc)
	$(GCC) -o $(LIBFILE) $^ -ldl -lpthread -shared -fPIC
	rm -f *.o

server: $(OBJS)
	$(GCC) -o $(OUTFILE) $^ -Wall -L${Boost_LIBRARY_DIRS} -L. \
		-lpthread -lboost_thread -lboost_system -ldl -ldada -fPIC

%.o: %.cc
	$(GCC) -c -o $@ $< -Wall -I../lib -g

clean :
	rm -f *.o $(OUTFILE) $(LIBFILE)

install:
	sleep 1
