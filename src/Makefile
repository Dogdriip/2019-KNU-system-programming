battle_typing: menu.o single_menu.o
	gcc -o battle_typing menu.o single_menu.o -lcurses

menu.o: menu.h menu.c
	gcc -c -o menu.o menu.c

single_menu.o: single/single_menu.h single/single_menu.c
	gcc -c -o single_menu.o single/single_menu.c

clean:
	rm -f *.o
