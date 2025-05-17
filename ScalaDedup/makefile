CC = g++
CFLAGS = -g -w -O3 -Wall
LIBS = -lpthread -lpbc -lPBC -lgmp -lcrypto -lpqxx
INCLUDES = -I ./pbcwrapper -L ./pbcwrapper 
OBJS = import.h io.o struct.o crypto.o

run: ./main.cpp $(OBJS) 
	$(CC) $(OBJS) -o main ./main.cpp $(CFLAGS) $(LIBS) $(INCLUDES) 

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o
	rm -rf main
	rm -rf log.txt
