# In order to execute this "Makefile" just type "make"
#

OBJS 	= Client.o mirror_client.o Reader.o reader_client.o writer_client.o \
Writer.o pidlist.o
SOURCE	= Client.cpp mirror_client.cpp Reader.cpp reader_client.cpp \
writer_client.cpp Writer.cpp pidlist.cpp
HEADER  = Client.hpp Reader.hpp Writer.hpp pidlist.hpp
OUT  	= mirror_client reader_client writer_client
CC	= g++
FLAGS   = -g -c -pedantic -ansi  -Wall -Wextra -Wno-missing-field-initializers -lcrypto \
-Wno-unused-parameter
# -g option enables debugging mode
# -c flag generates object code for separate files


# create/compile the individual files >>separately<<
mirror_client : mirror_client.o Client.o pidlist.o
	$(CC) mirror_client.o Client.o pidlist.o -o mirror_client -lcrypto
reader_client : reader_client.o Reader.o
	$(CC) reader_client.o Reader.o -o reader_client -lcrypto
mirror_client.o : mirror_client.cpp
	$(CC) $(FLAGS) mirror_client.cpp
Client.o : Client.cpp
	$(CC) $(FLAGS) Client.cpp
reader_client.o : reader_client.cpp
	$(CC) $(FLAGS) reader_client.cpp
Reader.o : Reader.cpp
	$(CC) $(FLAGS) Reader.cpp
writer_client: writer_client.o Writer.o
	$(CC) writer_client.o Writer.o -o writer_client -lcrypto
Writer.o : Writer.cpp
	$(CC) $(FLAGS) Writer.cpp
writer_client.o : writer_client.cpp
	$(CC) $(FLAGS) writer_client.cpp
pidlist.o : pidlist.cpp
	$(CC) $(FLAGS) pidlist.cpp
# clean house
all : mirror_client reader_client writer_client
clean:
	rm -f $(OBJS) $(OUT) common_dir mirror_dir  \
	reader.log writer.log *.log -r

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
