ALL : MAIN.exe


MAIN.exe : reverse.c parse_to_table.c hash_passwords.c
	gcc reverse.c parse_to_table.c linked_list.c hash_passwords.c -lssl -lcrypto -Wall -g -o MAIN.exe


clean : 
	rm *.exe
