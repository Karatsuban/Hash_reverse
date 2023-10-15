ALL : TREE.exe HASH.exe TABLE.exe

TREE.exe : parse_to_tree.c
	gcc parse_to_tree.c -o TREE.exe -Wall -g

HASH.exe : hash_passwords.c
	gcc hash_passwords.c -o HASH.exe -lssl -lcrypto -Wall

TABLE.exe : parse_to_table.c
	gcc parse_to_table.c linked_list.c -o TABLE.exe -Wall -g

clean : 
	rm *.exe
