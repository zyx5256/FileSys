CC   = gcc
OPTS =  -Wall  -g 

all: server   libmfs.so client

# this generates the target executables
server: server.o udp.o fs.o
	$(CC) -o server server.o udp.o fs.o

client: client.o  
	$(CC) -o client client.o -L. -lmfs

libmfs.so: mfs.o udp.o 
	$(CC)  -fPIC -shared  -o libmfs.so mfs.o  udp.o 

# this is a generic rule for .o files 
%.o: %.c 
	$(CC) $(OPTS) -fPIC -c $< -o $@

clean:
	rm -f server.o udp.o client.o mfs.o server client libmfs.so
