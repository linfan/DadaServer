GCC=g++
APP=dada

all: lib server

lib: $(wildcard ../lib/*.cc)
	$(GCC) -o lib$(APP).so $^ -ldl -shared -fPIC

server: $(wildcard *.cc)
	$(GCC) -o $(APP)_server $^ -lpthread -ldl -L. -ldada -fPIC -I../lib

clean :
	rm -f *.o $(APP)_server

install:
	scp $(APP)_server root@router.com:/bin/
	scp lib$(APP).so root@router.com:/lib/
