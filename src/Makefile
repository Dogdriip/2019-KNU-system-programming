battle_typing: menu.o single_menu.o multi_menu.o
	gcc -o battle_typing menu.o single_menu.o multi_menu.o -lcurses

menu.o: menu.h menu.c
	gcc -c -o menu.o menu.c

single_menu.o: single/single_menu.h single/single_menu.c
	gcc -c -o single_menu.o single/single_menu.c

multi_menu.o: multi/multi_menu.h multi/multi_menu.c
	gcc -c -o multi_menu.o multi/multi_menu.c

clean:
	rm -f *.o
