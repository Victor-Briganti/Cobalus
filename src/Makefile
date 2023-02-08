CC = clang++
OBJS = main.o block.o lexer.o parser.o compiler.o vcm.o exec.o error_log.o
CFLAGS = -O3 -std=c++20
CDEBUG = -g3 -Wall -std=c++20

debug: CFLAGS = -g3 -Wall -std=c++20
release: CFLAGS = -O3 -std=c++20

all: release

debug: $(OBJS)
	$(CC) $(CDEBUG) -o cobalu $(OBJS)
	make clean

release: $(OBJS)
	$(CC) $(CFLAGS) -o cobalu $(OBJS)
	make clean

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

