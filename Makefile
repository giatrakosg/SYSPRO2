# In order to execute this "Makefile" just type "make"
#

OBJS 	= Client.o mirror_client.o Reader.o reader_client.o
SOURCE	= Client.cpp mirror_client.cpp Reader.cpp reader_client.cpp
HEADER  = Client.hpp Reader.hpp
OUT  	= mirror_client reader_client
CC	= g++
FLAGS   = -g -c -pedantic -ansi  -Wall -Wextra -Wno-missing-field-initializers
# -g option enables debugging mode
# -c flag generates object code for separate files


# create/compile the individual files >>separately<<
mirror_client : mirror_client.o Client.o
	$(CC) mirror_client.o Client.o -o mirror_client
reader_client : reader_client.o Reader.o
	$(CC) reader_client.o Reader.o -o reader_client
mirror_client.o : mirror_client.cpp
	$(CC) $(FLAGS) mirror_client.cpp
Client.o : Client.cpp
	$(CC) $(FLAGS) Client.cpp
reader_client.o : reader_client.cpp
	$(CC) $(FLAGS) reader_client.cpp
Reader.o : Reader.cpp
	$(CC) $(FLAGS) Reader.cpp
# clean house
all : mirror_client reader_client
clean:
	rm -f $(OBJS) $(OUT) err.log common_dir/*

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
