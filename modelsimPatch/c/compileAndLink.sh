gcc -c -I/opt/modelsim/modeltech/include stimuli.c
ld -shared -E -o stimuli.so stimuli.o

gcc -c -I/opt/modelsim/modeltech/include response.c
ld -shared -E -o response.so response.o
