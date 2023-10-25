# Hash_reverse
Get plain text password from stored hashes


## Usage

There are two ways of using this program.  
One way (local use) is compile and execute the code in the current folder (jump to Compilation).  
The other way is to launch a docker and compile/execute the code in it.  
If you're not use which one to use, the docker way is recommended.  


### Docker use

Build the docker with the command `$ sudo docker build . -t reverse`  
Run the docker with the command `$ sudo docker run --rm -ti -v .:/home reverse`  
You'll be met with an interactive command line inside the docker.


### Compilation

To compile the code, simply type `$ make`.  
During compilation, one executable will be made: MAIN.exe.  


### Hashing password

> A password list <b>must</b> contain only one password per line.  
> This operation can take several seconds to process, depending on the file's size.

To transform any password list into a correspondance table hash-list, simply type the command  
`$ cat <password-file> | ./MAIN.exe -G -o <correspondance-table-file>`  
By default, the selected hash algorithm will be SHA256 but you can change use any algorithm available in the openSSL library by adding the argument `-a <hash-algo-name>`  


### Finding a given password

When the hashing step is completed, you can load the hash-password equivalence list in memory with the command  
`$ cat <hashes-to-match-file> | ./MAIN.exe -L -i <correspondance-table-file>`  

> This operation can take several seconds to process, depending on the file's size.  

For every line in the file containing hashes to match, the program will try to find if there is a corresponding clear password.  
The program will print a line with the hash and its corresponding clear password if it is found in the table.  
Else, nothing is displayed.

## How does it work ?

When loading the hash-password file, the TABLE.exe program will efectively store each line in a hash table.
The size of this table is calculated based on the number of lines of the hash-password file.
Each time a new hash is read, the hash is passed trhough a custom-made hash function that outputs an index number clamped between 0 and the size of the table. If there are not element yet in the table at this index, a new linked list is created and a node containing the hash and the clear password is added.
If the table already contains an entry, then a node is simply added at the end of the linked list containing the hash and clear password.
The goal of the custom-made function is to evenly distribute the number of elements at each index of the hash table so that the later parsing through a linked list is roughly the same for every hash table index (at worst).


## Some facts

This version of the code has been tested against the rockyou.txt password file which contains ~14M entries.  
Hashing every word in this file can take up to 30s: be patient!  
Loading the corresponding hash-password file can take up to 15s.  
Retrieving a stored password is immediate!  
Storing the file takes up roughly 1.5G of RAM.  
