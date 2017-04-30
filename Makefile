

main : batch_obj
	gcc -o BFSproc BFSproc.c BFSmode.o

batch_obj: 
	gcc -o BFSmode.o -c BFSmode.c

clean : 
	rm mylib.o assg2.o assg2.exe
