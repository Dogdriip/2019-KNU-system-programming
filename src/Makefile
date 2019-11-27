battle_typing: main_menu.o single_menu.o multi_menu.o connection.o string.o
	gcc -o battle_typing *.o -lcurses -pthread

main_menu.o: main_menu.h main_menu.c
	gcc -c -o main_menu.o main_menu.c

single_menu.o: single/single_menu.h single/single_menu.c
	gcc -c -o single_menu.o single/single_menu.c

multi_menu.o: multi/multi_menu.h multi/multi_menu.c
	gcc -c -o multi_menu.o multi/multi_menu.c

connection.o: util/connection.h util/connection.c
	gcc -c -o connection.o util/connection.c

string.o: util/string.c util/string.h
	gcc -c -o string.o util/string.c

clean:
	rm *.o battle_typing
