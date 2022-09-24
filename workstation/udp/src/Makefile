all : recv send

recv : recv.o
	gcc -o $@ $^

send : send.o
	gcc -o $@ $^

recv.o : recv.c
	gcc -c $<

send.o : send.c
	gcc -c $<