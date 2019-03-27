# In order to execute this "Makefile" just type "make"
#

OBJS 	= Client.o mirror_client.o
SOURCE	= Client.cpp mirror_client.cpp
HEADER  = Client.hpp
OUT  	= mirror_client
CC	= g++
FLAGS   = -g -c -pedantic -ansi  -Wall -Wextra
# -g option enables debugging mode
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

# create/compile the individual files >>separately<<
mirror_client.o : mirror_client.cpp
	$(CC) $(FLAGS) mirror_client.cpp
Client.o : Client.cpp
	$(CC) $(FLAGS) Client.cpp
# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
