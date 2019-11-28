battle_typing: main_menu.o single_menu.o string.o endless.o connection.o multi_menu.o multi_game.o
	gcc -o battle_typing *.o -lcurses -pthread

main_menu.o: main_menu.h main_menu.c
	gcc -c -o main_menu.o main_menu.c

single_menu.o: single/single_menu.h single/single_menu.c
	gcc -c -o single_menu.o single/single_menu.c

connection.o: util/connection.h util/connection.c
	gcc -c -o connection.o util/connection.c

multi_menu.o: multi/multi_menu.h multi/multi_menu.c
	gcc -c -o multi_menu.o multi/multi_menu.c

multi_game.o: multi/multi_game.h multi/multi_game.c
	gcc -c -o multi_game.o multi/multi_game.c

string.o: util/string.c util/string.h
	gcc -c -o string.o util/string.c

endless.o: single/endless.c single/endless.h
	gcc -c -o endless.o single/endless.c

#time_attack.o: single/time_attack.c single/time_attack.h
#	gcc -c -o time_attack.o single/time_attack.c

clean:
	rm *.o
