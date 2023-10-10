ALL : GEN.exe PARSE.exe HASH.exe

GEN.exe : projet.c
	gcc projet.c -o GEN -lssl -lcrypto -Wall

PARSE.exe : parse_hashes.c
	gcc parse_hashes.c -o PARSE -Wall

HASH.exe : hash_passwords.c
	gcc hash_passwords.c -o HASH -lssl -lcrypto -Wall

clean : 
	rm hashes.txt *.exe
