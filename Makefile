ALL : MAIN.exe


MAIN.exe : src/reverse.c src/parse_to_table.c src/hash_passwords.c src/linked_list.c
	gcc src/reverse.c src/parse_to_table.c src/linked_list.c src/hash_passwords.c -lssl -lcrypto -Wall -g -o MAIN.exe


clean : 
	rm MAIN.exe
