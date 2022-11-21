CC = gcc
CFLAGS = -Wall -O2 -march=native -flto -fPIC
# For debugging
#CFLAGS = -fPIC -Wall -ggdb
OBJ = parser.o scan.o scan_utils.o ast.o pp_lua.o pp_sexp.o pp.o driver.o

run: $(OBJ) main.o
	$(CC) $(CFLAGS) $(OPT) -o $@ $+ -lm

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $+
# Replace the line above to test the scanner
#$(CC) $(CFLAGS) -DYYLEX -c -o $@ $+

shared: $(OBJ)
	$(CC) $(CFLAGS) -shared -o liblparser.so $+ -lm

debug: run
	gdb --args ./run -lua ./tests/fib2.lua

leak: run
	valgrind --leak-check=full ./run -lua ./tests/fib2.lua

test: run
	cp ./run ./tests/l2l
	$(MAKE) -C tests

clean:
	$(RM) run *.o *.so parser.[ch] scan.[ch]
	$(MAKE) -C tests clean

parser.c: parser.y scan.c
	bison -o parser.c --defines=parser.h --debug parser.y

scan.c: scan.l
	flex --header-file=scan.h --outfile=scan.c scan.l
