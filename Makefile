# In order to execute this "Makefile" just type "make"
#

OBJS 	= Client.o mirror_client.o Reader.o reader_client.o writer_client.o \
Writer.o
SOURCE	= Client.cpp mirror_client.cpp Reader.cpp reader_client.cpp \
writer_client.cpp Writer.cpp
HEADER  = Client.hpp Reader.hpp Writer.hpp
OUT  	= mirror_client reader_client writer_client
CC	= g++
FLAGS   = -g -c -pedantic -ansi  -Wall -Wextra -Wno-missing-field-initializers \
`gpgme-config --cflags`
# -g option enables debugging mode
# -c flag generates object code for separate files


# create/compile the individual files >>separately<<
mirror_client : mirror_client.o Client.o
	$(CC) mirror_client.o Client.o -o mirror_client `gpgme-config --libs`
reader_client : reader_client.o Reader.o
	$(CC) reader_client.o Reader.o -o reader_client `gpgme-config --libs`
mirror_client.o : mirror_client.cpp
	$(CC) $(FLAGS) mirror_client.cpp
Client.o : Client.cpp
	$(CC) $(FLAGS) Client.cpp
reader_client.o : reader_client.cpp
	$(CC) $(FLAGS) reader_client.cpp
Reader.o : Reader.cpp
	$(CC) $(FLAGS) Reader.cpp
writer_client: writer_client.o Writer.o
	$(CC) writer_client.o Writer.o -o writer_client `gpgme-config --libs`
Writer.o : Writer.cpp
	$(CC) $(FLAGS) Writer.cpp
writer_client.o : writer_client.cpp
	$(CC) $(FLAGS) writer_client.cpp

# clean house
all : mirror_client reader_client writer_client
clean:
	rm -f $(OBJS) $(OUT) common_dir mirror_dir  \
	reader.log writer.log *.log -r

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
