# Hash_reverse
Get plain text password from stored hashes


## Usage

### Compilation

To compile the code, simply type `$ make`.  
During compilation, two executables will me made : TABLE.exe and HASH.exe.  

### Hashing password

> A password list <b>must</b> contain only one password per line.  
> This operation can take several seconds to process, depending on the file's size.

To transform any password list into a correspondance table hash-list, simply type the command `$ ./HASH.exe <password_file>`  
For the time being, only the SHA-256 hash algorithm can be applied to passwords.  
The result of the operation will be a file containing a hash, a space and a clear password on each line.  
By default, this file will be called 'hashes.txt'.  


### Finding a given password

When the hashing step is completed, you can load the hash-password equivalence list in memory with the command `$ ./TABLE.exe hashes.txt`  

> This process can take several seconds to process, depending on the file's size.  

Once all the lines have been loaded, the program will ask you for a hash. Simply type or paste the hash and press enter.  
If the hash is found, the program will display the clear password.  
If not, the program will inform you that no password was found.  
To exit the program, simply type 'exit'.  

## How does it work ?

When loading the hash-password file, the TABLE.exe program will efectively store each line in a hash table.
The size of this table is calculated based on the number of lines of the hash-password file.
Each time a new hash is read, the hash is passed trhough a custom-made hash function that outputs an index number clamped between 0 and the size of the table. If there are not element yet in the table at this index, a new linked list is created and a node containing the hash and the clear password is added.
If the table already contains an entry, then a node is simply added at the end of the linked list containing the hash and clear password.
The goal of the custom-made function is to evenly distribute the number of elements at each index of the hash table so that the later parsing through a linked list is roughly the same for every hash table index (at worst).


## Some facts

This version of the code has been tested against the rockyou.txt password file which contains ~14M entries.  
Some passwords cannot be stored du to their encodings and some are voluntarily dropped if they are too long (average password length is 10 characters).  
Hashing every word in this file can take up to 30s: be patient!  
Loading the corresponding hash-password file can take up to 15s.  
Retrieving a stored password is immediate!  
Storing the file takes up roughly 1.5G of RAM.  
