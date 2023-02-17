
#Author: Kenyon Leblanc
#Date Modified: 02/11/23

ship-shell:
	gcc ./*.c -o ship-shell
	$(clean)

clean:
	rm -f *.o

bash.o: bash.c
	gcc -c bash.c

interactive.o: interactive.c
	gcc -c interactive.c

shell.o: shell.c
	gcc -c shell.c

wish: bash.o interactive.o shell.o
	gcc -o wish bash.o interactive.o shell.o
    $(clean)
