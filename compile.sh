gcc -c hashmap.c -o hashmap.o
ar r lib_mylib.a hashmap.o
gcc -pthread -c main.c -o main.o
gcc main.o lib_mylib.a -o wynik.out
./wynik.out