
#Author: Kenyon Leblanc
#Date Modified: 02/11/23

ship-shell:
	gcc ./*.c -o ship-shell
	$(clean)

clean:
	rm -f *.o

wish:
	gcc ./*.c -o wish
