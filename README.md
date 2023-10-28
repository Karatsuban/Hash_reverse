# Hash_reverse
Get plain text password from stored hashes

## 0 - Recommendation and dependencies

To execute this program, it is recommended to use at least 2Gb RAM.  
All the tests have been made on a Debian system, as so, it is <b><em>higly</em></b> recommended to use a Debian system or a Linux system at the very least to compile and execute this program. There are NO garanties that this will work on any other platform.  
For the compilation, you will need `make`, `gcc` and `openssl=3.0`.

## 1 - Usage

There are two ways of using this program.  
Local use: compile and execute the code in the current folder.  
Docker use: launch a docker and compile/execute the code in it (jump to the 'Docker use' step).   
> If you're not sure use which one to use, the docker way is recommended.  

### 1.1 - Local use

Make sure the dependencies listed in the "Dependencies" are installed.  
If so, then you can go straight to the Compilation step.  

### 1.2 - Docker use

Build the docker with the command `$ sudo docker build . -t reverse`  
Run the docker with the command `$ sudo docker run --rm -ti -v .:/home reverse`  
You'll be met with an interactive command line inside the docker.


### 2 - Compilation

To compile the code, simply type `$ make`.  
During compilation, one executable will be made: MAIN.exe.  


### 3 - Hashing password (generate)

> A password list <b>must</b> contain only one password per line.  
> This operation can take several seconds to process, depending on the file's size.

To transform any password list into a correspondance table hash-list, simply type the command  
`$ cat <password-file> | ./MAIN.exe -G -o <correspondance-table-file>`  
Change <password-file> and <correspondance-table-file> to the paths of the file you want to use.  
By default, the selected hash algorithm will be SHA256 but you can change use any algorithm available in the openSSL library by adding the argument `-a <hash-algo-name>`  


### 4 - Finding a given password (lookup)

When the hashing step is completed, you can load the hash-password equivalence list in memory with the command  
`$ cat <hashes-to-match-file> | ./MAIN.exe -L -i <correspondance-table-file>`  
Change <hashes-to-match-file> and <correspondance-table-file> to the paths of the file you want to use.  
> This operation can take several seconds to process, depending on the file's size.  

For every line in the file containing hashes to match, the program will try to find if there is a corresponding clear password.  
The program will print a line with the hash and its corresponding clear password if it is found in the table.  
Else, nothing is displayed.


### 5 - How does it work ?

When loading the hash-password file, the TABLE.exe program will efectively store each line in a hash table.
The size of this table is calculated based on the number of lines of the hash-password file.
Each time a new hash is read, the hash is passed trhough a custom-made hash function that outputs an index number clamped between 0 and the size of the table. If there are not element yet in the table at this index, a new linked list is created and a node containing the hash and the clear password is added.
If the table already contains an entry, then a node is simply added at the end of the linked list containing the hash and clear password.
The goal of the custom-made function is to evenly distribute the number of elements at each index of the hash table so that the later parsing through a linked list is roughly the same for every hash table index (at worst).


### 6 - Some facts

This version of the code has been tested against the rockyou.txt password file which contains ~14M entries.  
Hashing every line in this file can take up to 30s: be patient!  
Loading the corresponding hash-password file can take up to 15s.  
Retrieving all of the hashes generated from rockyou.txt will take ~2 minutes.
Storing the correspondance table in memory takes up roughly 1.5G of RAM.  
