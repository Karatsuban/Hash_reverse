ALL : HASH.exe TABLE.exe

HASH.exe : hash_passwords.c
	gcc hash_passwords.c -o HASH.exe -lssl -lcrypto -Wall

TABLE.exe : parse_to_table.c
	gcc parse_to_table.c linked_list.c -o TABLE.exe -Wall -g

clean : 
	rm *.exe
