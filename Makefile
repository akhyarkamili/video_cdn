# Makefile for Project 1 of 15-441
CC = gcc
CFLAGS = -g -Wall -Werror
DFLAGS = -g -O0

all: proxy

server.o: server.c server.h logging.h
	$(CC) $(CFLAGS) -c server.c

logging.o: logging.c logging.h
	$(CC) $(CFLAGS) -c logging.c

wrapper.o: wrapper.c wrapper.h
	$(CC) $(CFLAGS) -c wrapper.c

proxy.o: proxy.c wrapper.h server.h logging.h
	$(CC) $(CFLAGS) -c proxy.c

proxy: server.o wrapper.o proxy.o logging.o

debug: 
	$(CC) $(DFLAGS) -c server.c
	$(CC) $(DFLAGS) -c logging.c
	$(CC) $(DFLAGS) -c wrapper.c
	$(CC) $(DFLAGS) -c proxy.c
	$(CC) server.o logging.o wrapper.o proxy.o -o proxy

clean:
	rm -f *~ *.o proxy *.tar *.zip *.gzip *.bzip *.gz
	
handin:
	cd .. && find 15-441-project-3 -name "*.c" -o -name "*.h" -o -name "*.git" -o -name "*.html" -o -name "*.png" -o -name "*.css" -o -name "Makefile" -o -name ".gitgnore" | tar -cf akamili.tar  -T -