SRC = ./src
INCLUDE = $(SRC)/include
OBJS = $(SRC)/DrogonPid.o $(SRC)/DrogonPidTuner.o $(SRC)/DrogonPosition.o \
	  $(SRC)/DrogonController.o $(SRC)/I2C.o $(SRC)/I2CIMU.o $(SRC)/I2CServo.o \
	  $(SRC)/DrogonFlight.o $(SRC)/drogon-flight.o
CC = g++
DEBUG = -g
CFLAGS = -std=c++11 -I $(INCLUDE) -I ../drogon-flight/src/include $(DEBUG) -Wall

all: drogon-flight

libdrogonflight.so: $(OBJS)
	$(CC) -shared -Wl,-soname,libdrogonflight.so -o $@ $(OBJS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

drogon-flight: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f src/*.o drogon-flight
