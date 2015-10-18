#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include "version.h"
using namespace std;

enum {ecb = 0, cbc = 1};

int debug;

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

void log(string message)
{
	if (debug == 1) {
		cout << message << endl;
	}
}

unsigned char *encrypt(unsigned char *plaintext, int p_len, unsigned char *key, int *c_len, int type)
{
	EVP_CIPHER_CTX *ctx;
	const EVP_CIPHER *cipher_type;
	int len;
	int r_len = 0;
	int max_block_size = 8;

	if (!(ctx = EVP_CIPHER_CTX_new())) {
		handleErrors();
	}

	if (type == cbc){
		cipher_type = EVP_aes_128_cbc();
	} else if (type == ecb) {
		cipher_type = EVP_aes_128_ecb();
	}

	if (EVP_EncryptInit_ex(ctx, cipher_type, NULL, key, NULL) != 1) {
		handleErrors();
	}

	unsigned char *ciphertext = (unsigned char*) malloc(p_len + EVP_CIPHER_CTX_block_size(ctx));

	if (EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, p_len) != 1) {
		handleErrors();
	}

	r_len = len;

	if (EVP_EncryptFinal_ex(ctx, ciphertext + len, &len) != 1) {
		handleErrors();
	}

	r_len += len;
	*c_len = r_len;
	EVP_CIPHER_CTX_free(ctx);
	return ciphertext;
}

unsigned char *decrypt(unsigned char *ciphertext, int c_len, unsigned char *key, int *p_len, int type)
{
	EVP_CIPHER_CTX *ctx;
	const EVP_CIPHER *cipher_type;
	int len;
	int r_len;

	if (!(ctx = EVP_CIPHER_CTX_new())) {
		handleErrors();
	}

	if (type == cbc) {
		cipher_type = EVP_aes_128_cbc();
	} else if(type == ecb) {
		cipher_type = EVP_aes_128_ecb();
	}

	if (EVP_DecryptInit_ex(ctx, cipher_type, NULL, key, NULL) != 1) {
		handleErrors();
	}

	unsigned char *plaintext = (unsigned char*) malloc(c_len + EVP_CIPHER_CTX_block_size(ctx) + 1);

	if (EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, c_len) != 1) {
		handleErrors();
	}

	r_len = len;

	if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) != 1) {
		handleErrors();
	}

	r_len += len;
	*p_len = r_len;

	EVP_CIPHER_CTX_free(ctx);

	return plaintext;
}


int main (int argc, char **argv)
{
	//argv[1] -in or -v
	//argv[2] input filepath
	//argv[3] -k option
	//argv[4] key ex 0011223344556677
	//argv[5] -ecb or -cbc
	//argv[6] -e or -d
	//argv[7] output filepath
	//argv[8] -debug

	string info_message = "Usage: cryptotool -in input -k key -ecb[-cbc] -e[-d] output [-debug]\n";
	if (argc < 6) {
		if (1 < argc && argc < 3 && strcmp(argv[1], "-v") == 0) {
			printf("cryptotool Version: %s, Authors: %s\n", VERSION, AUTHORS);
		} else {
			cout << info_message;
		}
		return 0;
	} else if (strcmp(argv[1], "-in") != 0) {
		cout << info_message;
		return 0;
	} else if (strcmp (argv[3], "-k") != 0){
		cout << info_message;
		return 0;
	} else if (strcmp (argv[5], "-cbc") != 0 || strcmp (argv[5], "-ecb") != 0) {
		if (strcmp (argv[5], "-cbc") != 0 && strcmp (argv[5], "-ecb") != 0) {
			cout << info_message;
			return 0;
		}
	} else if (strcmp (argv[6], "-e") != 0 || strcmp (argv[6], "-d") != 0) {
		if (strcmp (argv[5], "-e") != 0 && strcmp (argv[5], "-d") != 0) {
			cout << info_message;
			return 0;
		}
	}

	int type;
	if (strcmp(argv[5], "-cbc") == 0) {
		type = cbc;
	} else {
		type = ecb;
	}

	if (argc > 8) {
		debug = strcmp(argv[8], "-debug") == -1 ? 0 : 1;
	}


	unsigned char *key = (unsigned char *) argv[4];
	int key_len = strlen((char *)key);
	unsigned char *fine_key;

	/*padding if key length is not enough*/
	if(key_len < 16){
		fine_key = new unsigned char[16];
		strcpy((char *)fine_key, (char *)key);
		for(int i=key_len; i<16; ++i){
			fine_key[i] = '0';
		}
	}
	else{
		fine_key = key;
	}


	ifstream myfile;
	myfile.open (argv[2]);
	int intext_len;

	if (myfile.is_open()) {
		myfile.seekg(0, myfile.end);
		intext_len = myfile.tellg();
		myfile.seekg(0, myfile.beg);

		unsigned char *intext = new unsigned char[intext_len];
		myfile.read ((char*)intext, intext_len);
		myfile.close();

		log("------------Original data from [ " + (string) argv[2] + " ]");
		log((char*)intext);

		ERR_load_crypto_strings();
		OpenSSL_add_all_algorithms();
		OPENSSL_config(NULL);

		if (strcmp(argv[6], "-e") == 0) {
			int c_len;
			log("------------Encrypt data with " + (string) argv[5]);
			unsigned char *ciphertext = encrypt(intext, intext_len, fine_key, &c_len, type);
			log((char *)ciphertext);
			if (debug == 1) {
				BIO_dump_fp (stdout, (char*)ciphertext, c_len);
			}

			ofstream fout;
			fout.open(argv[7]);
			fout.write((char*)ciphertext, c_len);
			fout.close();
		} else {
			int p_len;

			log("------------Decrypt data with " + (string) argv[5]);
			if (debug == 1) {
				BIO_dump_fp (stdout, (char *)intext, intext_len);
			}
			unsigned char *plaintext = decrypt(intext, intext_len, fine_key, &p_len, type);
			plaintext[p_len] = '\0';
			log((char*)plaintext);

			ofstream fout;
			fout.open(argv[7]);
			fout.write ((char *)plaintext,p_len);
			fout.close();
		}
	} else {
		cout << "Please specify an existing file.\n";
	}

	EVP_cleanup();
	ERR_free_strings();

  return 0;
}
