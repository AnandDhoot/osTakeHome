Apart from the code for 'bbfs.c', the code for encryption, 'enc.c' must be added to the source directory. Additionally, the the 'Makefile' in src/ must be modified as follows - 

"LDADD = -lfuse -pthread" should be changed to 
"LDADD = -lfuse -pthread  -lssl -lcrypto -lm"

This implements character-cipher encryption using OpenSSL library we use a stream cipher namely AES CTR 128 bit encryption.