CC=gcc
CFLAGS=-Wall -Wextra -pedantic
RAYFLAGS=`pkg-config --cflags --libs raylib`
EXE=./run
TRASH=$(EXE)

main: main.c
	$(CC) $(CFLAGS) -o $(EXE) main.c $(RAYFLAGS)

mem_test: main.c
	$(CC) $(CFLAGS) -o $(EXE) main.c $(RAYFLAGS) -DMEM_TEST__

clean:
	rm -rf $(TRASH)
