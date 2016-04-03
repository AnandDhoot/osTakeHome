#include <openssl/aes.h>
#include <openssl/rand.h> 
#include <openssl/hmac.h>
#include <openssl/buffer.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <math.h>
 #include <unistd.h>

// Code example uses partail code from: http://stackoverflow.com/questions/3141860/aes-ctr-256-encryption-mode-of-operation-on-openssl
// Mostly in the ctr_ state, and init_ctr functions. 
using namespace std;
struct ctr_state 
{ 
	unsigned char ivec[AES_BLOCK_SIZE];	 
	unsigned int num; 
	unsigned char ecount[AES_BLOCK_SIZE]; 
}; 

FILE *readFile;
FILE *writeFile;
AES_KEY key; 

int bytes_read, bytes_written;	 
unsigned char indata[AES_BLOCK_SIZE]; 
unsigned char outdata[AES_BLOCK_SIZE];
unsigned char iv[AES_BLOCK_SIZE];
struct ctr_state state;	 

int init_ctr(struct ctr_state *state, const unsigned char iv[16])
{		 
	/* aes_ctr128_encrypt requires 'num' and 'ecount' set to zero on the
     * first call. */
    state->num = 0;
    memset(state->ecount, 0, AES_BLOCK_SIZE);

    /* Initialise counter in 'ivec' to 0 */
    memset(state->ivec + 8, 0, 8);

    /* Copy IV into 'ivec' */
    //memcpy(state->ivec, iv, 8);
}

void fencrypt(const unsigned char* read,unsigned char* write,int size, const unsigned char* enc_key)
{ 
    if(!RAND_bytes(iv, AES_BLOCK_SIZE))
    {
        fprintf(stderr, "Could not create random bytes.");
        exit(1);    
    }
    
	//Initializing the encryption KEY
	if (AES_set_encrypt_key(enc_key, 128, &key) < 0)
    {
        fprintf(stderr, "Could not set encryption key.");
        exit(1); 
    }

	init_ctr(&state, iv); //Counter call
	//Encrypting Blocks of 16 bytes and writing the output.txt with ciphertext	

		AES_ctr128_encrypt(read, write, size, &key, state.ivec, state.ecount, &state.num);
}
	
void fdecrypt(unsigned char* read, unsigned char* write,int size, const unsigned char* enc_key)
{	
    
	// readFile=fopen(read,"rb"); // The b is required in windows.
	// writeFile=fopen(write,"wb");
	
	// if(readFile==NULL)
	// {
	// 	fprintf(stderr,"Read file is null."); 
 //        exit(1);
	// }
	
	// if(writeFile==NULL)	
	// {
	// 	fprintf(stderr, "Write file is null."); 
 //        exit(1);
	// }

	//Initializing the encryption KEY
    if (AES_set_encrypt_key(enc_key, 128, &key) < 0)
    {
        fprintf(stderr, "Could not set decryption key.");
        exit(1);
    }

	init_ctr(&state, iv);//Counter call
	//Encrypting Blocks of 16 bytes and writing the output.txt with ciphertext		 
	AES_ctr128_encrypt(read, write, size, &key, state.ivec, state.ecount, &state.num);
}

int main(int argc, char *argv[])
{
	unsigned char w[1024],w1[1024];
	string d;
	cin>>d;
    fencrypt(reinterpret_cast< const unsigned char*>(d.c_str()), w,d.size(), (unsigned const char*)"1234567812345678");
    printf("%s\n",w );
    int n;
    printf("Enter Required Prefix Length\n");
    cin>>n;
	fdecrypt(w, w1,n, (unsigned const char*)"1234567812345678");
	  printf("%.*s\n",n,w1 );
	return 0;
}
