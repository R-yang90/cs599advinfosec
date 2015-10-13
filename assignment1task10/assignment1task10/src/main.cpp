//============================================================================
// Name        : tt.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;


void handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}


int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *ciphertext, unsigned char *key, int type)
{
	EVP_CIPHER_CTX *ctx;
	int len;
	int ciphertext_len;
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	if(type == 1){
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, NULL)) handleErrors();
		if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
		ciphertext_len = len;
	}
	else if (type == 0){
		if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL)) handleErrors();
		if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
		ciphertext_len = len;
	}

	return ciphertext_len;

}


int main (int argc, char **argv)
{
	if (argc < 6) {
		cout << "Usage: tt -in file -k key -ecb[-cbc]\n";
		return 0;
	} else if (strcmp(argv[1], "-in") != 0) {
		cout << "Usage: tt -in file -k key -ecb[-cbc]\n";
		return 0;
	} else if (strcmp (argv[3], "-k") != 0){
		cout << "Usage: tt -in file -k key -ecb[-cbc]\n";
		return 0;
	} else if (strcmp (argv[5], "-cbc") != 0 || strcmp (argv[5], "-ecb") != 0) {
		if (strcmp (argv[5], "-cbc") != 0 && strcmp (argv[5], "-ecb") != 0) {
			cout << "Usage: tt -in file -k key -ecb[-cbc]\n";
			return 0;
		}
	}


	int type;
	if(strcmp (argv[5], "-cbc") == 0){
		type = 1;
	}
	else{
		type = 0;
	}

	unsigned char *key = (unsigned char *) argv[4];

	ifstream myfile;

	myfile.open (argv[2]);
	if (myfile.is_open()){


		string line;
		string lines;
		while ( getline (myfile,line) ){
		  //cout << line << '\n';
		  lines += line;
		  lines += "\n";
		}

		int length = lines.length()*sizeof(char);
		char *plaintext = new char[length];
		//cout << lines;
		const char *plaintext_tmp = lines.c_str();
		strcpy(plaintext,plaintext_tmp);


		cout << plaintext;

		unsigned char *ciphertext;
		unsigned char * plaintext1 = (unsigned char*)plaintext;
		int plaintext_len = strlen(plaintext);
		encrypt(plaintext1, plaintext_len, ciphertext, key, type);
		cout << ciphertext;
		myfile.close();
	} else {
		cout << "Please specify an existing file.\n";
	}

	return 0;
}


