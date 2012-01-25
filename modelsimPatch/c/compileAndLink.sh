gcc -c -I/opt/modelsim/modeltech/include fileiopatch.c
ld -shared -E -o fileiopatch.so fileiopatch.o
