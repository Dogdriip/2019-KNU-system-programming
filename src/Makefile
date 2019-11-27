battle_typing: main_menu.o single_menu.o string.o
	gcc -o battle_typing *.o -lcurses

main_menu.o: main_menu.h main_menu.c
	gcc -c -o main_menu.o main_menu.c

single_menu.o: single/single_menu.h single/single_menu.c
	gcc -c -o single_menu.o single/single_menu.c

#multi_menu.o: multi/multi_menu.h multi/multi_menu.c
#	gcc -c -o multi_menu.o multi/multi_menu.c

string.o: util/string.c util/string.h
	gcc -c -o string.o util/string.c

clean:
	rm *.o battle_typing
