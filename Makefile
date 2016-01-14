make:
	gcc -Wall process.c `pkg-config fuse --cflags --libs` -o process
	./process myproc/

clean:
	fusermount -u myproc/
