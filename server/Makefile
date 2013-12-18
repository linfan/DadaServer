GCC=g++
APP=dada
OUTFILE=$(APP)_server
LIBFILE=lib$(APP).so
SRC = $(wildcard *.cc)
OBJS = $(SRC:%.cc=%.o)

BOOST_LIB_DIR = $(HOME)/app/boost_1_55_0/lib
MYSQL_CPP_LIB_DIR = $(HOME)/app/mysql-connector-c++-1.1.3/lib
MYSQL_BIN_DIR = $(HOME)/app/mysql-5.6.14/bin

all: lib server

lib: $(wildcard ../lib/*.cc)
	$(GCC) -o $(LIBFILE) $^ -ldl -lpthread -shared -fPIC
	rm -f *.o

server: $(OBJS)
	$(GCC) -o $(OUTFILE) $^ -Wall -L. -L$(BOOST_LIB_DIR) -L$(MYSQL_CPP_LIB_DIR) \
		-L$(MYSQL_LIB_DIR) -lpthread -lboost_thread -lboost_system -lmysqlcppconn \
		-ldl -ldada -fPIC `$(MYSQL_BIN_DIR)/mysql_config --libs`

%.o: %.cc
	$(GCC) -c -o $@ $< -Wall -I../lib -g `$(MYSQL_BIN_DIR)/mysql_config --cflags`

clean :
	rm -f *.o $(OUTFILE) $(LIBFILE)

