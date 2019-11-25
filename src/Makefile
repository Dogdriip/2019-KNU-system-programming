battle_typing: menu.o
	gcc -o battle_typing menu.o -lcurses

menu.o: menu.h menu.c
	gcc -c -o menu.o menu.c

clean:
	rm -f *.o
