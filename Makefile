OBJS = arduino/DrogonPid.o arduino/DrogonPidTuner.o arduino/DrogonPosition.o arduino/DrogonController.o DrogonTest.o
JNI_OBJS = arduino/DrogonPid.o arduino/DrogonPidTuner.o arduino/DrogonPosition.o arduino/DrogonController.o org_joemonti_drogon_flight_DrogonFlight.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -fPIC
LFLAGS = -Wall $(DEBUG)

all: libdrogonflight.so # libjdrogonflight.so

#libjdrogonflight.so : $(JNI_OBJS)
#	$(CC) -I ${JAVA_HOME}/include/ -I ${JAVA_HOME}/include/linux/ -shared -Wl,-soname,$@ -o $@ $(JNI_OBJS)

#org_joemonti_drogon_flight_DrogonFlight.o: org_joemonti_drogon_flight_DrogonFlight.cpp org_joemonti_drogon_flight_DrogonFlight.h
#	$(CC) -I ${JAVA_HOME}/include/ -I ${JAVA_HOME}/include/linux/ -c $< -o $@ $(CFLAGS)

libdrogonflight.so: $(OBJS)
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f arduino/*.o *.o libdrogonflight.so #libjdrogonflight.so
