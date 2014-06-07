gcc -c -fpic *
gcc -fpic -shared -o libmym.so -lpthread *.o
mv libmym.so /usr/lib
rm *.o
rm *.gch

