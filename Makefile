CC=gcc
CFLAGS=-Wall -Wextra -pedantic -fPIC
RAYFLAGS=`pkg-config --cflags --libs raylib`
EXE=./run
TRASH=$(EXE) libvoxel.so voxel.o

ifdef MEM_TEST
CFLAGS += -DMEM_TEST__
endif

main: main.c libvoxel.so
	$(CC) $(CFLAGS) -o $(EXE) main.c -L. -lvoxel $(RAYFLAGS) -Wl,-rpath,.

libvoxel.so: voxel.o
	$(CC) -shared -o libvoxel.so voxel.o $(RAYFLAGS)

voxel.o: voxel.c
	$(CC) $(CFLAGS) -c voxel.c

clean:
	rm -rf $(TRASH)
