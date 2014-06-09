cp psmalloc.h /usr/include
gcc -c -fpic *
gcc -fpic -shared -o libpsmalloc.so -lpthread *.o
mv libpsmalloc.so /usr/lib
rm *.o
rm *.gch

