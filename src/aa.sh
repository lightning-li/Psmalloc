gcc -c -fpic *.c
gcc -fpic -shared -o libpsmalloc.so -lpthread *.o
#mv libpsmalloc.so /usr/lib
#rm *.o

