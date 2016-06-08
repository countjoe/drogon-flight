OBJS = src/DrogonPid.o src/DrogonPidTuner.o src/DrogonPosition.o src/DrogonController.o DrogonTest.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -fPIC -I ./src/include/
LFLAGS = -Wall $(DEBUG)

all: libdrogonflight.so

libdrogonflight.so: $(OBJS)
	$(CC) -shared -Wl,-soname,libdrogonflight.so -o $@ $(OBJS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f src/*.o *.o libdrogonflight.so
