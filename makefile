

run: tst.o test.o thread_pool.o 
	gcc tst.o test.o thread_pool.o -o run -lpthread

tst.o: tst.c
	gcc -c tst.c -o tst.o -lpthread

test.o: test.c 
	gcc -c test.c -o test.o -lpthread

thread_pool.o: thread_pool.c
	gcc -c thread_pool.c -o thread_pool.o -lpthread

clean:	
	rm -rf *.o


	


