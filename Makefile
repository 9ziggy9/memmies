CC=gcc
CFLAGS=-Wall -Wextra -pedantic
RAYFLAGS=`pkg-config --cflags --libs raylib`
EXE=./run
TRASH=$(EXE)

ifdef MEM_TEST
CFLAGS += -DMEM_TEST__
endif

main: main.c
	$(CC) $(CFLAGS) -o $(EXE) main.c $(RAYFLAGS)

clean:
	rm -rf $(TRASH)
