CC = clang++
OBJS = main.o parser.o
CFLAGS = -O3 -std=c++20
CDEBUG = -g3 -Wall -std=c++20

debug: CFLAGS = -g3 -Wall -std=c++20
release: CFLAGS = -O3 -std=c++20

all: release

debug: $(OBJS)
	$(CC) $(CDEBUG) -o cobalus $(OBJS)

release: $(OBJS)
	$(CC) $(CFLAGS) -o cobalus $(OBJS)
	make clean

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS)

