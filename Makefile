OBJS = arduino/DrogonPosition.o arduino/DrogonController.o DrogonTest.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -fPIC
LFLAGS = -Wall $(DEBUG)

all: lib 

lib: $(OBJS)
	$(CC) -shared -Wl,-soname,libdrogonflight.so -o libdrogonflight.so $(OBJS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f arduino/*.o *.o libdrogonflight.so 
