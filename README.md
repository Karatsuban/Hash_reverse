# Hash_reverse
Get plain text password from stored hashes




# openssl

getting and installing the last version of openssl  

sudo apt install build-essential checkinstall zliblg-dev
cd /usr/local/src
https://www.openssl.org/source/openssl-3.1.3.tar.gz
tar -xf openssl-3.0.7.tar.gz
cd openssl-3.0.7
sudo ./config --prefix=/usr/local/ssl --openssldir=/usr/local/ssl shared zlib
sudo make
sudo install

cd /etc/ls.so/conf.d/
sudo echo /usr/local/ssl.lib64 > openssl-3-1-3.conf
sudo ldconfig -v

sudo nano /etc/environnement


~                                                                                          
~                                                                                          
~                                                                                          
~                                                                                          
~                                                                                          
~                                                                                          
~                                                                                          
~                                                                                          
~                                           
