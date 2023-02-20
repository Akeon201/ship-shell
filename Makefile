
#Author: Kenyon Leblanc
#Date Modified: 02/11/23

ship-shell:
	gcc ./*.c -o ship-shell
	$(clean)

wish: bash.o interactive.o shell.o shellerror.o path.o
	gcc -o wish bash.o interactive.o shell.o shellerror.o path.o
    $(clean)

clean:
	rm -f *.o

bash.o: bash.c
	gcc -c bash.c

interactive.o: interactive.c
	gcc -c interactive.c

shell.o: shell.c
	gcc -c shell.c

path.o: path.c
	gcc -c path.c

shellerror.o: shellerror.c
	gcc -c shellerror.c

